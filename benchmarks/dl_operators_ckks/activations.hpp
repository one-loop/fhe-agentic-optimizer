#pragma once

#include <algorithm>
#include <cmath>
#include <stdexcept>
#include <string>

namespace chehab::dl {

// Plaintext reference activations, matching activation() in
// generate_chebyshev_coeffs.py. Used to measure polynomial approximation
// error (true activation vs fitted polynomial), never FHE error.
inline double activation_reference(const std::string& name, double x) {
    if (name == "sigmoid") {
        return 1.0 / (1.0 + std::exp(-x));
    }
    if (name == "silu") {
        return x / (1.0 + std::exp(-x));
    }
    if (name == "gelu") {
        return 0.5 * x * (1.0 + std::erf(x / std::sqrt(2.0)));
    }
    if (name == "elu") {
        return x > 0.0 ? x : std::expm1(x);
    }
    if (name == "selu") {
        constexpr double alpha = 1.6732632423543772;
        constexpr double lam = 1.0507009873554805;
        return lam * (x > 0.0 ? x : alpha * std::expm1(x));
    }
    if (name == "softplus") {
        // log(1 + e^x), computed stably like numpy.logaddexp(0, x).
        return std::max(x, 0.0) + std::log1p(std::exp(-std::abs(x)));
    }
    if (name == "mish") {
        const double sp = std::max(x, 0.0) + std::log1p(std::exp(-std::abs(x)));
        return x * std::tanh(sp);
    }
    if (name == "hardshrink") {
        constexpr double lambd = 0.5;
        return (x >= -lambd && x <= lambd) ? 0.0 : x;
    }
    throw std::invalid_argument("unsupported activation: " + name);
}

} // namespace chehab::dl
