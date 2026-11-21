#!/usr/bin/env python3
"""Deterministic input files (fhe_io_example.txt) for the tasks added in Phase 0, BFV scalar layout:
every input is its own ciphertext holding one integer (header slot count 1, like benchmarks/nn_*).

  python3 llm_loop/gen_io.py matmul <n> <seed>        # a_i_j, b_i_j -> c_i_j (n x n matrices)
  python3 llm_loop/gen_io.py tree <tree file> <bound> <seed>  # Coyote polynomial tree: leaves -> result

Writes fhe_io_example.txt in the working directory and prints the task's width on stdout. Standalone (no
llm_loop imports): it runs as a case command inside the case directory. The benchmarks' own generators are
not used because they are unseeded (benchmarks/matrix_mul) or write fixed values (polynomials_coyote).
"""
import random
import sys
from pathlib import Path

MATMUL_BOUND = 64  # |c_i_j| <= 3 * 64 * 64, far below the 20-bit plaintext modulus / 2


def write(inputs, outputs) -> None:
    lines = [f"1 {len(inputs)} {len(outputs)}"]
    lines += [f"{label} 1 1 {value}" for label, value in inputs.items()]
    lines += [f"{label} 1 {value}" for label, value in outputs.items()]
    Path("fhe_io_example.txt").write_text("\n".join(lines) + "\n")


def matmul(n: int, seed: int) -> None:
    rng = random.Random(seed)
    a = [[rng.randint(-MATMUL_BOUND, MATMUL_BOUND) for _ in range(n)] for _ in range(n)]
    b = [[rng.randint(-MATMUL_BOUND, MATMUL_BOUND) for _ in range(n)] for _ in range(n)]
    inputs = {f"a_{i}_{j}": a[i][j] for i in range(n) for j in range(n)}
    inputs.update({f"b_{i}_{j}": b[i][j] for i in range(n) for j in range(n)})
    outputs = {f"c_{i}_{j}": sum(a[i][k] * b[k][j] for k in range(n)) for i in range(n) for j in range(n)}
    write(inputs, outputs)
    print(n)


def parse_tree(text: str):
    """Coyote tree syntax: ( op lhs rhs ) with op in {+, *}, leaves are variable names."""
    tokens = text.replace("(", " ( ").replace(")", " ) ").split()

    def node():
        token = tokens.pop(0)
        if token != "(":
            return token
        op, lhs, rhs = tokens.pop(0), node(), node()
        if tokens.pop(0) != ")":
            raise ValueError("malformed tree")
        return op, lhs, rhs

    tree = node()
    if tokens:
        raise ValueError("trailing tokens after the tree")
    return tree


def leaves(tree) -> list:
    if isinstance(tree, str):
        return [tree]
    return leaves(tree[1]) + leaves(tree[2])


def evaluate(tree, values) -> int:
    if isinstance(tree, str):
        return values[tree]
    op, lhs, rhs = tree
    a, b = evaluate(lhs, values), evaluate(rhs, values)
    return a + b if op == "+" else a * b


def nonzero(rng: random.Random, bound: int) -> int:
    """Uniform on [-bound, bound] without 0: a zero leaf would zero whole products and hide wrong kernels."""
    return rng.choice([v for v in range(-bound, bound + 1) if v])


def max_magnitude(tree, bound: int) -> int:
    if isinstance(tree, str):
        return bound
    op, lhs, rhs = tree
    a, b = max_magnitude(lhs, bound), max_magnitude(rhs, bound)
    return a + b if op == "+" else a * b


def tree_io(path: str, bound: int, seed: int) -> None:
    tree = parse_tree(Path(path).read_text())
    if max_magnitude(tree, bound) >= 2 ** 18:  # the plaintext modulus is a 20-bit prime; stay well below t / 2
        sys.exit(f"bound {bound} lets the result reach {max_magnitude(tree, bound)}: not exact modulo t")
    rng = random.Random(seed)
    inputs = {label: nonzero(rng, bound) for label in dict.fromkeys(leaves(tree))}
    write(inputs, {"result": evaluate(tree, inputs)})
    print(1)


if __name__ == "__main__":
    kind = sys.argv[1]
    if kind == "matmul":
        matmul(int(sys.argv[2]), int(sys.argv[3]))
    elif kind == "tree":
        tree_io(sys.argv[2], int(sys.argv[3]), int(sys.argv[4]))
    else:
        sys.exit(__doc__)
