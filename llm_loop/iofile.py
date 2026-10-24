"""Read and write the benchmark I/O file (fhe_io_example.txt), CKKS layout:

  <slot_count> <nb_inputs> <nb_outputs>
  <label> <is_cipher> <is_signed> <v_0> ... <v_{n-1}>     (inputs; one value = scalar plaintext)
  <label> <is_cipher> <v_0> ... <v_{n-1}>                 (outputs)
"""
from dataclasses import dataclass, field
from pathlib import Path
from typing import Dict, List

import numpy as np


@dataclass
class IoExample:
    slot_count: int
    # label -> (is_cipher, values); a single value is a scalar plaintext.
    inputs: Dict[str, tuple] = field(default_factory=dict)
    outputs: Dict[str, np.ndarray] = field(default_factory=dict)

    def value(self, label: str) -> np.ndarray:
        """Input values broadcast to slot_count (scalars become constant vectors)."""
        values = self.inputs[label][1]
        return np.full(self.slot_count, values[0]) if len(values) == 1 else np.asarray(values, dtype=float)

    def scalar(self, label: str) -> float:
        values = self.inputs[label][1]
        if not np.allclose(values, values[0]):
            raise ValueError(f"input {label} is not constant across slots")
        return float(values[0])


def read(path: Path) -> IoExample:
    lines = Path(path).read_text().split("\n")
    slot_count, nb_inputs, nb_outputs = map(int, lines[0].split())
    io = IoExample(slot_count)
    for line in lines[1:1 + nb_inputs]:
        tokens = line.split()
        io.inputs[tokens[0]] = (int(tokens[1]), np.array([float(v) for v in tokens[3:]]))
    for line in lines[1 + nb_inputs:1 + nb_inputs + nb_outputs]:
        tokens = line.split()
        io.outputs[tokens[0]] = np.array([float(v) for v in tokens[2:]])
    return io


def _fmt(values) -> str:
    return " ".join(f"{float(v):.17g}" for v in values)


def write(io: IoExample, path: Path) -> None:
    lines: List[str] = [f"{io.slot_count} {len(io.inputs)} {len(io.outputs)}"]
    for label, (is_cipher, values) in io.inputs.items():
        lines.append(f"{label} {is_cipher} 1 {_fmt(values)}")
    for label, values in io.outputs.items():
        lines.append(f"{label} 1 {_fmt(values)}")
    Path(path).write_text("\n".join(lines) + "\n")
