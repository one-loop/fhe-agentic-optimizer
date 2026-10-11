"""Write fhe_io_example.txt for the elementwise_ckks benchmark.

x0 is encrypted. The second operand is encrypted (x1; ops add, mul), a
plaintext slot vector (p; add_plain, mul_plain) or a plaintext scalar (s, a
single value; add_scalar, mul_scalar). The expected output is computed
independently in numpy. Values are written with 17 significant digits.
"""
import argparse

import numpy as np

OPS = ["add", "add_plain", "add_scalar", "mul", "mul_plain", "mul_scalar"]

parser = argparse.ArgumentParser(description="Get io_file generation parameters")
parser.add_argument("--op", choices=OPS, required=True)
parser.add_argument("--slot_count", type=int, default=8, help="function slot count")
parser.add_argument("--min", dest="lo", type=float, default=-2.0)
parser.add_argument("--max", dest="hi", type=float, default=2.0)
parser.add_argument("--seed", type=int, default=0)
args = parser.parse_args()

rng = np.random.default_rng(args.seed)
x0 = rng.uniform(args.lo, args.hi, args.slot_count)
if args.op.endswith("scalar"):
    label, is_cipher, operand = "s", 0, rng.uniform(args.lo, args.hi, 1)
    broadcast = np.full(args.slot_count, operand[0])
else:
    label, is_cipher = ("x1", 1) if args.op in ("add", "mul") else ("p", 0)
    operand = broadcast = rng.uniform(args.lo, args.hi, args.slot_count)
y = x0 + broadcast if args.op.startswith("add") else x0 * broadcast


def fmt(values):
    return " ".join(f"{v:.17g}" for v in values)


is_signed = 1
with open("fhe_io_example.txt", "w") as file:
    file.write(f"{args.slot_count} 2 1\n")
    file.write(f"x0 1 {is_signed} {fmt(x0)}\n")
    file.write(f"{label} {is_cipher} {is_signed} {fmt(operand)}\n")
    file.write(f"y 1 {fmt(y)}\n")
