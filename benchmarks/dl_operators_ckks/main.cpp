#include "activations.hpp"
#include "chebyshev_coeffs.hpp"
#include "fhe_operators.hpp"

#include <seal/seal.h>

#include <algorithm>
#include <chrono>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <optional>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

namespace {

using Clock = std::chrono::high_resolution_clock;

std::vector<double> decrypt_decode(
    const seal::Ciphertext& ct,
    seal::Decryptor& decryptor,
    seal::CKKSEncoder& encoder,
    std::size_t count) {
    seal::Plaintext pt;
    decryptor.decrypt(ct, pt);
    std::vector<double> values;
    encoder.decode(pt, values);
    values.resize(count);
    return values;
}

double max_abs_error(const std::vector<double>& a,
                     const std::vector<double>& b) {
    if (a.size() != b.size()) {
        throw std::invalid_argument("error vectors have different sizes");
    }
    double e = 0.0;
    for (std::size_t i = 0; i < a.size(); ++i) {
        e = std::max(e, std::abs(a[i] - b[i]));
    }
    return e;
}

std::vector<double> chebyshev_plain(
    const std::vector<double>& x,
    const std::vector<double>& coeffs,
    double lo,
    double hi) {
    std::vector<double> out(x.size(), 0.0);
    for (std::size_t i = 0; i < x.size(); ++i) {
        const double z = 2.0 * (x[i] - lo) / (hi - lo) - 1.0;
        double t0 = 1.0;
        double value = coeffs[0] * t0;
        if (coeffs.size() > 1) {
            double t1 = z;
            value += coeffs[1] * t1;
            for (std::size_t k = 2; k < coeffs.size(); ++k) {
                const double tk = 2.0 * z * t1 - t0;
                value += coeffs[k] * tk;
                t0 = t1;
                t1 = tk;
            }
        }
        out[i] = value;
    }
    return out;
}

const chehab::dl::ChebyshevCoeffSet& find_coeff_set(const std::string& activation, int degree) {
    for (const auto& set : chehab::dl::chebyshev_coeff_sets()) {
        if (activation == set.activation && degree == set.degree) {
            return set;
        }
    }
    throw std::invalid_argument(
        "no committed Chebyshev coefficient set for " + activation
        + " degree " + std::to_string(degree)
        + "; add it to COEFF_SETS in generate_chebyshev_coeffs.py");
}

// Eval-mode BatchNorm on flattened [N, C, spatial...] data, computed from the
// original statistics. Deliberately independent of precompute_batch_norm()
// and the expand_* packing helpers: the channel of slot i is derived directly
// from its index.
std::vector<double> batch_norm_reference(
    const std::vector<double>& x,
    const std::vector<double>& gamma,
    const std::vector<double>& beta,
    const std::vector<double>& running_mean,
    const std::vector<double>& running_var,
    double eps,
    std::size_t channels,
    std::size_t spatial_size) {
    std::vector<double> out(x.size());
    for (std::size_t i = 0; i < x.size(); ++i) {
        const std::size_t c = (i / spatial_size) % channels;
        out[i] = gamma[c] * (x[i] - running_mean[c]) / std::sqrt(running_var[c] + eps)
                 + beta[c];
    }
    return out;
}

// PASS/FAIL tolerance: absolute error between the decrypted CKKS output and a
// plaintext reference of the same computation. This is the conservative CKKS
// tolerance suggested in the project handoff. With N = 32768 and a 2^40 scale
// the observed errors are 1e-9..4e-7 (Chebyshev up to degree 9 included), so
// it leaves ample headroom; it is NOT tight enough to catch a small
// systematic bias such as the 5.7e-6 forced-scale error fixed earlier.
// Polynomial approximation error (true activation vs the fitted polynomial)
// is a modelling property, reported separately and not part of PASS/FAIL.
constexpr double fhe_abs_tolerance = 1e-4;

// BatchNorm folding check: folded A*x+B vs the original-formula reference,
// both in plaintext double arithmetic.
constexpr double fold_abs_tolerance = 1e-12;

struct Result {
    std::string name;
    double max_fhe_error = 0.0;           // decrypted output vs plaintext reference
    // Latency of the encrypted operator alone (plaintext encoding done inside
    // the operator included; encryption, decryption and key generation
    // excluded), over `timed_runs` runs after one untimed warmup run.
    double median_ms = 0.0;
    double min_ms = 0.0;
    double max_ms = 0.0;
    std::size_t timed_runs = 0;
    std::size_t start_chain_index = 0;    // chain index of the fresh input
    std::size_t end_chain_index = 0;      // chain index of the output
    double output_scale_log2 = 0.0;
    // Longest chain of ciphertext-ciphertext multiplications, from the
    // operator's evaluation structure (not measured): Quad 1, BatchNorm 0,
    // Chebyshev recurrence of degree d: d - 1.
    std::size_t ct_ct_depth = 0;
    std::optional<double> fold_diff;      // BatchNorm only
    std::optional<double> approx_error;   // Chebyshev only, informational

