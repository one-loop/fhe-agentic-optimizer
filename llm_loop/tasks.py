"""The 26 optimization tasks: every CKKS benchmark that run_benchmarks.py runs.

Each task has
  - the benchmark's own input generators for the "fixed" and "dense" cases
    (the same commands run_benchmarks.py uses), so a kernel is checked on the
    exact inputs the Phase 1 baseline was measured on;
  - an independent NumPy oracle that computes the expected outputs from the
    inputs, used for the held-out check;
  - a held-out input randomizer: fresh random ciphertext data (and fresh tables
    or BatchNorm vectors) with the dense case's slot count. Public structure
    stays as defined by the operator (Chebyshev/sign coefficients, masks), so a
    kernel cannot pass by exploiting coincidences of the committed inputs, such
    as table columns that are multiples of each other.

Tolerances follow each operator's owner: 1e-6 absolute, 1e-4 for the rotation
and reduction operators (extract, table_mult, embedding).
"""
import json
from dataclasses import dataclass, field
from pathlib import Path
from typing import Callable, Dict, List, Optional

import numpy as np

from llm_loop.iofile import IoExample

REPO = Path(__file__).resolve().parent.parent
SRC_BENCH = REPO / "benchmarks"
BUILD_BENCH = REPO / "build" / "benchmarks"
BASELINES = Path(__file__).resolve().parent / "baselines"
COEFFS = SRC_BENCH / "dl_operators_ckks" / "chebyshev_coeffs.json"

EPS = 1e-14  # coefficients at or below it get no term (same threshold as the benchmarks)
TOL = 1e-6
TOL_ROTATIONS = 1e-4
HELDOUT_SEEDS = (101, 202)


@dataclass
class Case:
    name: str
    command: List[str]  # writes fhe_io_example.txt into the working directory
    slot_count: Optional[int] = None  # None: first line of the command's stdout


@dataclass
class Task:
    name: str
    family: str
    owner: str
    record: str  # run_benchmarks.py benchmark name, for checking the baseline
    tolerance: float
    cases: List[Case]
    oracle: Callable[[IoExample], Dict[str, np.ndarray]]
    randomize: Callable[[IoExample, np.random.Generator], None]  # in place, on a copy of the dense case
    description: str = ""
    expected_levels: Optional[int] = None

    @property
    def baseline_path(self) -> Path:
        return BASELINES / f"{self.name}.cpp"

    def baseline(self) -> str:
        return self.baseline_path.read_text()

    def heldout(self, dense: IoExample, seed: int) -> IoExample:
        """Fresh inputs with the dense case's layout, expected outputs from the oracle."""
        io = IoExample(dense.slot_count, {k: (c, v.copy()) for k, (c, v) in dense.inputs.items()})
        self.randomize(io, np.random.default_rng(seed))
        io.outputs = self.oracle(io)
        return io


def _set(io: IoExample, label: str, values) -> None:
    is_cipher = io.inputs[label][0]
    io.inputs[label] = (is_cipher, np.asarray(values, dtype=float))


def _uniform(lo: float, hi: float, *labels: str):
    def randomize(io: IoExample, rng: np.random.Generator) -> None:
        for label in labels:
            _set(io, label, rng.uniform(lo, hi, len(io.inputs[label][1])))
    return randomize


# ---------------------------------------------------------------------------
# Chebyshev activations (benchmarks/chebyshev_ckks)
# ---------------------------------------------------------------------------
def coefficient_set(activation: str, degree: int) -> dict:
    for s in json.loads(COEFFS.read_text())["sets"]:
        if s["activation"] == activation and s["degree"] == degree:
            return s
    raise KeyError(f"no coefficient set for {activation} degree {degree}")


def chebyshev_terms(activation: str, degree: int) -> List[int]:
    return [k for k, c in enumerate(coefficient_set(activation, degree)["coefficients"]) if abs(c) > EPS]


