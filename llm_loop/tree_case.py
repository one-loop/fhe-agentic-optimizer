#!/usr/bin/env python3
"""Write fhe_io_example.txt (into the working directory) for one random expression tree task:
every input a random integer in [-64, 64] from the given seed, the expected result from the task's oracle.

  python3 llm_loop/tree_case.py tree_50-50_5_1 1
"""
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent.parent))

import numpy as np  # noqa: E402

from llm_loop import iofile  # noqa: E402
from llm_loop.tasks import BFV_INPUT_BOUND, TASKS, TREES_DIR, parse_tree, tree_inputs  # noqa: E402


def main() -> None:
    name, seed = sys.argv[1], int(sys.argv[2])
    task = TASKS[name]
    rng = np.random.default_rng(seed)
    labels = sorted(set(tree_inputs(parse_tree((TREES_DIR / f"{name}.txt").read_text()))), key=lambda s: int(s[1:]))
    io = iofile.IoExample(1, {label: (1, np.array([float(rng.integers(-BFV_INPUT_BOUND, BFV_INPUT_BOUND + 1))]))
                              for label in labels})
    io.outputs = task.oracle(io)
    iofile.write(io, Path("fhe_io_example.txt"))
    print(1)


if __name__ == "__main__":
    main()
