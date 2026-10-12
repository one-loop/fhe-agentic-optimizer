"""Optimization tasks: a baseline CHEHAB DSL kernel plus the inputs that check it.

Every task reuses a repository benchmark: its input generator (run in that
benchmark's build directory) writes the encrypted/plaintext inputs and the
independently computed expected outputs, and its baseline kernel reproduces the
benchmark's DSL program. A candidate must pass both cases: "fixed" (a few
values) and "dense" (many values; also the timing case).
"""
from dataclasses import dataclass, field
from pathlib import Path
from typing import List, Optional

TASK_DIR = Path(__file__).resolve().parent / "tasks"


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


def _chebyshev(activation: str, nonzero: str) -> Task:
    return Task(
        name=f"chebyshev_{activation}_d7",
        bench_dir="chebyshev_ckks",
        description=(
            f"Degree-7 Chebyshev polynomial approximation of the {activation} activation on [-4, 4] "
            "(benchmarks/chebyshev_ckks).\n"
            "Inputs: ciphertext `x` (values in [-4, 4]); plaintext vectors, each holding one constant in "
            "every slot: `alpha` = 0.25 (maps x to z = x/4 in [-1, 1]), `beta` = 0 (unused), `one` = 1.0, "
            "and `c0` ... `c7`, the Chebyshev coefficients.\n"
            "Output: `y` = c0 + sum_k c_k T_k(z), where T_k are Chebyshev polynomials of the first kind "
            "(T0 = 1, T1 = z, T_{k} = 2 z T_{k-1} - T_{k-2}). "
            f"Only {nonzero} are non-negligible and need to be evaluated; the others are ~1e-16.\n"
            "Any mathematically equivalent evaluation is allowed (e.g. a different polynomial basis or "
            "evaluation order), as long as it only uses these inputs."
        ),
        tolerance=1e-6,
        cases=[
            Case("fixed", ["./generate_chebyshev_ckks", "p", "points9", activation, "7"]),
            Case("dense", ["./generate_chebyshev_ckks", "p", "dense", activation, "7"]),
        ],
    )


TASKS = {
    t.name: t for t in [
        _chebyshev("sigmoid", "c0, c1, c3, c5 and c7"),
        _chebyshev("gelu", "c0, c1, c2, c4 and c6"),
        Task(
            name="relu_composite",
            bench_dir="relu_ckks",
            description=(
                "ReLU on [-4, 4] through a composite minimax approximation of sign "
                "(benchmarks/relu_ckks): y = x * h, h = p3(p2(p1(x))) + 0.5, where p1, p2, p3 are odd "
                "degree-7 polynomials.\n"
                "Inputs: ciphertext `x` (values in [-4, 4]); plaintext scalars `s<i>c<k>` (i = 1..3, "
                "k = 1, 3, 5, 7), the coefficient of t^k in stage i, with 1/4^k already folded into stage 1 "
                "and 1/2 into stage 3; plaintext scalar `half` = 0.5.\n"
                "Output: `y`. Stage i computes t_i = sum_k s<i>c<k> * t_{i-1}^k with t_0 = x. Any "
                "mathematically equivalent evaluation is allowed, using only these inputs. The baseline "
                "already uses all 10 available levels."
            ),
            tolerance=1e-6,
            cases=[
                Case("fixed", ["python3", "generate_relu_ckks.py", "points9"]),
                Case("dense", ["python3", "generate_relu_ckks.py", "dense"]),
            ],
        ),
        Task(
            name="embedding",
            bench_dir="extract_embedding_ckks",
            description=(
                "Embedding-table lookup with 3 table columns (benchmarks/extract_embedding_ckks, op "
                "embedding).\n"
                "Inputs: ciphertext `indicator`, a one-hot vector over the slots() slots (the lookup index; "
                "treat it as an arbitrary encrypted vector, its position is secret); plaintext vectors "
                "`table_0`, `table_1`, `table_2` (table column i, one value per slot); plaintext "
                "`first_slot` = (1, 0, 0, ..., 0).\n"
                "Output: `output`, with output[i] = sum over all slots j of indicator[j] * table_i[j] for "
                "i = 0, 1, 2, and 0 in every other slot. The result must be exact for any indicator "
                "vector, not only one-hot ones. Rotations are cyclic over slots()."
            ),
            tolerance=1e-4,
            cases=[
                Case("fixed", ["python3", "generate_extract_embedding_ckks.py", "--op", "embedding",
                               "--slot_count", "8", "--seed", "0"], slot_count=8),
                Case("dense", ["python3", "generate_extract_embedding_ckks.py", "--op", "embedding",
                               "--slot_count", "256", "--seed", "1"], slot_count=256),
                Case("random", ["python3", str(TASK_DIR / "gen_embedding_random.py"), "64", "7"],
                     slot_count=64),
            ],
        ),
    ]
}
