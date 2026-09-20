#!/usr/bin/env python3
"""Generate Chebyshev-basis coefficients for FHE-friendly activations.

Example:
  python3 generate_chebyshev_coeffs.py --activation sigmoid --degree 5 --min -4 --max 4
"""

import argparse
import json
import math
import numpy as np


def activation(name: str, x: np.ndarray) -> np.ndarray:
    if name == "sigmoid":
        return 1.0 / (1.0 + np.exp(-x))
    if name == "silu":
        return x / (1.0 + np.exp(-x))
    if name == "gelu":
        # Exact GELU via erf, vectorized without requiring scipy.
        erf = np.vectorize(math.erf)
        return 0.5 * x * (1.0 + erf(x / math.sqrt(2.0)))
    if name == "elu":
        return np.where(x > 0.0, x, np.expm1(x))
    if name == "selu":
        alpha = 1.6732632423543772
        lam = 1.0507009873554805
        return lam * np.where(x > 0.0, x, alpha * np.expm1(x))
    if name == "softplus":
        return np.logaddexp(0.0, x)
    if name == "mish":
        sp = np.logaddexp(0.0, x)
        return x * np.tanh(sp)
    if name == "hardshrink":
        lambd = 0.5
        return np.where((x >= -lambd) & (x <= lambd), 0.0, x)
    raise ValueError(f"unsupported activation: {name}")


def fit(name: str, degree: int, lo: float, hi: float, samples: int):
    if not hi > lo:
        raise ValueError("--max must exceed --min")
    if degree < 1:
        raise ValueError("degree must be >= 1")
    x = np.linspace(lo, hi, samples, dtype=np.float64)
    z = 2.0 * (x - lo) / (hi - lo) - 1.0
    y = activation(name, x)
    coeffs = np.polynomial.chebyshev.chebfit(z, y, degree)
    approx = np.polynomial.chebyshev.chebval(z, coeffs)
    return coeffs, float(np.max(np.abs(approx - y)))


def main():
    p = argparse.ArgumentParser()
    p.add_argument("--activation", required=True,
                   choices=["gelu", "silu", "sigmoid", "elu", "selu",
                            "softplus", "mish", "hardshrink"])
    p.add_argument("--degree", type=int, default=5)
    p.add_argument("--min", dest="lo", type=float, default=-4.0)
    p.add_argument("--max", dest="hi", type=float, default=4.0)
    p.add_argument("--samples", type=int, default=4097)
    p.add_argument("--json", action="store_true")
    args = p.parse_args()

    coeffs, error = fit(args.activation, args.degree, args.lo, args.hi, args.samples)
    if args.json:
        print(json.dumps({
            "activation": args.activation,
            "degree": args.degree,
            "range": [args.lo, args.hi],
            "coefficients": coeffs.tolist(),
            "max_fit_error": error,
        }, indent=2))
    else:
        print(f"// {args.activation}, degree={args.degree}, range=[{args.lo}, {args.hi}]")
        print("std::vector<double> coeffs{")
        for c in coeffs:
            print(f"    {c:.17e},")
        print("};")
        print(f"// max fit error on calibration grid: {error:.17e}")


if __name__ == "__main__":
    main()
