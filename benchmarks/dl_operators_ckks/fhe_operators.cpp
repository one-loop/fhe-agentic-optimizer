#include "fhe_operators.hpp"

#include <algorithm>
#include <cmath>
#include <stdexcept>

namespace chehab::dl {
namespace {

void require_same_size(const std::vector<double>& a,
                       const std::vector<double>& b,
                       const char* what) {
    if (a.size() != b.size()) {
        throw std::invalid_argument(std::string(what) + ": size mismatch");
    }
}

seal::Plaintext encode_vector_at(
    const std::vector<double>& values,
    seal::parms_id_type parms_id,
    double scale,
    seal::CKKSEncoder& encoder,
    seal::Evaluator& evaluator) {
    seal::Plaintext pt;
    encoder.encode(values, scale, pt);
    if (pt.parms_id() != parms_id) {
        evaluator.mod_switch_to_inplace(pt, parms_id);
    }
    return pt;
}

seal::Plaintext encode_scalar_at(
    double value,
    seal::parms_id_type parms_id,
    double scale,
    seal::CKKSEncoder& encoder,
    seal::Evaluator& evaluator) {
    seal::Plaintext pt;
    encoder.encode(value, scale, pt);
    if (pt.parms_id() != parms_id) {
        evaluator.mod_switch_to_inplace(pt, parms_id);
    }
    return pt;
}

void normalize_scale(seal::Ciphertext& ct, double target_scale) {
    ct.scale() = target_scale;
}

void align_ciphertexts(seal::Ciphertext& a,
                       seal::Ciphertext& b,
                       seal::Evaluator& evaluator,
                       double target_scale) {
    if (a.parms_id() != b.parms_id()) {
        // parms_id ordering is not numerically comparable, so try switching copies
        // to each other's level. Only one direction can succeed.
        try {
            evaluator.mod_switch_to_inplace(a, b.parms_id());
        } catch (const std::invalid_argument&) {
            evaluator.mod_switch_to_inplace(b, a.parms_id());
        }
    }
    normalize_scale(a, target_scale);
    normalize_scale(b, target_scale);
}

seal::Ciphertext multiply_rescale(
    seal::Ciphertext lhs,
    seal::Ciphertext rhs,
    seal::Evaluator& evaluator,
    const seal::RelinKeys& relin_keys,
    double target_scale) {
    align_ciphertexts(lhs, rhs, evaluator, target_scale);
    seal::Ciphertext out;
    evaluator.multiply(lhs, rhs, out);
    evaluator.relinearize_inplace(out, relin_keys);
    evaluator.rescale_to_next_inplace(out);
    normalize_scale(out, target_scale);
    return out;
}

seal::Ciphertext multiply_plain_rescale(
    seal::Ciphertext input,
    const seal::Plaintext& plain,
    seal::Evaluator& evaluator,
    double target_scale) {
    seal::Ciphertext out;
    evaluator.multiply_plain(input, plain, out);
    evaluator.rescale_to_next_inplace(out);
    normalize_scale(out, target_scale);
    return out;
}

void add_plain_scalar_inplace(
    seal::Ciphertext& ct,
    double value,
    seal::CKKSEncoder& encoder,
    seal::Evaluator& evaluator,
    double target_scale) {
    auto pt = encode_scalar_at(value, ct.parms_id(), target_scale, encoder, evaluator);
    normalize_scale(ct, target_scale);
    evaluator.add_plain_inplace(ct, pt);
}

void add_plain_vector_inplace(
    seal::Ciphertext& ct,
    const std::vector<double>& value,
    seal::CKKSEncoder& encoder,
    seal::Evaluator& evaluator,
    double target_scale) {
    auto pt = encode_vector_at(value, ct.parms_id(), target_scale, encoder, evaluator);
    normalize_scale(ct, target_scale);
    evaluator.add_plain_inplace(ct, pt);
}

seal::Ciphertext multiply_plain_scalar_rescale(
    const seal::Ciphertext& input,
    double value,
    seal::CKKSEncoder& encoder,
    seal::Evaluator& evaluator,
    double target_scale) {
    auto pt = encode_scalar_at(value, input.parms_id(), target_scale, encoder, evaluator);
    return multiply_plain_rescale(input, pt, evaluator, target_scale);
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
    seal::CKKSEncoder& encoder,
    seal::Evaluator& evaluator,
    double target_scale) {
    require_same_size(expanded_scale, expanded_shift, "batchnorm scale/shift");
    if (expanded_scale.empty()) {
        throw std::invalid_argument("batchnorm parameter vectors cannot be empty");
    }

    auto scale_pt = encode_vector_at(
        expanded_scale, input.parms_id(), target_scale, encoder, evaluator);
    seal::Ciphertext out = multiply_plain_rescale(
        input, scale_pt, evaluator, target_scale);
    add_plain_vector_inplace(out, expanded_shift, encoder, evaluator, target_scale);
    return out;
}

seal::Ciphertext quad(
    const seal::Ciphertext& input,
    seal::Evaluator& evaluator,
    const seal::RelinKeys& relin_keys,
    double target_scale) {
    seal::Ciphertext out;
    evaluator.square(input, out);
    evaluator.relinearize_inplace(out, relin_keys);
    evaluator.rescale_to_next_inplace(out);
    normalize_scale(out, target_scale);
    return out;
}


seal::Ciphertext chebyshev(
    const seal::Ciphertext& input,
    const std::vector<double>& coeffs,
    double range_min,
    double range_max,
    seal::CKKSEncoder& encoder,
    seal::Evaluator& evaluator,
    const seal::RelinKeys& relin_keys,
    double target_scale) {

    if (coeffs.empty()) {
        throw std::invalid_argument(
            "Chebyshev coefficient vector cannot be empty");
    }

    if (!(range_max > range_min)) {
        throw std::invalid_argument(
            "Chebyshev range_max must exceed range_min");
    }

    const double alpha =
        2.0 / (range_max - range_min);

    const double beta =
        -(range_max + range_min) /
        (range_max - range_min);

    seal::Ciphertext z =
        multiply_plain_scalar_rescale(
            input,
            alpha,
            encoder,
            evaluator,
            target_scale);

    if (std::abs(beta) > 1e-14) {
        add_plain_scalar_inplace(
            z,
            beta,
            encoder,
            evaluator,
            target_scale);
    }

    if (coeffs.size() == 1) {
        throw std::invalid_argument(
            "degree-0 Chebyshev polynomial is not supported");
    }

    constexpr double coeff_epsilon = 1e-14;

    seal::Ciphertext result =
        multiply_plain_scalar_rescale(
            z,
            coeffs[1],
            encoder,
            evaluator,
            target_scale);

    if (std::abs(coeffs[0]) > coeff_epsilon) {
        add_plain_scalar_inplace(
            result,
            coeffs[0],
            encoder,
            evaluator,
            target_scale);
    }

    if (coeffs.size() == 2) {
        return result;
    }

    seal::Ciphertext t_prev2 = z;

    seal::Ciphertext t_prev1 =
        multiply_rescale(
            z,
            z,
            evaluator,
            relin_keys,
            target_scale);

    auto two = encode_scalar_at(
        2.0,
        t_prev1.parms_id(),
        1.0,
        encoder,
        evaluator);

    evaluator.multiply_plain_inplace(
        t_prev1,
        two);

    normalize_scale(
        t_prev1,
        target_scale);

    add_plain_scalar_inplace(
        t_prev1,
        -1.0,
        encoder,
        evaluator,
        target_scale);

    if (std::abs(coeffs[2]) > coeff_epsilon) {
        seal::Ciphertext term =
            multiply_plain_scalar_rescale(
                t_prev1,
                coeffs[2],
                encoder,
                evaluator,
                target_scale);

        if (result.parms_id() != term.parms_id()) {
            evaluator.mod_switch_to_inplace(
                result,
                term.parms_id());
        }

        normalize_scale(
            result,
            target_scale);

        normalize_scale(
            term,
            target_scale);

        evaluator.add_inplace(
            result,
            term);
    }

    for (std::size_t k = 3;
         k < coeffs.size();
         ++k) {

        seal::Ciphertext z_k = z;
        seal::Ciphertext prev1_k = t_prev1;

        if (z_k.parms_id() != prev1_k.parms_id()) {
            evaluator.mod_switch_to_inplace(
                z_k,
                prev1_k.parms_id());
        }

        normalize_scale(
            z_k,
            target_scale);

        normalize_scale(
            prev1_k,
            target_scale);

        seal::Ciphertext tk =
            multiply_rescale(
                z_k,
                prev1_k,
                evaluator,
                relin_keys,
                target_scale);

        auto two_k = encode_scalar_at(
            2.0,
            tk.parms_id(),
            1.0,
            encoder,
            evaluator);

        evaluator.multiply_plain_inplace(
            tk,
            two_k);

        normalize_scale(
            tk,
            target_scale);

        seal::Ciphertext prev2_k =
            t_prev2;

        if (prev2_k.parms_id() !=
            tk.parms_id()) {

            evaluator.mod_switch_to_inplace(
                prev2_k,
                tk.parms_id());
        }

        normalize_scale(
            prev2_k,
            target_scale);

        evaluator.sub_inplace(
            tk,
            prev2_k);

        if (std::abs(coeffs[k]) >
            coeff_epsilon) {

            seal::Ciphertext term =
                multiply_plain_scalar_rescale(
                    tk,
                    coeffs[k],
                    encoder,
                    evaluator,
                    target_scale);

            if (result.parms_id() !=
                term.parms_id()) {

                evaluator.mod_switch_to_inplace(
                    result,
                    term.parms_id());
            }

            normalize_scale(
                result,
                target_scale);

            normalize_scale(
                term,
                target_scale);

            evaluator.add_inplace(
                result,
                term);
        }

        t_prev2 = std::move(t_prev1);
        t_prev1 = std::move(tk);
    }

    return result;
}


} // namespace chehab::dl