    bool pass() const {
        return max_fhe_error <= fhe_abs_tolerance
               && (!fold_diff || *fold_diff <= fold_abs_tolerance);
    }
};

double median(std::vector<double> v) {
    std::sort(v.begin(), v.end());
    const std::size_t n = v.size();
    return n % 2 == 1 ? v[n / 2] : 0.5 * (v[n / 2 - 1] + v[n / 2]);
}

// Encrypt x, run op once as a warmup and then `timed_runs` more times,
// decrypt the last output, and compare it with expected.
template <typename Op>
Result evaluate(
    std::string name,
    const std::vector<double>& x,
    const std::vector<double>& expected,
    Op op,
    std::size_t ct_ct_depth,
    std::size_t timed_runs,
    double scale,
    const seal::SEALContext& context,
    seal::CKKSEncoder& encoder,
    seal::Encryptor& encryptor,
    seal::Decryptor& decryptor) {
    seal::Plaintext pt;
    encoder.encode(x, scale, pt);
    seal::Ciphertext ct;
    encryptor.encrypt(pt, ct);

    seal::Ciphertext out_ct = op(ct); // warmup, not timed
    std::vector<double> times_ms;
    times_ms.reserve(timed_runs);
    for (std::size_t i = 0; i < timed_runs; ++i) {
        const auto start = Clock::now();
        out_ct = op(ct);
        const auto stop = Clock::now();
        times_ms.push_back(std::chrono::duration<double, std::milli>(stop - start).count());
    }

    Result r;
    r.name = std::move(name);
    r.max_fhe_error = max_abs_error(
        decrypt_decode(out_ct, decryptor, encoder, x.size()), expected);
    r.median_ms = median(times_ms);
    r.min_ms = *std::min_element(times_ms.begin(), times_ms.end());
    r.max_ms = *std::max_element(times_ms.begin(), times_ms.end());
    r.timed_runs = timed_runs;
    r.start_chain_index = context.get_context_data(ct.parms_id())->chain_index();
    r.end_chain_index = context.get_context_data(out_ct.parms_id())->chain_index();
    r.output_scale_log2 = std::log2(out_ct.scale());
    r.ct_ct_depth = ct_ct_depth;
    return r;
}

void print_table(const std::vector<Result>& results) {
    std::cout << std::left << std::setw(22) << "operator"
              << std::setw(8) << "status"
              << std::setw(13) << "max_fhe_err"
              << std::setw(13) << "approx_err"
              << std::setw(7) << "depth"
              << std::setw(14) << "chain(s->e)"
              << std::setw(8) << "levels"
              << std::setw(12) << "out_log2s"
              << "median_ms (min..max)\n";
    for (const auto& r : results) {
        std::cout << std::left << std::setw(22) << r.name
                  << std::setw(8) << (r.pass() ? "PASS" : "FAIL")
                  << std::scientific << std::setprecision(3)
                  << std::setw(13) << r.max_fhe_error;
        if (r.approx_error) {
            std::cout << std::setw(13) << *r.approx_error;
        } else {
            std::cout << std::setw(13) << "n/a";
        }
        std::cout << std::setw(7) << r.ct_ct_depth
                  << std::setw(14)
                  << (std::to_string(r.start_chain_index) + "->"
                      + std::to_string(r.end_chain_index))
                  << std::setw(8) << (r.start_chain_index - r.end_chain_index)
                  << std::fixed << std::setprecision(6) << std::setw(12)
                  << r.output_scale_log2;
        std::cout << std::fixed << std::setprecision(3) << r.median_ms
                  << " (" << r.min_ms << ".." << r.max_ms << ")\n";
    }
    std::cout << "depth = ciphertext-ciphertext multiplicative depth; "
                 "levels = start chain index - end chain index\n";
    if (!results.empty()) {
        std::cout << "latency: encrypted operator only, 1 warmup + "
                  << results.front().timed_runs << " timed runs\n";
    }
    std::cout << "tolerance: |fhe - plaintext reference| <= " << std::scientific
              << std::setprecision(1) << fhe_abs_tolerance
              << " (BatchNorm folding <= " << fold_abs_tolerance << ")\n";
}

} // namespace

