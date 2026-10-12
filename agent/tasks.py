"""Optimization tasks: a baseline CHEHAB DSL kernel plus the inputs that check it.

Every task reuses a repository CKKS benchmark. Its input generator (run in that
benchmark's build directory) writes the encrypted and plaintext inputs and the
independently computed expected outputs. Its baseline kernel reproduces the
benchmark's DSL program (agent/tasks/*.cpp, written by agent/tasks/gen_baselines.py).

Each task has two cases: "fixed" (few values) and "dense" (many values, also the
timing case). Some tasks add a third case. A candidate must be correct on every
case.
"""
import json
from dataclasses import dataclass, field
from pathlib import Path
from typing import List, Optional

AGENT = Path(__file__).resolve().parent
TASK_DIR = AGENT / "tasks"
COEFFS = AGENT.parent / "benchmarks" / "dl_operators_ckks" / "chebyshev_coeffs.json"
EPS = 1e-14

# The repository benchmarks compare with 1e-6, except the extraction and embedding
# operators, which the team runs at 1e-4: their errors are 1e-5 from the rotations.
TOL = 1e-6
TOL_ROTATIONS = 1e-4


@dataclass
class Case:
    name: str
    generator: List[str]  # run in build/benchmarks/<bench_dir>; writes fhe_io_example.txt
    slot_count: Optional[int] = None  # None: first line of the generator's stdout


@dataclass
class Task:
    name: str
    bench_dir: str
    description: str  # what the kernel computes and what its inputs mean (shown to the LLM)
    tolerance: float
    cases: List[Case] = field(default_factory=list)

    @property
    def baseline_path(self) -> Path:
        return TASK_DIR / f"{self.name}.cpp"

    def baseline(self) -> str:
        return self.baseline_path.read_text()


def _nonzero(activation: str, degree: int):
    for s in json.loads(COEFFS.read_text())["sets"]:
        if s["activation"] == activation and s["degree"] == degree:
            return [k for k, c in enumerate(s["coefficients"]) if abs(c) > EPS]
    raise KeyError(f"no coefficient set for {activation} degree {degree}")


def _chebyshev(activation: str, degree: int) -> Task:
    terms = _nonzero(activation, degree)
    evaluated = [f"c{k}" for k in terms if k >= 1]
    constant = " and c0" if 0 in terms else ""
    return Task(
        name=f"chebyshev_{activation}_d{degree}",
        bench_dir="chebyshev_ckks",
        description=(
            f"Degree-{degree} Chebyshev polynomial approximation of the {activation} activation on [-4, 4] "
            "(benchmarks/chebyshev_ckks).\n"
            "Inputs: ciphertext `x` (values in [-4, 4]); plaintext scalars: `alpha` = 0.25 (maps x to "
            "z = x/4 in [-1, 1]), `one` = 1.0, and c0 ... c"
            f"{degree}, the Chebyshev coefficients (each one is the same in every slot).\n"
            "Output: `y` = c0 + sum_k c_k T_k(z), with T0 = 1, T1 = z and T_k = 2 z T_{k-1} - T_{k-2}. "
            f"The coefficients {', '.join(evaluated)}{constant} are non-negligible; the rest are ~1e-16 and "
            "can be dropped.\n"
            "Any mathematically equivalent evaluation is allowed (another basis or evaluation order), as long "
            "as it only uses these inputs."
        ),
        tolerance=TOL,
        cases=[
            Case("fixed", ["./generate_chebyshev_ckks", "p", "points9", activation, str(degree)]),
            Case("dense", ["./generate_chebyshev_ckks", "p", "dense", activation, str(degree)]),
        ],
    )


def _elementwise(op: str, desc: str) -> Task:
    return Task(
        name=f"elementwise_{op}",
        bench_dir="elementwise_ckks",
        description=f"Elementwise {op.replace('_', ' ')} (benchmarks/elementwise_ckks). {desc}",
        tolerance=TOL,
        cases=[
            Case("fixed", ["python3", "generate_elementwise_ckks.py", "--op", op, "--slot_count", "8",
                           "--seed", "0"], slot_count=8),
            Case("dense", ["python3", "generate_elementwise_ckks.py", "--op", op, "--slot_count", "256",
                           "--seed", "1"], slot_count=256),
        ],
    )


