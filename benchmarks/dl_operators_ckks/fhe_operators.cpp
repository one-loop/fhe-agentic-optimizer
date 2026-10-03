#include "fhe_operators.hpp"

#include <algorithm>
#include <cmath>
#include <optional>
#include <sstream>
#include <stdexcept>

namespace chehab::dl {
namespace {

// Chebyshev coefficients at or below this magnitude are fitting noise
// (e.g. even terms of an odd function) and are not evaluated.
constexpr double coeff_epsilon = 1e-14;

// Two scales constructed to be equal (e.g. s * p / q with p = target * q / s)
// differ only by floating-point rounding, ~1e-16 relative. SEAL's add requires
// near bit-equal scales, so such scales are snapped together. Any larger
// difference is a real mismatch and is reported instead of being hidden; a
// relative scale change of 1e-9 would perturb decoded values by at most 1e-9
// relative, far below CKKS noise at a 2^40 scale.
constexpr double scale_rel_tolerance = 1e-9;

void require_same_size(const std::vector<double>& a,
                       const std::vector<double>& b,
                       const char* what) {
    if (a.size() != b.size()) {
        throw std::invalid_argument(std::string(what) + ": size mismatch");
    }
}

std::size_t chain_index(const seal::SEALContext& context, seal::parms_id_type parms_id) {
    return context.get_context_data(parms_id)->chain_index();
}

// The prime removed by rescale_to_next at this level.
double last_prime(const seal::SEALContext& context, seal::parms_id_type parms_id) {
    return static_cast<double>(
        context.get_context_data(parms_id)->parms().coeff_modulus().back().value());
}

bool same_scale(double a, double b) {
    return std::abs(a - b) <= scale_rel_tolerance * std::max(std::abs(a), std::abs(b));
}

void snap_scale(seal::Ciphertext& ct, double expected) {
    if (!same_scale(ct.scale(), expected)) {
        std::ostringstream msg;
        msg << "CKKS scale mismatch: have " << ct.scale() << ", expected " << expected;
        throw std::logic_error(msg.str());
    }
    ct.scale() = expected;
}

void mod_switch_to(seal::Ciphertext& ct,
                   seal::parms_id_type parms_id,
                   seal::Evaluator& evaluator) {
    if (ct.parms_id() != parms_id) {
        evaluator.mod_switch_to_inplace(ct, parms_id);
    }
}

// ct * values followed by a rescale, with the result landing on out_scale.
template <typename Values>
seal::Ciphertext multiply_plain_to_scale(
    const seal::Ciphertext& ct,
    const Values& values,
    double out_scale,
    const seal::SEALContext& context,
    seal::CKKSEncoder& encoder,
    seal::Evaluator& evaluator) {
    const double plain_scale = out_scale * last_prime(context, ct.parms_id()) / ct.scale();
    seal::Plaintext pt;
    encoder.encode(values, ct.parms_id(), plain_scale, pt);
    seal::Ciphertext out;
    evaluator.multiply_plain(ct, pt, out);
    evaluator.rescale_to_next_inplace(out);
    snap_scale(out, out_scale);
    return out;
}

// ct + values, with values encoded at the ciphertext's own level and scale.
template <typename Values>
void add_plain_inplace(
    seal::Ciphertext& ct,
    const Values& values,
    seal::CKKSEncoder& encoder,
    seal::Evaluator& evaluator) {
    seal::Plaintext pt;
    encoder.encode(values, ct.parms_id(), ct.scale(), pt);
    evaluator.add_plain_inplace(ct, pt);
}

// Bring ct to (parms_id, scale). If the scales already agree this is a plain
// mod switch. Otherwise ct is multiplied by 1 encoded so the rescaled result
// lands on the target scale, which requires ct to sit above the target level.
seal::Ciphertext align_to(
    seal::Ciphertext ct,
    seal::parms_id_type parms_id,
    double scale,
    const seal::SEALContext& context,
    seal::CKKSEncoder& encoder,
    seal::Evaluator& evaluator) {
    if (!same_scale(ct.scale(), scale)) {
        if (chain_index(context, ct.parms_id()) <= chain_index(context, parms_id)) {
            throw std::logic_error(
                "cannot align CKKS scale without a spare level above the target");
        }
        ct = multiply_plain_to_scale(ct, 1.0, scale, context, encoder, evaluator);
    }
    mod_switch_to(ct, parms_id, evaluator);
    snap_scale(ct, scale);
    return ct;
}

// lhs * rhs at the lower of the two levels, relinearized and rescaled. The
// result keeps its natural scale lhs.scale() * rhs.scale() / q_l.
seal::Ciphertext multiply_rescale(
    seal::Ciphertext lhs,
    seal::Ciphertext rhs,
    const seal::SEALContext& context,
    seal::Evaluator& evaluator,
    const seal::RelinKeys& relin_keys) {
    if (chain_index(context, lhs.parms_id()) > chain_index(context, rhs.parms_id())) {
        mod_switch_to(lhs, rhs.parms_id(), evaluator);
    } else {
        mod_switch_to(rhs, lhs.parms_id(), evaluator);
    }
    seal::Ciphertext out;
    evaluator.multiply(lhs, rhs, out);
    evaluator.relinearize_inplace(out, relin_keys);
    evaluator.rescale_to_next_inplace(out);
    return out;
}

} // namespace

BatchNormParams precompute_batch_norm(
    const std::vector<double>& gamma,
    const std::vector<double>& beta,
    const std::vector<double>& running_mean,
    const std::vector<double>& running_var,
    double eps) {
    require_same_size(gamma, beta, "batchnorm gamma/beta");
    require_same_size(gamma, running_mean, "batchnorm gamma/running_mean");
    require_same_size(gamma, running_var, "batchnorm gamma/running_var");
    if (eps <= 0.0) {
        throw std::invalid_argument("batchnorm eps must be positive");
    }

    BatchNormParams result;
    result.scale.resize(gamma.size());
    result.shift.resize(gamma.size());
    for (std::size_t c = 0; c < gamma.size(); ++c) {
        if (running_var[c] < 0.0) {
            throw std::invalid_argument("batchnorm running variance must be non-negative");
        }
        const double s = gamma[c] / std::sqrt(running_var[c] + eps);
        result.scale[c] = s;
        result.shift[c] = beta[c] - s * running_mean[c];
    }
    return result;
}

std::vector<double> expand_batchnorm1d_channels(
    const std::vector<double>& per_channel,
    std::size_t batch,
    std::size_t channels,
    std::size_t length) {
    if (per_channel.size() != channels) {
        throw std::invalid_argument("BatchNorm1d channel parameter count mismatch");
    }
    std::vector<double> out(batch * channels * length);
    std::size_t idx = 0;
    for (std::size_t n = 0; n < batch; ++n) {
        for (std::size_t c = 0; c < channels; ++c) {
            for (std::size_t l = 0; l < length; ++l) {
                out[idx++] = per_channel[c];
            }
        }
    }
    return out;
}

std::vector<double> expand_batchnorm2d_channels(
    const std::vector<double>& per_channel,
    std::size_t batch,
    std::size_t channels,
    std::size_t height,
    std::size_t width) {
    if (per_channel.size() != channels) {
        throw std::invalid_argument("BatchNorm2d channel parameter count mismatch");
    }
    std::vector<double> out(batch * channels * height * width);
    std::size_t idx = 0;
    for (std::size_t n = 0; n < batch; ++n) {
        for (std::size_t c = 0; c < channels; ++c) {
            for (std::size_t h = 0; h < height; ++h) {
                for (std::size_t w = 0; w < width; ++w) {
                    out[idx++] = per_channel[c];
                }
            }
        }
    }
    return out;
}

seal::Ciphertext batch_norm(
    const seal::Ciphertext& input,
    const std::vector<double>& expanded_scale,
    const std::vector<double>& expanded_shift,
    const seal::SEALContext& context,
    seal::CKKSEncoder& encoder,
    seal::Evaluator& evaluator,
    double output_scale) {
    require_same_size(expanded_scale, expanded_shift, "batchnorm scale/shift");
    if (expanded_scale.empty()) {
        throw std::invalid_argument("batchnorm parameter vectors cannot be empty");
    }

    seal::Ciphertext out = multiply_plain_to_scale(
        input, expanded_scale, output_scale, context, encoder, evaluator);
    add_plain_inplace(out, expanded_shift, encoder, evaluator);
    return out;
}

seal::Ciphertext quad(
    const seal::Ciphertext& input,
    seal::Evaluator& evaluator,
    const seal::RelinKeys& relin_keys) {
    seal::Ciphertext out;
    evaluator.square(input, out);
    evaluator.relinearize_inplace(out, relin_keys);
    evaluator.rescale_to_next_inplace(out);
    return out;
}

seal::Ciphertext chebyshev(
    const seal::Ciphertext& input,
    const std::vector<double>& coeffs,
    double range_min,
    double range_max,
    const seal::SEALContext& context,
    seal::CKKSEncoder& encoder,
    seal::Evaluator& evaluator,
    const seal::RelinKeys& relin_keys,
    double output_scale) {

    if (coeffs.size() < 2) {
        throw std::invalid_argument(
            "Chebyshev polynomial must have degree >= 1");
    }

    if (!(range_max > range_min)) {
        throw std::invalid_argument(
            "Chebyshev range_max must exceed range_min");
    }

    // Map x from [range_min, range_max] to z in [-1, 1]: z = alpha*x + beta.
    const double alpha = 2.0 / (range_max - range_min);
    const double beta = -(range_max + range_min) / (range_max - range_min);

    seal::Ciphertext z = multiply_plain_to_scale(
        input, alpha, output_scale, context, encoder, evaluator);
    if (std::abs(beta) > coeff_epsilon) {
        add_plain_inplace(z, beta, encoder, evaluator);
    }

    // Accumulate c_k * T_k for k >= 1. Every term lands on output_scale, so
    // accumulating only needs the running sum switched down to the term's
    // level. The sum starts from the first non-negligible term, which avoids
    // building an encrypted zero (SEAL rejects transparent ciphertexts).
    std::optional<seal::Ciphertext> result;
    auto add_term = [&](const seal::Ciphertext& t, double c) {
        if (std::abs(c) <= coeff_epsilon) {
            return;
        }
        seal::Ciphertext term = multiply_plain_to_scale(
            t, c, output_scale, context, encoder, evaluator);
        if (!result) {
            result = std::move(term);
            return;
        }
        mod_switch_to(*result, term.parms_id(), evaluator);
        snap_scale(*result, output_scale);
        evaluator.add_inplace(*result, term);
    };

    // T1 = z.
    add_term(z, coeffs[1]);

    if (coeffs.size() > 2) {
        // T2 = 2*z^2 - 1, built explicitly so no encrypted T0 is needed.
        seal::Ciphertext t_prev2 = z;
        seal::Ciphertext t_prev1 = multiply_rescale(z, z, context, evaluator, relin_keys);
        evaluator.add_inplace(t_prev1, t_prev1);
        add_plain_inplace(t_prev1, -1.0, encoder, evaluator);
        add_term(t_prev1, coeffs[2]);

        // Tk = 2*z*T{k-1} - T{k-2}. T{k-2} sits two levels above Tk, so it can
        // be brought to Tk's exact scale with one plaintext multiply and no
        // extra depth.
        for (std::size_t k = 3; k < coeffs.size(); ++k) {
            seal::Ciphertext tk = multiply_rescale(z, t_prev1, context, evaluator, relin_keys);
            evaluator.add_inplace(tk, tk);
            seal::Ciphertext prev2 = align_to(
                t_prev2, tk.parms_id(), tk.scale(), context, encoder, evaluator);
            evaluator.sub_inplace(tk, prev2);
            add_term(tk, coeffs[k]);

            t_prev2 = std::move(t_prev1);
            t_prev1 = std::move(tk);
        }
    }

    if (!result) {
        throw std::invalid_argument(
            "Chebyshev polynomial has no non-negligible coefficient of degree >= 1");
    }

    // c0 * T0 = c0.
    if (std::abs(coeffs[0]) > coeff_epsilon) {
        add_plain_inplace(*result, coeffs[0], encoder, evaluator);
    }
    return std::move(*result);
}

} // namespace chehab::dl
