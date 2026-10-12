"""Stronger check for the embedding task: random indicator and random table
columns (the benchmark's own cases use a one-hot indicator at slot 0 or 1 and
patterned columns, which a non-equivalent kernel could still pass).
Writes fhe_io_example.txt in the current directory.

  python3 gen_embedding_random.py <slot_count> <seed>
"""
import sys

import numpy as np

slot_count, seed = int(sys.argv[1]), int(sys.argv[2])
rng = np.random.default_rng(seed)
indicator = rng.uniform(-1.0, 1.0, slot_count)
columns = [rng.uniform(-1.0, 1.0, slot_count) for _ in range(3)]
expected = np.zeros(slot_count)
expected[:3] = [float(np.dot(indicator, c)) for c in columns]
first_slot = np.zeros(slot_count)
first_slot[0] = 1.0


def fmt(values):
    return " ".join(f"{v:.17g}" for v in values)


inputs = [("indicator", 1, indicator)] + [(f"table_{i}", 0, c) for i, c in enumerate(columns)]
inputs.append(("first_slot", 0, first_slot))
with open("fhe_io_example.txt", "w") as f:
    f.write(f"{slot_count} {len(inputs)} 1\n")
    for label, is_cipher, values in inputs:
        f.write(f"{label} {is_cipher} 1 {fmt(values)}\n")
    f.write(f"output 1 {fmt(expected)}\n")
print(slot_count)
