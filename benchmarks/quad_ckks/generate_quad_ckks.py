"""Write fhe_io_example.txt for the quad_ckks benchmark.

Inputs are random doubles; the expected output is computed independently in
numpy as x**2. Values are written with 17 significant digits.
"""
import argparse

import numpy as np

parser = argparse.ArgumentParser(description="Get io_file generation parameters")
parser.add_argument("--slot_count", type=int, default=8, help="function slot count")
parser.add_argument("--min", dest="lo", type=float, default=-2.0)
parser.add_argument("--max", dest="hi", type=float, default=2.0)
parser.add_argument("--seed", type=int, default=0)
args = parser.parse_args()

rng = np.random.default_rng(args.seed)
x = rng.uniform(args.lo, args.hi, args.slot_count)
y = x**2


def fmt(values):
    return " ".join(f"{v:.17g}" for v in values)


is_cipher = 1
is_signed = 1
with open("fhe_io_example.txt", "w") as file:
    file.write(f"{args.slot_count} 1 1\n")
    file.write(f"x {is_cipher} {is_signed} {fmt(x)}\n")
    file.write(f"y {is_cipher} {fmt(y)}\n")