int main(int argc, char** argv) {
    using namespace seal;
    using namespace chehab::dl;

    std::size_t timed_runs = 10;
    for (int i = 1; i < argc; ++i) {
        const std::string arg = argv[i];
        if (arg == "--repeats" && i + 1 < argc) {
            timed_runs = std::stoul(argv[++i]);
        } else {
            std::cerr << "usage: " << argv[0] << " [--repeats N]\n";
            return 2;
        }
    }
    if (timed_runs == 0) {
        std::cerr << "--repeats must be at least 1\n";
        return 2;
    }

    EncryptionParameters parms(scheme_type::ckks);
    const std::size_t poly_modulus_degree = 32768;
    parms.set_poly_modulus_degree(poly_modulus_degree);
    parms.set_coeff_modulus(CoeffModulus::Create(
        poly_modulus_degree,
        {60, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 60}));

    SEALContext context(parms);
    if (!context.parameters_set()) {
        std::cerr << "Invalid SEAL parameters\n";
        return 2;
    }

    KeyGenerator keygen(context);
    SecretKey secret_key = keygen.secret_key();
    PublicKey public_key;
    keygen.create_public_key(public_key);
    RelinKeys relin_keys;
    keygen.create_relin_keys(relin_keys);

    Encryptor encryptor(context, public_key);
    Evaluator evaluator(context);
    Decryptor decryptor(context, secret_key);
    CKKSEncoder encoder(context);

    const double scale = std::pow(2.0, 40);

    std::vector<Result> results;

    // ------------------------------------------------------------
    // Quad: exact square activation.
    // ------------------------------------------------------------
    {
        std::vector<double> x{-2.0, -1.0, -0.25, 0.0, 0.5, 1.5, 2.0};
        std::vector<double> expected(x.size());
        for (std::size_t i = 0; i < x.size(); ++i) expected[i] = x[i] * x[i];

        results.push_back(evaluate(
            "Quad", x, expected,
            [&](const Ciphertext& ct) { return quad(ct, evaluator, relin_keys); },
            1, timed_runs, scale, context, encoder, encryptor, decryptor));
    }

    // ------------------------------------------------------------
    // BatchNorm1d: [N,C,L] flattened in NCL order.
    // ------------------------------------------------------------
    {
        const std::size_t N = 1, C = 2, L = 4;
        std::vector<double> x{-1.0, -0.5, 0.0, 0.5,
                              1.0,  1.5, 2.0, 2.5};
        std::vector<double> gamma{1.2, 0.8};
        std::vector<double> beta{0.1, -0.2};
        std::vector<double> mean{0.25, 1.25};
        std::vector<double> var{0.5, 0.75};
        const double eps = 1e-5;
        auto bn = precompute_batch_norm(gamma, beta, mean, var, eps);
        auto s = expand_batchnorm1d_channels(bn.scale, N, C, L);
        auto b = expand_batchnorm1d_channels(bn.shift, N, C, L);

        auto expected = batch_norm_reference(x, gamma, beta, mean, var, eps, C, L);
        std::vector<double> folded(x.size());
        for (std::size_t i = 0; i < x.size(); ++i) folded[i] = s[i] * x[i] + b[i];

        auto r = evaluate(
            "BatchNorm1d", x, expected,
            [&](const Ciphertext& ct) {
                return batch_norm(ct, s, b, context, encoder, evaluator, scale);
            },
            0, timed_runs, scale, context, encoder, encryptor, decryptor);
        r.fold_diff = max_abs_error(folded, expected);
        results.push_back(r);
    }

    // ------------------------------------------------------------
    // BatchNorm2d: [N,C,H,W] flattened in NCHW order.
    // ------------------------------------------------------------
    {
        const std::size_t N = 1, C = 2, H = 2, W = 2;
        std::vector<double> x{-1.0, -0.5, 0.0, 0.5,
                              1.0,  1.5, 2.0, 2.5};
        std::vector<double> gamma{1.1, 0.7};
        std::vector<double> beta{0.0, 0.2};
        std::vector<double> mean{0.1, 1.0};
        std::vector<double> var{0.9, 0.6};
        const double eps = 1e-5;
        auto bn = precompute_batch_norm(gamma, beta, mean, var, eps);
        auto s = expand_batchnorm2d_channels(bn.scale, N, C, H, W);
        auto b = expand_batchnorm2d_channels(bn.shift, N, C, H, W);

        auto expected = batch_norm_reference(x, gamma, beta, mean, var, eps, C, H * W);
        std::vector<double> folded(x.size());
        for (std::size_t i = 0; i < x.size(); ++i) folded[i] = s[i] * x[i] + b[i];

        auto r = evaluate(
            "BatchNorm2d", x, expected,
            [&](const Ciphertext& ct) {
                return batch_norm(ct, s, b, context, encoder, evaluator, scale);
            },
            0, timed_runs, scale, context, encoder, encryptor, decryptor);
        r.fold_diff = max_abs_error(folded, expected);
        results.push_back(r);
    }

    // ------------------------------------------------------------
    // Chebyshev activations. One generic evaluator; each entry selects a
    // committed coefficient set from chebyshev_coeffs.hpp by activation and
    // degree. Add entries here to test more activations.
    // ------------------------------------------------------------
    const std::vector<std::pair<std::string, int>> encrypted_activation_tests{
        {"sigmoid", 5},
    };
    constexpr std::size_t activation_test_points = 9;
    for (const auto& [activation, degree] : encrypted_activation_tests) {
        const ChebyshevCoeffSet& set = find_coeff_set(activation, degree);
        const double lo = set.range_min, hi = set.range_max;

        std::vector<double> x(activation_test_points);
        for (std::size_t i = 0; i < x.size(); ++i) {
            x[i] = lo + (hi - lo) * static_cast<double>(i) / (x.size() - 1);
        }
        auto poly_ref = chebyshev_plain(x, set.coeffs, lo, hi);
        std::vector<double> true_activation(x.size());
        for (std::size_t i = 0; i < x.size(); ++i) {
            true_activation[i] = activation_reference(activation, x[i]);
        }

        auto r = evaluate(
            "Chebyshev-" + activation + "-d" + std::to_string(degree), x, poly_ref,
            [&](const Ciphertext& ct) {
                return chebyshev(
                    ct, set.coeffs, lo, hi, context, encoder, evaluator, relin_keys, scale);
            },
            set.coeffs.size() - 2, timed_runs, scale, context, encoder, encryptor, decryptor);
        r.approx_error = max_abs_error(poly_ref, true_activation);
        results.push_back(r);
    }

    print_table(results);

    const bool all_pass = std::all_of(
        results.begin(), results.end(), [](const Result& r) { return r.pass(); });
    return all_pass ? 0 : 1;
}