def _extract(name: str, op: str, desc: str, fixed_op: Optional[str] = None, tol: float = TOL_ROTATIONS) -> Task:
    fixed_op = fixed_op or op
    return Task(
        name=name,
        bench_dir="extract_embedding_ckks",
        description=f"{desc} (benchmarks/extract_embedding_ckks).",
        tolerance=tol,
        cases=[
            Case("fixed", ["python3", "generate_extract_embedding_ckks.py", "--op", fixed_op, "--slot_count", "8",
                           "--seed", "0"], slot_count=8),
            Case("dense", ["python3", "generate_extract_embedding_ckks.py", "--op", op, "--slot_count", "256",
                           "--seed", "1"], slot_count=256),
        ],
    )


TASKS = {}
for _act, _deg in [("gelu", 7), ("silu", 7), ("sigmoid", 7), ("elu", 7), ("selu", 7), ("softplus", 7),
                   ("mish", 7), ("hardshrink", 7), ("relu", 7), ("sigmoid", 5), ("relu", 2)]:
    TASKS[f"chebyshev_{_act}_d{_deg}"] = _chebyshev(_act, _deg)

TASKS["relu_composite"] = Task(
    name="relu_composite",
    bench_dir="relu_ckks",
    description=(
        "ReLU on [-4, 4] through a composite minimax approximation of sign (benchmarks/relu_ckks): "
        "y = x * h, h = p3(p2(p1(x))) + 0.5, where p1, p2, p3 are odd degree-7 polynomials.\n"
        "Inputs: ciphertext `x` (values in [-4, 4]); plaintext scalars `s<i>c<k>` (i = 1..3, k = 1, 3, 5, 7), "
        "the coefficient of t^k in stage i, with 1/4^k already folded into stage 1 and 1/2 into stage 3; "
        "plaintext scalar `half` = 0.5.\n"
        "Output: `y`. Stage i computes t_i = sum_k s<i>c<k> * t_{i-1}^k with t_0 = x. Any mathematically "
        "equivalent evaluation is allowed, using only these inputs. The baseline already uses all 10 "
        "available levels."
    ),
    tolerance=TOL,
    cases=[
        Case("fixed", ["python3", "generate_relu_ckks.py", "points9"]),
        Case("dense", ["python3", "generate_relu_ckks.py", "dense"]),
    ],
)

TASKS["quad"] = Task(
    name="quad",
    bench_dir="quad_ckks",
    description=("Quadratic activation y = x * x (benchmarks/quad_ckks). Inputs: ciphertext `x`. "
                 "Output: `y`. Any equivalent evaluation is allowed."),
    tolerance=TOL,
    cases=[
        Case("fixed", ["python3", "generate_quad_ckks.py", "--slot_count", "8", "--seed", "0"], slot_count=8),
        Case("dense", ["python3", "generate_quad_ckks.py", "--slot_count", "256", "--seed", "1"], slot_count=256),
    ],
)

for _name, _case in [("batchnorm1d", ("bn1d", "bn1d_rand")), ("batchnorm2d", ("bn2d", "bn2d_rand"))]:
    TASKS[_name] = Task(
        name=_name,
        bench_dir="batchnorm_ckks",
        description=(
            f"Inference BatchNorm ({_name[-2:]}) with statistics folded offline (benchmarks/batchnorm_ckks): "
            "y = x * A + B.\nInputs: ciphertext `x`; plaintext vectors `A` and `B`, one value per slot "
            "giving each channel's scale and shift.\nOutput: `y`. Any equivalent evaluation is allowed."
        ),
        tolerance=TOL,
        cases=[
            Case("fixed", ["./generate_batchnorm_ckks", _case[0]]),
            Case("dense", ["./generate_batchnorm_ckks", _case[1]]),
        ],
    )

