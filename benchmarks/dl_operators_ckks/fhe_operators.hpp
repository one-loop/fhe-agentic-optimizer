#pragma once

#include <seal/seal.h>

#include <cstddef>
#include <string>
#include <vector>

namespace chehab::dl {

// CKKS scale convention used by every operator in this module:
//
// * Ciphertext scales are never overwritten with a nominal value. After a
//   rescale the scale is whatever SEAL computes (s / q_l), and decoding uses it.
// * A ciphertext-plaintext product that is followed by a rescale encodes the
//   plaintext at out_scale * q_l / ct.scale(), so the rescaled result lands on
//   out_scale exactly (q_l is the prime dropped by the rescale).
// * Ciphertext-ciphertext products keep their natural scale s_a * s_b / q_l.

struct BatchNormParams {
    std::vector<double> scale; // gamma / sqrt(running_var + eps)
    std::vector<double> shift; // beta - scale * running_mean
};

BatchNormParams precompute_batch_norm(
    const std::vector<double>& gamma,
    const std::vector<double>& beta,
    const std::vector<double>& running_mean,
    const std::vector<double>& running_var,
    double eps);

// Expand C channel parameters for flattened [N,C,L] data (NCL order).
std::vector<double> expand_batchnorm1d_channels(
    const std::vector<double>& per_channel,
    std::size_t batch,
    std::size_t channels,
    std::size_t length);

// Expand C channel parameters for flattened [N,C,H,W] data (NCHW order).
std::vector<double> expand_batchnorm2d_channels(
    const std::vector<double>& per_channel,
    std::size_t batch,
    std::size_t channels,
    std::size_t height,
    std::size_t width);

// Inference-time BatchNorm: y = scale * x + shift. The result has scale
// exactly output_scale and is one level below the input.
seal::Ciphertext batch_norm(
    const seal::Ciphertext& input,
    const std::vector<double>& expanded_scale,
    const std::vector<double>& expanded_shift,
    const seal::SEALContext& context,
    seal::CKKSEncoder& encoder,
    seal::Evaluator& evaluator,
    double output_scale);

// Exact square activation y = x^2. The result keeps its natural scale
// input.scale()^2 / q_l and is one level below the input.
seal::Ciphertext quad(
    const seal::Ciphertext& input,
    seal::Evaluator& evaluator,
    const seal::RelinKeys& relin_keys);

// Evaluate sum_k coeffs[k] * T_k(z), where z maps [range_min, range_max] to [-1,1].
// Coefficients must be Chebyshev-basis coefficients in NumPy convention.
// The result has scale exactly output_scale.
seal::Ciphertext chebyshev(
    const seal::Ciphertext& input,
    const std::vector<double>& coeffs,
    double range_min,
    double range_max,
    const seal::SEALContext& context,
    seal::CKKSEncoder& encoder,
    seal::Evaluator& evaluator,
    const seal::RelinKeys& relin_keys,
    double output_scale);

} // namespace chehab::dl
