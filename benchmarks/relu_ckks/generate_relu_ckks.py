"""Write fhe_io_example.txt for the relu_ckks benchmark.

  python3 generate_relu_ckks.py <points> [--coeffs relu_sign_coeffs.json]

points: points9  nine evenly spaced points over [-bound, bound]
        dense    256 seeded uniform points over [-bound, bound]

Inputs: ciphertext x; plaintext scalars (one value each) s<i>c<k>, the
coefficient of t^k in stage i, with 1/bound^k folded into stage 1 and 1/2 into
the last stage, and half = 0.5. The expected output is the same computation in
plaintext double arithmetic, x * (p_n'(... p_1'(x)) + 0.5), so comparing it with
the decrypted output measures FHE numerical error only. The approximation error
(vs the true ReLU) is printed separately.

Prints the slot count on the first line, then diagnostics, including the
levels the program must consume: sum over stages of ceil(log2(degree + 1)),
plus one for x * h.
"""
import argparse
import json
import math
from pathlib import Path

import numpy as np

parser = argparse.ArgumentParser()
parser.add_argument("points", choices=["points9", "dense"])
parser.add_argument("--coeffs", default=str(Path(__file__).resolve().parent / "relu_sign_coeffs.json"))
args = parser.parse_args()

fit = json.loads(Path(args.coeffs).read_text())
bound, stages = fit["bound"], fit["stages"]

if args.points == "points9":
    x = np.linspace(-bound, bound, 9)
else:
    x = np.random.default_rng(5).uniform(-bound, bound, 256)
n = len(x)

# Folded coefficients, as the encrypted program uses them.
folded = []
for i, stage in enumerate(stages, 1):
    coeffs = {}
    for j, c in enumerate(stage["odd_coefficients"]):
        k = 2 * j + 1
        if i == 1:
            c /= bound ** k
        if i == len(stages):
            c /= 2.0
        coeffs[k] = c
    folded.append(coeffs)

t = x.copy()
for coeffs in folded:
    t = sum(c * t ** k for k, c in coeffs.items())
y = x * (t + 0.5)

approx_error = float(np.max(np.abs(y - np.maximum(x, 0.0))))


def fmt(values):
    return " ".join(f"{v:.17g}" for v in values)


scalars = [(f"s{i}c{k}", c) for i, coeffs in enumerate(folded, 1) for k, c in coeffs.items()]
scalars.append(("half", 0.5))
with open("fhe_io_example.txt", "w") as f:
    f.write(f"{n} {len(scalars) + 1} 1\n")
    f.write(f"x 1 1 {fmt(x)}\n")
    for label, value in scalars:
        f.write(f"{label} 0 1 {value:.17g}\n")
    f.write(f"y 1 {fmt(y)}\n")

levels = sum(math.ceil(math.log2(s["degree"] + 1)) for s in stages) + 1
print(n)
print(f"relu via composite sign, stage degrees {[s['degree'] for s in stages]}, eps {fit['eps']}, "
      f"on [-{bound}, {bound}], {args.points} ({n} points)")
print(f"approximation error (plaintext polynomial vs relu) on these points: {approx_error:.6g}")
print(f"max_fit_error (fitting grid, relu_sign_coeffs.json): {fit['relu_max_error_grid']:.6g}")
print(f"reference levels: {levels}")
