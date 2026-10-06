"""Generate CKKS inputs and independent plaintext outputs for extraction and embedding."""
import argparse

import numpy as np

OPS = ["extract", "extract_linear", "extract_sparse", "extract_sparse_boundary",
       "table_mult", "embedding"]
parser = argparse.ArgumentParser()
parser.add_argument("--op", choices=OPS, required=True)
parser.add_argument("--slot_count", type=int, default=8)
parser.add_argument("--seed", type=int, default=0)
args = parser.parse_args()

if args.slot_count < 8 or args.slot_count & (args.slot_count - 1):
    parser.error("slot_count must be a power of two of at least 8")

rng = np.random.default_rng(args.seed)
slot_count = args.slot_count
inputs = []
outputs = []

if args.op in ("extract", "extract_linear"):
    values = rng.uniform(-2.0, 2.0, slot_count)
    offset, extracted_size = slot_count // 4, slot_count // 2
    mask = np.zeros(slot_count)
    mask[offset:offset + extracted_size] = 1.0
    expected = np.roll(values * mask, -offset)
    inputs = [("input", 1, values), ("mask", 0, mask)]
    outputs = [("output", expected)]
elif args.op in ("extract_sparse", "extract_sparse_boundary"):
    num_dense = slot_count // 4
    logical_size = (slot_count + num_dense if args.op == "extract_sparse_boundary"
                    else slot_count + slot_count // 2)
    values = rng.uniform(-2.0, 2.0, logical_size)
    padded = np.pad(values, (0, 2 * slot_count - logical_size))
    first, second = padded[:slot_count], padded[slot_count:]
    keep_front = np.zeros(slot_count)
    keep_front[:num_dense] = 1.0
    drop_front = 1.0 - keep_front
    expected = np.pad(values[num_dense:], (0, 2 * slot_count - (logical_size - num_dense)))
    inputs = [("first", 1, first), ("second", 1, second),
              ("keep_front", 0, keep_front), ("drop_front", 0, drop_front)]
    output_count = (logical_size - num_dense + slot_count - 1) // slot_count
    outputs = [(f"output_{i}", expected[i * slot_count:(i + 1) * slot_count])
               for i in range(output_count)]
else:
    columns = [np.arange(1, slot_count + 1, dtype=float) * factor for factor in (1, 2, -1)]
    if args.op == "embedding":
        indicator = np.zeros(slot_count)
        indicator[args.seed % slot_count] = 1.0
    else:
        indicator = rng.uniform(-1.0, 1.0, slot_count)
    coordinates = [float(np.dot(indicator, column)) for column in columns]
    inputs = [("indicator", 1, indicator)]
    inputs += [(f"table_{i}", 0, column) for i, column in enumerate(columns)]
    if args.op == "table_mult":
        outputs = [(f"output_{i}", np.full(slot_count, value))
                   for i, value in enumerate(coordinates)]
    else:
        first_slot = np.zeros(slot_count)
        first_slot[0] = 1.0
        inputs.append(("first_slot", 0, first_slot))
        expected = np.zeros(slot_count)
        expected[:len(coordinates)] = coordinates
        outputs = [("output", expected)]


def fmt(values):
    return " ".join(f"{value:.17g}" for value in values)


with open("fhe_io_example.txt", "w") as file:
    file.write(f"{slot_count} {len(inputs)} {len(outputs)}\n")
    for label, is_cipher, values in inputs:
        file.write(f"{label} {is_cipher} 1 {fmt(values)}\n")
    for label, values in outputs:
        file.write(f"{label} 1 {fmt(values)}\n")