def _chebyshev_oracle(activation: str, degree: int):
    terms = chebyshev_terms(activation, degree)
    lo, hi = coefficient_set(activation, degree)["range"]
    beta = -(hi + lo) / (hi - lo)

    def oracle(io: IoExample) -> Dict[str, np.ndarray]:
        z = io.value("x") * io.value("alpha")
        if abs(beta) > EPS:
            z = z + io.value("beta")
        t = [np.ones_like(z), z]
        for k in range(2, degree + 1):
            t.append(2 * z * t[k - 1] - t[k - 2])
        y = sum(io.value(f"c{k}") * t[k] for k in terms if k >= 1)
        if 0 in terms:
            y = y + io.value("c0")
        return {"y": y}
    return oracle


def _chebyshev(activation: str, degree: int, owner: str) -> Task:
    terms = chebyshev_terms(activation, degree)
    return Task(
        name=f"chebyshev_{activation}_d{degree}", family="chebyshev", owner=owner,
        record=f"chebyshev_{activation}_d{degree}_ckks", tolerance=TOL,
        cases=[Case(case, [str(BUILD_BENCH / "chebyshev_ckks" / "generate_chebyshev_ckks"), "p", points,
                           activation, str(degree)])
               for case, points in (("fixed", "points9"), ("dense", "dense"))],
        oracle=_chebyshev_oracle(activation, degree),
        randomize=_uniform(-4.0, 4.0, "x"),
        description=(
            f"Degree-{degree} Chebyshev approximation of {activation} on [-4, 4]. Inputs: ciphertext `x` in [-4, 4]; "
            "plaintexts `alpha` (= 0.25, so z = x * alpha is in [-1, 1]), `one` (= 1) and the coefficients "
            f"c0..c{degree} (same value in every slot). Output `y` = sum_k c_k T_k(z) over the non-negligible "
            f"terms {terms}, with T0 = 1, T1 = z, T_k = 2 z T_(k-1) - T_(k-2)."),
    )


# ---------------------------------------------------------------------------
# Composite ReLU (benchmarks/relu_ckks)
# ---------------------------------------------------------------------------
def _relu_oracle(io: IoExample) -> Dict[str, np.ndarray]:
    x = io.value("x")
    t = x
    for stage in (1, 2, 3):
        t = sum(io.value(f"s{stage}c{k}") * t ** k for k in (1, 3, 5, 7) if f"s{stage}c{k}" in io.inputs)
    return {"y": x * (t + io.value("half"))}


