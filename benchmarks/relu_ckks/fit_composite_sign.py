#!/usr/bin/env python3
"""Fit a composite minimax polynomial approximation of sign(x) for ReLU.

sign(x) is approximated on [-1, -eps] U [eps, 1] by p_k o ... o p_1, where each
p_i is an odd polynomial of degree d_i and the minimax (Remez) approximation of
1 on the interval that the previous stages map [eps, 1] onto: p_1 on [eps, 1],
p_i on [1 - E_{i-1}, 1 + E_{i-1}], E_i being p_i's minimax error. This stage-wise
construction is the optimal composite minimax approximation of Lee et al.
(IEEE TDSC 2021, "Minimax approximation of sign function by composite
polynomial for homomorphic comparison"). Odd polynomials make the negative
half follow by symmetry.

ReLU(x) on [-B, B] is then x * (1 + sign(x / B)) / 2. Exact for |x| >= eps * B
up to |x| * E_k / 2; for |x| < eps * B the error is at most |x| (1 + max p_k) / 2.

  python3 fit_composite_sign.py --degrees 7 7 7 --eps 0.0625 --bound 4 --emit relu_sign_coeffs.json
"""
import argparse
import json

import numpy as np

GRID = 200001


def odd_eval(coeffs, x):
    """sum_j coeffs[j] * x^(2j+1) by Horner in x^2 (elementwise, no BLAS)."""
    x2 = x * x
    acc = np.zeros_like(x) + coeffs[-1]
    for c in reversed(coeffs[:-1]):
        acc = acc * x2 + c
    return acc * x


def remez_odd_one(degree, a, b, iterations=100):
    """Minimax odd polynomial of `degree` approximating 1 on [a, b], 0 < a < b.

    Returns (coefficients of x^1, x^3, ..., x^degree; minimax error)."""
    k = (degree + 1) // 2
    powers = np.arange(1, degree + 1, 2)
    grid = np.linspace(a, b, GRID)
    # Chebyshev-like initial reference of k + 1 points.
    ref = np.sort((a + b) / 2 + (b - a) / 2 * np.cos(np.pi * np.arange(k + 1) / k))
    coeffs = err = None
    for _ in range(iterations):
        m = np.hstack([ref[:, None] ** powers[None, :], ((-1.0) ** np.arange(k + 1))[:, None]])
        sol = np.linalg.solve(m, np.ones(k + 1))
        coeffs, level = sol[:k], abs(sol[k])
        err = odd_eval(coeffs, grid) - 1.0
        # Local extrema of the error (endpoints included), one per sign run.
        interior = np.where((np.diff(np.sign(np.diff(err))) != 0))[0] + 1
        candidates = np.unique(np.concatenate([[0], interior, [GRID - 1]]))
        runs, run = [], [candidates[0]]
        for idx in candidates[1:]:
            if np.sign(err[idx]) == np.sign(err[run[-1]]):
                run.append(idx)
            else:
                runs.append(max(run, key=lambda i: abs(err[i])))
                run = [idx]
        runs.append(max(run, key=lambda i: abs(err[i])))
        while len(runs) > k + 1:  # drop the smaller end extremum
            runs.pop(0 if abs(err[runs[0]]) < abs(err[runs[-1]]) else -1)
        if len(runs) < k + 1:
            raise RuntimeError(f"Remez lost alternation (degree {degree} on [{a}, {b}])")
        ref = grid[runs]
        if np.max(np.abs(err)) - level <= 1e-12 * max(level, 1e-300) + 1e-15:
            break
    return coeffs, float(np.max(np.abs(err)))


def fit(degrees, eps):
    stages, lo, hi = [], eps, 1.0
    for degree in degrees:
        coeffs, error = remez_odd_one(degree, lo, hi)
        stages.append({"degree": degree, "interval": [lo, hi], "odd_coefficients": coeffs.tolist(),
                       "minimax_error": error})
        lo, hi = 1.0 - error, 1.0 + error
    return stages


def composite_sign(stages, s):
    for stage in stages:
        s = odd_eval(stage["odd_coefficients"], s)
    return s


def main():
    p = argparse.ArgumentParser()
    p.add_argument("--degrees", type=int, nargs="+", default=[7, 7, 7])
    p.add_argument("--eps", type=float, default=0.0625, help="sign is approximated on |s| in [eps, 1]")
    p.add_argument("--bound", type=float, default=4.0, help="ReLU input range [-bound, bound]")
    p.add_argument("--emit", metavar="JSON")
    args = p.parse_args()
    if any(d < 1 or d % 2 == 0 for d in args.degrees):
        p.error("stage degrees must be odd")

    stages = fit(args.degrees, args.eps)
    x = np.linspace(-args.bound, args.bound, 400001)
    relu = x * (1.0 + composite_sign(stages, x / args.bound)) / 2.0
    max_err = float(np.max(np.abs(relu - np.maximum(x, 0.0))))
    outer = np.abs(x) >= args.eps * args.bound
    max_err_outer = float(np.max(np.abs(relu - np.maximum(x, 0.0))[outer]))
    for i, st in enumerate(stages, 1):
        print(f"stage {i}: degree {st['degree']} on [{st['interval'][0]:.6g}, {st['interval'][1]:.6g}], "
              f"minimax error {st['minimax_error']:.4g}")
    print(f"ReLU on [-{args.bound}, {args.bound}]: max error {max_err:.4g}; "
          f"for |x| >= {args.eps * args.bound:g}: {max_err_outer:.4g}")
    if args.emit:
        with open(args.emit, "w") as f:
            json.dump({
                "generator": "fit_composite_sign.py " + " ".join(
                    ["--degrees", *map(str, args.degrees), "--eps", repr(args.eps), "--bound", repr(args.bound)]),
                "method": "stage-wise Remez minimax odd polynomials approximating sign (Lee et al., TDSC 2021); "
                          "ReLU(x) = x * (1 + sign(x / bound)) / 2",
                "numpy_version": np.__version__,
                "eps": args.eps, "bound": args.bound, "stages": stages,
                "relu_max_error_grid": max_err, "relu_max_error_outside_eps": max_err_outer,
                "grid_note": "max |relu_approx - relu| on 400001 uniform points of [-bound, bound]; "
                             "not a global bound",
            }, f, indent=2)
            f.write("\n")


if __name__ == "__main__":
    main()
