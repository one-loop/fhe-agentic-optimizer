#include "fhe_operators.hpp"

#include <seal/seal.h>

#include <algorithm>
#include <chrono>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <stdexcept>
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

void print_result(const char* name, double ms, double error) {
    std::cout << std::left << std::setw(18) << name
              << " latency_ms=" << std::fixed << std::setprecision(3) << ms
              << " max_error=" << std::scientific << error << '\n';
}

} // namespace

int main() {
    using namespace seal;
    using namespace chehab::dl;

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

    // ------------------------------------------------------------
    // Quad: exact square activation.
    // ------------------------------------------------------------
    {
        std::vector<double> x{-2.0, -1.0, -0.25, 0.0, 0.5, 1.5, 2.0};
        Plaintext pt;
        encoder.encode(x, scale, pt);
        Ciphertext ct;
        encryptor.encrypt(pt, ct);

        const auto start = Clock::now();
        auto out_ct = quad(ct, evaluator, relin_keys);
        const auto stop = Clock::now();

        auto out = decrypt_decode(out_ct, decryptor, encoder, x.size());
        std::vector<double> expected(x.size());
        for (std::size_t i = 0; i < x.size(); ++i) expected[i] = x[i] * x[i];
        const double ms = std::chrono::duration<double, std::milli>(stop - start).count();
        print_result("Quad", ms, max_abs_error(out, expected));
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

        Plaintext pt;
        encoder.encode(x, scale, pt);
        Ciphertext ct;
        encryptor.encrypt(pt, ct);

        const auto start = Clock::now();
        auto out_ct = batch_norm(ct, s, b, context, encoder, evaluator, scale);
        const auto stop = Clock::now();
        auto out = decrypt_decode(out_ct, decryptor, encoder, x.size());
        const double ms = std::chrono::duration<double, std::milli>(stop - start).count();
        print_result("BatchNorm1d", ms, max_abs_error(out, expected));
        std::cout << "  folded A*x+B vs reference max_diff="
                  << std::scientific << max_abs_error(folded, expected) << '\n';
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

        Plaintext pt;
        encoder.encode(x, scale, pt);
        Ciphertext ct;
        encryptor.encrypt(pt, ct);

        const auto start = Clock::now();
        auto out_ct = batch_norm(ct, s, b, context, encoder, evaluator, scale);
        const auto stop = Clock::now();
        auto out = decrypt_decode(out_ct, decryptor, encoder, x.size());
        const double ms = std::chrono::duration<double, std::milli>(stop - start).count();
        print_result("BatchNorm2d", ms, max_abs_error(out, expected));
        std::cout << "  folded A*x+B vs reference max_diff="
                  << std::scientific << max_abs_error(folded, expected) << '\n';
    }

    // ------------------------------------------------------------
    // Chebyshev: degree-5 sigmoid approximation on [-4,4].
    // Coefficients generated by generate_chebyshev_coeffs.py.
    // ------------------------------------------------------------
    {
        const double lo = -4.0, hi = 4.0;
        // numpy.polynomial.chebyshev.chebfit(z, sigmoid(x), 5)
        const std::vector<double> coeffs{
            5.0000000000000033e-01,
            5.6085648526285004e-01,
            3.6141304177580773e-17,
           -9.2319238911727442e-02,
           -7.8654830959237120e-18,
            2.4061351870331430e-02
        };
        std::vector<double> x{-4.0, -3.0, -2.0, -1.0, 0.0, 1.0, 2.0, 3.0, 4.0};
        auto poly_ref = chebyshev_plain(x, coeffs, lo, hi);
        std::vector<double> true_sigmoid(x.size());
        for (std::size_t i = 0; i < x.size(); ++i) {
            true_sigmoid[i] = 1.0 / (1.0 + std::exp(-x[i]));
        }

        Plaintext pt;
        encoder.encode(x, scale, pt);
        Ciphertext ct;
        encryptor.encrypt(pt, ct);

        const auto start = Clock::now();
        auto out_ct = chebyshev(
            ct, coeffs, lo, hi, context, encoder, evaluator, relin_keys, scale);
        const auto stop = Clock::now();
        auto out = decrypt_decode(out_ct, decryptor, encoder, x.size());
        const double ms = std::chrono::duration<double, std::milli>(stop - start).count();

        print_result("ChebyshevSigmoid", ms, max_abs_error(out, poly_ref));
        std::cout << "  polynomial approximation max_error="
                  << std::scientific << max_abs_error(poly_ref, true_sigmoid) << '\n';
    }

    return 0;
}