# ---------------------------------------------------------------------------
# Extraction and embedding (benchmarks/extract_embedding_ckks)
# ---------------------------------------------------------------------------
def _extract_oracle(io: IoExample) -> Dict[str, np.ndarray]:
    n = io.slot_count
    return {"output": np.roll(io.value("input") * io.value("mask"), -(n // 4))}


def sparse_layout(slot_count: int):
    """(num_dense, logical_size, output_count). The fixed case (8 slots) is the
    benchmark's extract_sparse_boundary variant, every larger case extract_sparse."""
    num_dense = slot_count // 4
    logical = slot_count + num_dense if slot_count <= 8 else slot_count + slot_count // 2
    vocab = logical - num_dense
    return num_dense, logical, (vocab + slot_count - 1) // slot_count


def _sparse_oracle(io: IoExample) -> Dict[str, np.ndarray]:
    n = io.slot_count
    num_dense, logical, out_count = sparse_layout(n)
    values = np.concatenate([io.value("first"), io.value("second")])[:logical]
    expected = np.pad(values[num_dense:], (0, out_count * n - (logical - num_dense)))
    return {f"output_{i}": expected[i * n:(i + 1) * n] for i in range(out_count)}


def _sparse_randomize(io: IoExample, rng: np.random.Generator) -> None:
    n = io.slot_count
    _, logical, _ = sparse_layout(n)
    values = np.pad(rng.uniform(-2.0, 2.0, logical), (0, 2 * n - logical))  # chunks are zero-padded
    _set(io, "first", values[:n])
    _set(io, "second", values[n:])


def _table_oracle(io: IoExample) -> Dict[str, np.ndarray]:
    n = io.slot_count
    return {f"output_{i}": np.full(n, float(np.dot(io.value("indicator"), io.value(f"table_{i}"))))
            for i in range(3)}


def _embedding_oracle(io: IoExample) -> Dict[str, np.ndarray]:
    out = np.zeros(io.slot_count)
    for i in range(3):
        out[i] = float(np.dot(io.value("indicator"), io.value(f"table_{i}")))
    return {"output": out}


def _tables_randomize(one_hot: bool):
    def randomize(io: IoExample, rng: np.random.Generator) -> None:
        n = io.slot_count
        if one_hot:
            indicator = np.zeros(n)
            indicator[rng.integers(n)] = 1.0
        else:
            indicator = rng.uniform(-1.0, 1.0, n)
        _set(io, "indicator", indicator)
        for i in range(3):  # independent columns, not multiples of each other
            _set(io, f"table_{i}", rng.uniform(-float(n), float(n), n))
    return randomize


def _extract_cases(op: str) -> List[Case]:
    script = str(SRC_BENCH / "extract_embedding_ckks" / "generate_extract_embedding_ckks.py")
    fixed_op = "extract_sparse_boundary" if op == "extract_sparse" else op
    return [Case("fixed", ["python3", script, "--op", fixed_op, "--slot_count", "8", "--seed", "0"], 8),
            Case("dense", ["python3", script, "--op", op, "--slot_count", "256", "--seed", "1"], 256)]


# ---------------------------------------------------------------------------
# Elementwise add / multiply (benchmarks/elementwise_ckks)
# ---------------------------------------------------------------------------
def _elementwise(op: str) -> Task:
    script = str(SRC_BENCH / "elementwise_ckks" / "generate_elementwise_ckks.py")
    operand = "x1" if op in ("add", "mul") else ("s" if "scalar" in op else "p")
    is_add = op.startswith("add")

    def oracle(io: IoExample) -> Dict[str, np.ndarray]:
        a, b = io.value("x0"), io.value(operand)
        return {"y": a + b if is_add else a * b}

    return Task(
        name=f"elementwise_{op}", family="elementwise", owner="Moonis", record=f"{op}_ckks", tolerance=TOL,
        cases=[Case(case, ["python3", script, "--op", op, "--slot_count", str(slot), "--seed", str(seed)], slot)
               for case, slot, seed in (("fixed", 8, 0), ("dense", 256, 1))],
        oracle=oracle, randomize=_uniform(-2.0, 2.0, "x0", operand),
        description=f"Elementwise {op.replace('_', ' ')}: y = x0 {'+' if is_add else '*'} {operand}.",
        expected_levels=0 if is_add else 1,
    )


# ---------------------------------------------------------------------------
TASKS: Dict[str, Task] = {}

for _act in ["gelu", "silu", "sigmoid", "elu", "selu", "softplus", "mish", "hardshrink"]:
    TASKS[f"chebyshev_{_act}_d7"] = _chebyshev(_act, 7, "Umair")
TASKS["chebyshev_sigmoid_d5"] = _chebyshev("sigmoid", 5, "Umair")
TASKS["chebyshev_relu_d7"] = _chebyshev("relu", 7, "Moonis")
TASKS["chebyshev_relu_d2"] = _chebyshev("relu", 2, "Moonis")

TASKS["relu_composite"] = Task(
    name="relu_composite", family="relu_composite", owner="Moonis", record="relu_ckks", tolerance=TOL,
    cases=[Case(case, ["python3", str(SRC_BENCH / "relu_ckks" / "generate_relu_ckks.py"), points])
           for case, points in (("fixed", "points9"), ("dense", "dense"))],
    oracle=_relu_oracle, randomize=_uniform(-4.0, 4.0, "x"),
    description=("ReLU on [-4, 4] by a composite odd minimax approximation of sign: t0 = x, "
                 "t_i = sum_k s<i>c<k> t_(i-1)^k (k = 1, 3, 5, 7; i = 1..3), y = x * (t_3 + half)."),
    expected_levels=10,
)

TASKS["quad"] = Task(
    name="quad", family="quad", owner="Umair", record="quad_ckks", tolerance=TOL,
    cases=[Case(case, ["python3", str(SRC_BENCH / "quad_ckks" / "generate_quad_ckks.py"), "--slot_count", str(slot),
                       "--seed", str(seed)], slot)
           for case, slot, seed in (("fixed", 8, 0), ("dense", 256, 1))],
    oracle=lambda io: {"y": io.value("x") ** 2}, randomize=_uniform(-2.0, 2.0, "x"),
    description="Quadratic activation y = x * x.", expected_levels=1,
)

for _name, _fixed, _dense in (("batchnorm1d", "bn1d", "bn1d_rand"), ("batchnorm2d", "bn2d", "bn2d_rand")):
    def _bn_randomize(io: IoExample, rng: np.random.Generator) -> None:
        _set(io, "x", rng.uniform(-3.0, 3.0, len(io.inputs["x"][1])))
        _set(io, "A", rng.uniform(0.3, 2.0, len(io.inputs["A"][1])))
        _set(io, "B", rng.uniform(-1.0, 1.0, len(io.inputs["B"][1])))

    TASKS[_name] = Task(
        name=_name, family="batchnorm", owner="Umair", record=f"{_name}_ckks", tolerance=TOL,
        cases=[Case(case, [str(BUILD_BENCH / "batchnorm_ckks" / "generate_batchnorm_ckks"), gen])
               for case, gen in (("fixed", _fixed), ("dense", _dense))],
        oracle=lambda io: {"y": io.value("x") * io.value("A") + io.value("B")}, randomize=_bn_randomize,
        description=f"Inference BatchNorm ({_name[-2:]}) folded offline: y = x * A + B, A and B plaintext vectors.",
        expected_levels=1,
    )

for _op in ["add", "add_plain", "add_scalar", "mul", "mul_plain", "mul_scalar"]:
    TASKS[f"elementwise_{_op}"] = _elementwise(_op)

for _op, _desc in (
        ("extract", "output = (input * mask) rotated left by slots/4 (mask = 1 on slots [n/4, 3n/4))."),
        ("extract_linear", "Same computation as extract, with the mask used as a projection diagonal.")):
    TASKS[_op] = Task(
        name=_op, family="extract", owner="mabyan22", record=f"{_op}_ckks", tolerance=TOL_ROTATIONS,
        cases=_extract_cases(_op), oracle=_extract_oracle, randomize=_uniform(-2.0, 2.0, "input"),
        description=_desc, expected_levels=1)

TASKS["extract_sparse"] = Task(
    name="extract_sparse", family="extract", owner="mabyan22", record="extract_sparse_ckks",
    tolerance=TOL_ROTATIONS, cases=_extract_cases("extract_sparse"), oracle=_sparse_oracle,
    randomize=_sparse_randomize, expected_levels=1,
    description=("Sparse extraction over two zero-padded ciphertext chunks `first`, `second`: drop the first "
                 "slots/4 logical values and shift the rest to the front of output_0 (and output_1 in the dense "
                 "case). Masks: keep_front (1 on the first slots/4 slots) and drop_front (1 - keep_front)."))

TASKS["table_mult"] = Task(
    name="table_mult", family="reduction", owner="mabyan22", record="table_mult_ckks", tolerance=TOL_ROTATIONS,
    cases=_extract_cases("table_mult"), oracle=_table_oracle, randomize=_tables_randomize(one_hot=False),
    expected_levels=1,
    description="output_i = sum over all slots of indicator * table_i, in every slot, for table columns i = 0, 1, 2.")

TASKS["embedding"] = Task(
    name="embedding", family="reduction", owner="mabyan22", record="embedding_ckks", tolerance=TOL_ROTATIONS,
    cases=_extract_cases("embedding"), oracle=_embedding_oracle, randomize=_tables_randomize(one_hot=True),
    expected_levels=2,
    description=("Embedding lookup: output[i] = sum over slots j of indicator[j] * table_i[j] for i = 0, 1, 2, "
                 "and 0 in every other slot; first_slot = (1, 0, ..., 0)."))

assert len(TASKS) == 26, len(TASKS)