for _op in ["add", "add_plain", "add_scalar", "mul", "mul_plain", "mul_scalar"]:
    _desc = {
        "add": "Inputs: ciphertexts `x0`, `x1`. Output: `y` = x0 + x1 (depth 0).",
        "add_plain": "Inputs: ciphertext `x0`, plaintext vector `p`. Output: `y` = x0 + p (depth 0).",
        "add_scalar": "Inputs: ciphertext `x0`, plaintext scalar `s`. Output: `y` = x0 + s (depth 0).",
        "mul": "Inputs: ciphertexts `x0`, `x1`. Output: `y` = x0 * x1 (depth 1, with relinearization).",
        "mul_plain": "Inputs: ciphertext `x0`, plaintext vector `p`. Output: `y` = x0 * p (depth 1).",
        "mul_scalar": "Inputs: ciphertext `x0`, plaintext scalar `s`. Output: `y` = x0 * s (depth 1).",
    }[_op]
    TASKS[f"elementwise_{_op}"] = _elementwise(_op, _desc)

TASKS["extract"] = _extract(
    "extract", "extract",
    "Extraction of a window: output = (input * mask) rotated so the window starts at slot 0. The window is "
    "slots 1/4 to 3/4 of the input.\nInputs: ciphertext `input`; plaintext `mask` (1 inside the window, 0 "
    "outside). Output: `output`.")
TASKS["extract_linear"] = _extract(
    "extract_linear", "extract_linear",
    "Linear extraction (projection onto a window): the same computation as extract, with the mask used as a "
    "projection diagonal.\nInputs: ciphertext `input`; plaintext `mask`. Output: `output`.")
TASKS["extract_sparse"] = _extract(
    "extract_sparse", "extract_sparse",
    "Sparse extraction over two ciphertext chunks: the first num_dense = slots/4 slots of the logical input "
    "are dropped and the rest is shifted to the front of each output chunk.\nInputs: ciphertext chunks "
    "`first`, `second`; plaintext masks `keep_front` (1 for the first num_dense slots, else 0) and "
    "`drop_front` (1 - keep_front). Outputs: `output_0` (and `output_1` in the dense case).",
    fixed_op="extract_sparse_boundary")
TASKS["table_mult"] = _extract(
    "table_mult", "table_mult",
    "Table lookup with 3 table columns: output_i = sum over all slots of indicator * table_i, in every slot.\n"
    "Inputs: ciphertext `indicator`; plaintext vectors `table_0`, `table_1`, `table_2`. "
    "Outputs: `output_0`, `output_1`, `output_2`. Rotations are cyclic over slots().")

TASKS["embedding"] = Task(
    name="embedding",
    bench_dir="extract_embedding_ckks",
    description=(
        "Embedding-table lookup with 3 table columns (benchmarks/extract_embedding_ckks, op embedding).\n"
        "Inputs: ciphertext `indicator`, a one-hot vector over the slots() slots (the lookup index; treat it "
        "as an arbitrary encrypted vector, its position is secret); plaintext vectors `table_0`, `table_1`, "
        "`table_2` (table column i, one value per slot); plaintext `first_slot` = (1, 0, 0, ..., 0).\n"
        "Output: `output`, with output[i] = sum over all slots j of indicator[j] * table_i[j] for i = 0, 1, 2, "
        "and 0 in every other slot. The result must be exact for any indicator vector, not only one-hot ones. "
        "Rotations are cyclic over slots()."
    ),
    tolerance=TOL_ROTATIONS,
    cases=[
        Case("fixed", ["python3", "generate_extract_embedding_ckks.py", "--op", "embedding", "--slot_count", "8",
                       "--seed", "0"], slot_count=8),
        Case("dense", ["python3", "generate_extract_embedding_ckks.py", "--op", "embedding", "--slot_count", "256",
                       "--seed", "1"], slot_count=256),
        Case("random", ["python3", str(TASK_DIR / "gen_embedding_random.py"), "64", "7"], slot_count=64),
    ],
)
