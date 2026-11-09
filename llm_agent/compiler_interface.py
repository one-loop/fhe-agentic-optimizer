"""CHEHAB and Microsoft SEAL evaluation bridge for the LLM agent.

Every number this module reports comes from compiling a program with CHEHAB,
building the generated SEAL code, running it, and checking its decrypted output
against an independent reference. Nothing is estimated and nothing falls back to
default values: a step that fails is reported as failed.

BFV benchmarks
  * unoptimized: the benchmark's scalar program (``./<bench> 0 ...``). CHEHAB's
    generated main compares every output with the generator's expected values
    (``outputs_match``).
  * e-graph: CHEHAB's e-graph vectorizer (``./<bench> 1 <slots> 0 ...``).
  * candidates: an LLM S-expression compiled through the vectorized path
    (``./<bench> 1 <slots> 2 ...``).
  In vectorized mode CHEHAB writes placeholder zeros as expected outputs, so the
  harness checks the decrypted slots itself: the reference is the scalar
  expression CHEHAB emitted (``expression.txt``) evaluated mod t on the
  generator's inputs, and that reference is first cross-checked against the
  generator's expected outputs.

CKKS benchmarks
  * unoptimized: the benchmark program as written, compiled by CHEHAB.
  * e-graph: not applicable (CHEHAB's vectorizer is BFV-only).
  * candidates: compiled by ``benchmarks/ckks_candidate`` through the same CHEHAB
    CKKS pipeline. The generated main compares the decrypted result with the
    generator's expected outputs (max abs error <= tolerance).

Isolation: CHEHAB's benchmark binaries share ``build/benchmarks/{expression,
inputs,vectorized_code,candidate_vectorized_code}.txt``. A process-wide lock on
``build/benchmarks`` stops two harness runs from interleaving, and stale files
are deleted before every compile so an old program can never be measured.
"""

import fcntl
import os
import re
import shutil
import statistics
import subprocess
import time
from dataclasses import asdict, dataclass, field
from pathlib import Path
from typing import Any, Dict, List, Optional, Set, Tuple

from .cost_model import evaluate_cost_metrics
from .grammar import (
    ASTNode,
    extract_vec_lengths,
    parse_sexpr,
    tokenize,
    validate_expression,
)


# ---------------------------------------------------------------------------
# Results
# ---------------------------------------------------------------------------


@dataclass
class EvaluationResult:
    success: bool  # compiled, built, ran AND produced verified-correct outputs
    candidate_expr: str
    scheme: str = "BFV"
    stage_failed: Optional[str] = None
    error_message: Optional[str] = None
    # Timing (ms). BFV: median end-to-end time of the generated main
    # (parse + encode + encrypt + evaluate). CKKS: median eval-only time.
    execution_latency_ms: float = 0.0
    latency_samples_ms: List[float] = field(default_factory=list)
    compilation_time_ms: float = 0.0
    # Correctness
    correctness_checked: bool = False
    correct: bool = False
    correctness_detail: str = ""
    # Circuit metrics (from CHEHAB's Quantifier and the generated code)
    depth: int = 0
    multiplicative_depth: int = 0
    op_counts: Dict[str, int] = field(default_factory=dict)
    remaining_noise_budget: int = 0
    ckks_max_abs_error: Optional[float] = None
    ckks_levels_consumed: int = 0
    ckks_tolerance: Optional[float] = None
    static_cost: float = 0.0

    def to_dict(self) -> Dict[str, Any]:
        return asdict(self)

    def feedback_string(self) -> str:
        ops = ", ".join(f"{k}={v}" for k, v in sorted(self.op_counts.items()) if v)
        if self.success:
            lines = [
                "STATUS: SUCCESS (compiled by CHEHAB, executed with SEAL, decrypted output verified correct)",
                f"- Measured latency: {self.execution_latency_ms:.2f} ms (median of {len(self.latency_samples_ms)} runs)",
                f"- Depth (ops on longest path, multiplicative): ({self.depth}, {self.multiplicative_depth})",
                f"- Generated SEAL operations: {ops or 'none'}",
            ]
            if self.scheme == "CKKS":
                lines.append(
                    f"- Levels consumed: {self.ckks_levels_consumed}; max abs error {self.ckks_max_abs_error:.2e} "
                    f"(tolerance {self.ckks_tolerance:g})"
                )
            else:
                lines.append(f"- Remaining noise budget: {self.remaining_noise_budget} bits")
            return "\n".join(lines) + "\n"
        if self.stage_failed == "correctness":
            return (
                "STATUS: WRONG RESULT. The program compiled and ran, but its decrypted output does not match "
                "the benchmark's expected output, so it is rejected.\n"
                f"Details: {self.correctness_detail}\n"
                "Guidance: your expression is not equivalent to the original computation. Re-derive which value "
                "each output slot must hold and fix the packing/rotation (or algebra).\n"
            )
        return (
            f"STATUS: FAILED at stage '{self.stage_failed}'\n"
            f"Error details:\n{self.error_message}\n"
            "Guidance: analyze this error, fix the expression, and propose a corrected candidate.\n"
        )


class HarnessError(RuntimeError):
    """The benchmark itself could not be set up; no LLM numbers can be trusted."""


# ---------------------------------------------------------------------------
# Process-wide lock on build/benchmarks
# ---------------------------------------------------------------------------

_LOCK_FD = None


def acquire_build_lock(build_benchmarks_dir: Path):
    global _LOCK_FD
    if _LOCK_FD is not None:
        return
    build_benchmarks_dir.mkdir(parents=True, exist_ok=True)
    fd = open(build_benchmarks_dir / ".llm_harness.lock", "w")
    try:
        fcntl.flock(fd, fcntl.LOCK_EX | fcntl.LOCK_NB)
    except BlockingIOError:
        fd.close()
        raise HarnessError(
            f"Another harness run holds {build_benchmarks_dir}/.llm_harness.lock. CHEHAB's benchmark binaries "
            "share files in that directory, so concurrent runs would corrupt each other. Run suites one at a time."
        )
    fd.write(str(os.getpid()))
    fd.flush()
    _LOCK_FD = fd


# ---------------------------------------------------------------------------
# Helpers
# ---------------------------------------------------------------------------

OP_PATTERNS = {
    "mul": r"evaluator\.multiply\(",
    "mul_plain": r"evaluator\.multiply_plain\(",
    "square": r"evaluator\.square\(",
    "add": r"evaluator\.add\(",
    "add_plain": r"evaluator\.add_plain\(",
    "sub": r"evaluator\.sub\(",
    "sub_plain": r"evaluator\.sub_plain\(",
    "negate": r"evaluator\.negate\(",
    "relin": r"evaluator\.relinearize\(",
    "rotate": r"evaluator\.rotate_(?:rows|vector|columns)\(",
    "rescale": r"evaluator\.rescale_to_next\(",
    "mod_switch": r"evaluator\.mod_switch_to_next\(",
}


def count_ops(source: str) -> Dict[str, int]:
    return {name: len(re.findall(pat, source)) for name, pat in OP_PATTERNS.items()}


def run(cmd, cwd: Path, timeout: float, env=None) -> subprocess.CompletedProcess:
    return subprocess.run(
        cmd,
        cwd=str(cwd),
        shell=isinstance(cmd, str),
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE,
        universal_newlines=True,
        timeout=timeout,
        env=env,
    )


def tail(text: str, n: int = 900) -> str:
    text = (text or "").strip()
    return text if len(text) <= n else "..." + text[-n:]


def parse_depth(stdout: str) -> Tuple[int, int]:
    m = re.search(r"max:\s*\((\d+),\s*(\d+)\)", stdout)
    return (int(m.group(1)), int(m.group(2))) if m else (0, 0)


def parse_compile_time(stdout: str) -> float:
    m = re.search(r"Compile time\s*:\s*\n\s*([0-9.eE+-]+)\s*ms", stdout)
    return float(m.group(1)) if m else 0.0


def is_int_literal(tok: str) -> bool:
    return re.fullmatch(r"[+-]?\d+", tok) is not None


def is_num_literal(tok: str) -> bool:
    return re.fullmatch(r"[+-]?(\d+(\.\d*)?|\.\d+)([eE][+-]?\d+)?", tok) is not None


# --- io files ---------------------------------------------------------------


@dataclass
class IoFile:
    slot_count: int
    inputs: Dict[str, Tuple[bool, List[str]]]  # label -> (is_cipher, raw value tokens)
    input_order: List[str]
    outputs: Dict[str, List[str]]
    output_order: List[str]
    raw_input_lines: List[str]


def parse_io_file(path: Path) -> IoFile:
    lines = [l for l in path.read_text().splitlines() if l.strip()]
    slot_count, nb_in, nb_out = (int(t) for t in lines[0].split()[:3])
    inputs, order, raw = {}, [], []
    for line in lines[1 : 1 + nb_in]:
        toks = line.split()
        inputs[toks[0]] = (toks[1] == "1", toks[3:])
        order.append(toks[0])
        raw.append(line)
    outputs, out_order = {}, []
    for line in lines[1 + nb_in : 1 + nb_in + nb_out]:
        toks = line.split()
        outputs[toks[0]] = toks[2:]
        out_order.append(toks[0])
    return IoFile(slot_count, inputs, order, outputs, out_order, raw)


# --- BFV reference evaluation ----------------------------------------------


def eval_scalar_expr(node: ASTNode, values: Dict[str, int], t: int) -> int:
    if node.is_leaf:
        if is_int_literal(node.val):
            return int(node.val) % t
        if node.val not in values:
            raise HarnessError(f"reference expression uses unknown input '{node.val}'")
        return values[node.val] % t
    args = [eval_scalar_expr(a, values, t) for a in node.args]
    op = node.val
    if op in ("+", "Add"):
        return sum(args) % t
    if op in ("*", "Mul"):
        r = 1
        for a in args:
            r = (r * a) % t
        return r
    if op in ("-", "Minus"):
        return (-args[0]) % t if len(args) == 1 else (args[0] - sum(args[1:])) % t
    if op == "Neg":
        return (-args[0]) % t
    raise HarnessError(f"unsupported operator '{op}' in reference expression")


# --- CKKS candidate language -----------------------------------------------

CKKS_OPS = {"+", "-", "*", "let"}


def check_ckks_expression(ast: ASTNode, available: Set[str]) -> Optional[str]:
    """Static checks for the CKKS candidate language. Returns an error or None."""

    def walk(node: ASTNode, scope: Set[str]) -> Optional[str]:
        if node.is_leaf:
            if is_num_literal(node.val) or node.val in scope:
                return None
            return f"unknown variable '{node.val}'. Available inputs: {sorted(available)}"
        op = node.val
        if op not in CKKS_OPS:
            return (
                f"unknown operator '{op}'. CKKS candidates use only (+ a b ...), (- a b), (- a), (* a b ...) "
                "and (let name value body); there is no Vec/rotation (every operation already acts on all slots)."
            )
        if op == "let":
            if len(node.args) != 3 or not node.args[0].is_leaf or is_num_literal(node.args[0].val):
                return "let must be written (let name value body) with a symbol as name"
            name = node.args[0].val
            if name in available:
                return f"let name '{name}' shadows an input; choose another name"
            err = walk(node.args[1], scope)
            return err or walk(node.args[2], scope | {name})
        if op == "-" and len(node.args) not in (1, 2):
            return "'-' takes one (negation) or two (subtraction) arguments"
        if op in ("+", "*") and len(node.args) < 2:
            return f"'{op}' takes at least two arguments"
        for a in node.args:
            err = walk(a, scope)
            if err:
                return err
        return None

    return walk(ast, set(available))


def replace_literals(ast: ASTNode, mapping: Dict[str, str]) -> ASTNode:
    if ast.is_leaf:
        if is_num_literal(ast.val):
            key = repr(float(ast.val))
            if key not in mapping:
                mapping[key] = f"lit_{len(mapping)}"
            return ASTNode(mapping[key], [], is_leaf=True)
        return ast
    if ast.val == "let":
        return ASTNode("let", [ast.args[0]] + [replace_literals(a, mapping) for a in ast.args[1:]])
    return ASTNode(ast.val, [replace_literals(a, mapping) for a in ast.args])


# --- CKKS reference programs (exactly the computation each benchmark performs)


def _let_chain(bindings: List[Tuple[str, str]], body: str) -> str:
    expr = body
    for name, value in reversed(bindings):
        expr = f"(let {name} {value} {expr})"
    return expr


def chebyshev_reference(io: IoFile, eps: float = 1e-14) -> str:
    """Mirror of benchmarks/chebyshev_ckks/chebyshev_ckks.cpp fhe(set, "p")."""

    def val(label):
        return float(io.inputs[label][1][0])

    degree = max(int(l[1:]) for l in io.inputs if re.fullmatch(r"c\d+", l))
    b = []
    z = "(* x alpha)"
    if abs(val("beta")) > eps:
        z = f"(+ {z} beta)"
    b.append(("z", z))
    t = {1: "z"}
    b.append(("zsq", "(* z z)"))
    b.append(("t2", "(- (+ zsq zsq) one)"))
    t[2] = "t2"
    for k in range(3, degree + 1):
        b.append((f"zt{k}", f"(* z {t[k - 1]})"))
        b.append((f"t{k}", f"(- (+ zt{k} zt{k}) {t[k - 2]})"))
        t[k] = f"t{k}"
    result = None
    for k in range(1, degree + 1):
        if abs(val(f"c{k}")) <= eps:
            continue
        term = f"(* {t[k]} c{k})"
        result = term if result is None else f"(+ {result} {term})"
    if abs(val("c0")) > eps:
        result = f"(+ {result} c0)"
    return _let_chain(b, result)


def relu_reference(io: IoFile, degrees=(7, 7, 7)) -> str:
    """Mirror of benchmarks/relu_ckks/relu_ckks.cpp (OddPolynomial stages)."""
    bindings: List[Tuple[str, str]] = []
    t = "x"
    for stage, degree in enumerate(degrees, start=1):
        pow2 = [t]
        while (1 << len(pow2)) < degree:
            name = f"s{stage}p{1 << len(pow2)}"
            bindings.append((name, f"(* {pow2[-1]} {pow2[-1]})"))
            pow2.append(name)

        def coeff(k):
            return f"s{stage}c{k}"

        def ev(offset, deg):
            if deg == 1:
                return f"(* {pow2[0]} {coeff(offset + 1)})"
            if deg == 3:
                return f"(+ (* {pow2[0]} {coeff(offset + 1)}) (* (* {pow2[0]} {coeff(offset + 3)}) {pow2[1]}))"
            j = 0
            while (2 << j) < deg:
                j += 1
            m = 1 << j
            return f"(+ {ev(offset, m - 1)} (* {ev(offset + m, deg - m)} {pow2[j]}))"

        out = f"t{stage}"
        bindings.append((out, ev(0, degree)))
        t = out
    return _let_chain(bindings, f"(* x (+ {t} half))")


# ---------------------------------------------------------------------------
# Compiler interface
# ---------------------------------------------------------------------------


class CompilerInterface:
    """Compiles, runs and verifies baselines and candidates for one benchmark."""

    def __init__(
        self,
        repo_root: Optional[str] = None,
        benchmark_name: str = "dot_product",
        slot_count: int = 4,
        exec_warmup: int = 1,
        exec_repeats: int = 5,
        ckks_processes: int = 3,
        ckks_eval_repeats: int = 5,
        ckks_tolerance: float = 1e-6,
        compile_timeout: float = 600,
        run_timeout: float = 600,
    ):
        self.repo_root = Path(repo_root).resolve() if repo_root else Path(__file__).resolve().parent.parent
        self.benchmark_name = benchmark_name
        self.slot_count = slot_count
        self.exec_warmup = exec_warmup
        self.exec_repeats = exec_repeats
        self.ckks_processes = ckks_processes
        self.ckks_eval_repeats = ckks_eval_repeats
        self.ckks_tolerance = ckks_tolerance
        self.compile_timeout = compile_timeout
        self.run_timeout = run_timeout

        self.conda_prefix = os.environ.get("CONDA_PREFIX", "/opt/anaconda3")
        self.env = os.environ.copy()
        self.cmake = shutil.which("cmake") or "cmake"

        self.bench_parent = self.repo_root / "build" / "benchmarks"
        acquire_build_lock(self.bench_parent)
        self.shared_files = {
            name: self.bench_parent / f"{name}.txt"
            for name in ("expression", "inputs", "vectorized_code", "candidate_vectorized_code")
        }
        self._resolve_layout()

        self._setup_done = False
        self._unoptimized: Optional[EvaluationResult] = None
        self._egraph: Optional[EvaluationResult] = None
        self.reference_result: Optional[EvaluationResult] = None  # CKKS: reference program via candidate path
        self.unoptimized_expression = ""
        self.available_vars: Set[str] = set()
        self.input_types: Dict[str, str] = {}
        # BFV state
        self.plain_modulus: Optional[int] = None
        self.reference_slots: List[int] = []
        self.allowed_literals: Set[str] = {"0"}
        # CKKS state
        self.base_io: Optional[IoFile] = None

    # -- layout --------------------------------------------------------------

    def _resolve_layout(self):
        name, s = self.benchmark_name, self.slot_count
        if "ckks" in name:
            self.scheme = "CKKS"
            if name == "quad_ckks":
                folder, gen = "quad_ckks", ["python3", "generate_quad_ckks.py", "--slot_count", str(s), "--seed", "0"]
                cmd, kind = f"./quad_ckks {s} 1 1 1", "quad"
            elif name.startswith("batchnorm"):
                case = "bn1d" if "1d" in name else "bn2d"
                folder, gen = "batchnorm_ckks", ["./generate_batchnorm_ckks", case]
                cmd, kind = f"./batchnorm_ckks {s} 1 1 1", "batchnorm"
            elif name.startswith("chebyshev"):
                parts = name.split("_")
                act, deg = parts[1], parts[2].lstrip("d")
                folder, gen = "chebyshev_ckks", ["./generate_chebyshev_ckks", "p", "points9", act, deg]
                cmd, kind = f"./chebyshev_ckks p {s} 1 1 1 {act} {deg}", "chebyshev"
            elif name == "relu_ckks":
                folder, gen = "relu_ckks", ["python3", "generate_relu_ckks.py", "points9"]
                cmd, kind = f"./relu_ckks {s} 1 1 1", "relu"
            else:
                raise HarnessError(
                    f"CKKS benchmark '{name}' has no reference program in the harness yet "
                    "(supported: quad_ckks, batchnorm1d/2d_ckks, chebyshev_<act>_d<k>_ckks, relu_ckks)."
                )
            self.ckks_kind = kind
        else:
            self.scheme = "BFV"
            folder, gen = name, ["python3", f"generate_{name}.py", "--slot_count", str(s)]
            cmd = None
        self.folder = folder
        self.build_dir = self.bench_parent / folder
        self.he_dir = self.build_dir / "he"
        self.gen_cmd = gen
        self.ckks_baseline_cmd = cmd
        self.cand_dir = self.bench_parent / "ckks_candidate"

    # -- shared steps --------------------------------------------------------

    def _clear(self, paths):
        for p in paths:
            if p.is_dir():
                shutil.rmtree(p, ignore_errors=True)
            elif p.exists():
                p.unlink()

    def _clear_generated(self, build_dir: Path):
        he = build_dir / "he"
        self._clear(
            [he / "_gen_he_fhe.cpp", he / "_gen_he_fhe.hpp", he / "build",
             he / "fhe_io_example_adapted.txt", build_dir / "fhe_io_example_adapted.txt"]
        )

    def _compile(self, cmd: str, build_dir: Path) -> Tuple[Optional[str], subprocess.CompletedProcess]:
        try:
            res = run(cmd, build_dir, self.compile_timeout, self.env)
        except subprocess.TimeoutExpired:
            return f"CHEHAB compilation timed out after {self.compile_timeout:.0f}s", None
        if res.returncode != 0:
            return f"CHEHAB exited with status {res.returncode}:\n{tail(res.stderr or res.stdout)}", res
        if not (build_dir / "he" / "_gen_he_fhe.cpp").exists():
            return f"CHEHAB produced no generated code:\n{tail(res.stderr or res.stdout)}", res
        return None, res

    def _build_he(self, build_dir: Path) -> Optional[str]:
        he = build_dir / "he"
        # Use the current runtime helpers, not whatever the last build copied.
        helpers = ["ckks_utils.cpp", "ckks_utils.hpp"] if self.scheme == "CKKS" else ["utils.cpp", "utils.hpp"]
        for h in helpers:
            shutil.copy2(self.repo_root / "benchmarks" / h, he / h)
        try:
            r = run([self.cmake, "-S", ".", "-B", "build", f"-DCMAKE_PREFIX_PATH={self.conda_prefix}",
                     "-DCMAKE_BUILD_TYPE=Release"], he, 300, self.env)
            if r.returncode != 0:
                return f"cmake configure failed:\n{tail(r.stderr or r.stdout)}"
            r = run([self.cmake, "--build", "build", "-j", "8"], he, 600, self.env)
            if r.returncode != 0:
                return f"SEAL C++ build failed:\n{tail(r.stderr or r.stdout)}"
        except subprocess.TimeoutExpired:
            return "SEAL C++ build timed out"
        if not (he / "build" / "main").exists():
            return "SEAL executable 'main' not found after build"
        return None

    def _gen_metrics(self, build_dir: Path, compile_stdout: str, result: EvaluationResult):
        gen = build_dir / "he" / "_gen_he_fhe.cpp"
        if gen.exists():
            result.op_counts = count_ops(gen.read_text())
        result.depth, result.multiplicative_depth = parse_depth(compile_stdout)
        result.compilation_time_ms = parse_compile_time(compile_stdout)

    # -- BFV -----------------------------------------------------------------

    def _run_bfv_main(self, build_dir: Path) -> Tuple[Optional[str], Dict[str, Any]]:
        main_dir = build_dir / "he" / "build"
        samples, outputs, noise, match, t = [], None, None, None, None
        for i in range(self.exec_warmup + self.exec_repeats):
            try:
                r = run("./main", main_dir, self.run_timeout, self.env)
            except subprocess.TimeoutExpired:
                return f"SEAL execution timed out after {self.run_timeout:.0f}s", {}
            if r.returncode != 0:
                return f"SEAL execution failed (status {r.returncode}):\n{tail(r.stderr or r.stdout)}", {}
            m = re.search(r"execution_time_\(ms\):\s*([0-9.eE+-]+)", r.stdout)
            if not m:
                return f"no execution_time in SEAL output:\n{tail(r.stdout)}", {}
            run_outputs = {
                mm.group(1): [int(v) for v in mm.group(2).split()]
                for mm in re.finditer(r"decrypted_output:\s*(\S+)((?:\s+\d+)*)", r.stdout)
            }
            if outputs is not None and run_outputs != outputs:
                return "decrypted outputs differ between runs", {}
            outputs = run_outputs
            nb = [int(v) for v in re.findall(r"Remaining_noise_budget:\s*(-?\d+)", r.stdout)]
            noise = min(nb) if nb else noise
            mm = re.search(r"outputs_match:\s*(\d)", r.stdout)
            match = int(mm.group(1)) if mm else match
            mt = re.search(r"plain_modulus:\s*(\d+)", r.stdout)
            t = int(mt.group(1)) if mt else t
            if i >= self.exec_warmup:
                samples.append(float(m.group(1)))
        return None, {"samples": samples, "outputs": outputs or {}, "noise": noise or 0, "match": match, "t": t}

    def _check_vectorized(self, outputs: Dict[str, List[int]]) -> Tuple[bool, str]:
        if len(outputs) != 1:
            return False, f"expected exactly one output ciphertext, got {len(outputs)}"
        slots = next(iter(outputs.values()))
        n = len(self.reference_slots)
        if len(slots) < n:
            return False, f"output has {len(slots)} slots but the computation has {n} outputs"
        wrong = [i for i in range(n) if slots[i] != self.reference_slots[i]]
        if wrong:
            return False, (
                f"{len(wrong)} of the first {n} output slots hold wrong values (slot indices {wrong[:12]}"
                f"{'...' if len(wrong) > 12 else ''}); slot i must equal element i of the original (Vec ...)."
            )
        return True, f"all {n} output slots equal the reference values"

    def _bfv_setup(self):
        # 1. Fresh inputs and independently computed expected outputs.
        r = run(self.gen_cmd, self.build_dir, 300, self.env)
        io_path = self.build_dir / "fhe_io_example.txt"
        if r.returncode != 0 or not io_path.exists():
            raise HarnessError(f"generator failed: {tail(r.stderr or r.stdout)}")
        io = parse_io_file(io_path)
        if io.slot_count != 1:
            raise HarnessError(
                f"io file has function slot count {io.slot_count}; the BFV correctness check supports scalar "
                "benchmarks (slot count 1) only"
            )

        # 2. Unoptimized scalar baseline (CHEHAB's own golden check applies here).
        self._unoptimized = self._bfv_measure_program(
            f"./{self.benchmark_name} 0 {self.slot_count} 0 0 1 1 1", "unoptimized (scalar)", vectorized=False
        )
        if not self._unoptimized.success:
            raise HarnessError(
                f"unoptimized baseline failed at {self._unoptimized.stage_failed}: "
                f"{self._unoptimized.error_message or self._unoptimized.correctness_detail}"
            )

        # 3. E-graph compile: also emits this benchmark's scalar expression.
        self._clear(list(self.shared_files.values()))
        self._clear_generated(self.build_dir)
        egraph_cmd = f"./{self.benchmark_name} 1 {self.slot_count} 0 0 1 1 1"
        err, res = self._compile(egraph_cmd, self.build_dir)
        expr_file, inputs_file = self.shared_files["expression"], self.shared_files["inputs"]
        if not expr_file.exists() or not expr_file.read_text().strip():
            raise HarnessError("CHEHAB did not write expression.txt for this benchmark")
        self.unoptimized_expression = expr_file.read_text().strip()
        labels = inputs_file.read_text().split("\n")[0].split() if inputs_file.exists() else []
        self.available_vars = set(labels)
        unknown = self.available_vars - set(io.inputs)
        if not self.available_vars or unknown:
            raise HarnessError(f"inputs.txt does not match this benchmark's io file (unknown: {sorted(unknown)})")
        self.input_types = {l: "ciphertext" if io.inputs[l][0] else "plaintext" for l in self.available_vars}

        # 4. Reference output slots, cross-checked against the generator's expected outputs.
        t = self.plain_modulus
        ast = parse_sexpr(self.unoptimized_expression)
        if ast.is_leaf or ast.val != "Vec":
            raise HarnessError("expression.txt is not a (Vec ...) expression")
        values = {l: int(v[0]) for l, (_, v) in io.inputs.items()}
        self.reference_slots = [eval_scalar_expr(a, values, t) for a in ast.args]
        expected = sorted(int(v[0]) % t for v in io.outputs.values())
        if sorted(self.reference_slots) != expected:
            raise HarnessError(
                "expression.txt does not compute this benchmark's expected outputs; refusing to continue"
            )
        self.allowed_literals = {"0"} | {tok for tok in tokenize(self.unoptimized_expression) if is_int_literal(tok)}

        # 5. Measure the e-graph program generated in step 3.
        eg = EvaluationResult(success=False, candidate_expr="CHEHAB e-graph vectorizer", scheme="BFV")
        if err:
            eg.stage_failed, eg.error_message = "chehab_compiler", err
        else:
            vec = self.shared_files["vectorized_code"]
            eg.candidate_expr = vec.read_text().splitlines()[0] if vec.exists() else eg.candidate_expr
            self._finish_bfv(eg, res, vectorized=True)
        self._egraph = eg

    def _bfv_measure_program(self, cmd: str, label: str, vectorized: bool) -> EvaluationResult:
        result = EvaluationResult(success=False, candidate_expr=label, scheme="BFV")
        self._clear_generated(self.build_dir)
        err, res = self._compile(cmd, self.build_dir)
        if err:
            result.stage_failed, result.error_message = "chehab_compiler", err
            return result
        return self._finish_bfv(result, res, vectorized)

    def _finish_bfv(self, result: EvaluationResult, res, vectorized: bool) -> EvaluationResult:
        self._gen_metrics(self.build_dir, res.stdout, result)
        err = self._build_he(self.build_dir)
        if err:
            result.stage_failed, result.error_message = "seal_build", err
            return result
        err, out = self._run_bfv_main(self.build_dir)
        if err:
            result.stage_failed, result.error_message = "seal_execution", err
            return result
        result.latency_samples_ms = out["samples"]
        result.execution_latency_ms = statistics.median(out["samples"])
        result.remaining_noise_budget = out["noise"]
        if out["t"]:
            self.plain_modulus = out["t"]
        result.correctness_checked = True
        if vectorized:
            ok, detail = self._check_vectorized(out["outputs"])
        else:
            ok = out["match"] == 1
            detail = "CHEHAB golden check: every output equals the generator's expected value" if ok else (
                "CHEHAB golden check failed" if out["match"] is not None else "no outputs_match line")
        result.correct, result.correctness_detail = ok, detail
        if not ok:
            result.stage_failed = "correctness"
            return result
        result.success = True
        return result

    def _bfv_candidate(self, expr: str, ast: ASTNode, iteration: int) -> EvaluationResult:
        result = EvaluationResult(success=False, candidate_expr=expr, scheme="BFV")
        widths = extract_vec_lengths(ast)
        n_out = len(self.reference_slots)
        if not widths:
            result.stage_failed, result.error_message = "syntax_validation", "candidate contains no (Vec ...) packing"
            return result
        if widths[0] < n_out:
            result.stage_failed = "syntax_validation"
            result.error_message = f"vector width {widths[0]} is smaller than the number of outputs ({n_out})"
            return result
        rot_shift_ids = set()

        def collect(node):
            if not node.is_leaf:
                if node.val in ("VecAddRot", "VecMulRot", "VecMinusRot", "<<") and len(node.args) == 2:
                    rot_shift_ids.add(id(node.args[1]))
                for a in node.args:
                    collect(a)

        collect(ast)
        bad = set()

        def lits(node):
            if node.is_leaf:
                if is_int_literal(node.val) and id(node) not in rot_shift_ids and node.val not in self.allowed_literals:
                    bad.add(node.val)
            else:
                for a in node.args:
                    lits(a)

        lits(ast)
        if bad:
            result.stage_failed = "syntax_validation"
            result.error_message = (
                f"integer literals {sorted(bad)} do not appear in the original expression; only "
                f"{sorted(self.allowed_literals)} (and rotation shift amounts) are allowed"
            )
            return result

        self._clear([self.shared_files["vectorized_code"], self.shared_files["candidate_vectorized_code"]])
        self._clear_generated(self.build_dir)
        width = widths[0]
        self.shared_files["candidate_vectorized_code"].write_text(f"{expr.strip()}\n{width} {width} 1 1\n")
        err, res = self._compile(f"./{self.benchmark_name} 1 {self.slot_count} 2 0 1 1 1", self.build_dir)
        if err:
            result.stage_failed, result.error_message = "chehab_compiler", err
            return result
        return self._finish_bfv(result, res, vectorized=True)

    # -- CKKS ----------------------------------------------------------------

    def _run_ckks_main(self, build_dir: Path) -> Tuple[Optional[str], Dict[str, Any]]:
        main_dir = build_dir / "he" / "build"
        medians, errors, levels, passes = [], [], None, []
        for _ in range(self.ckks_processes):
            try:
                r = run(["./main", "--tol", repr(self.ckks_tolerance), "--repeats", str(self.ckks_eval_repeats)],
                        main_dir, self.run_timeout, self.env)
            except subprocess.TimeoutExpired:
                return f"SEAL execution timed out after {self.run_timeout:.0f}s", {}
            m = re.search(r"fhe_eval_median_\(ms\):\s*([0-9.eE+-]+)", r.stdout)
            if not m:
                return f"SEAL execution failed (status {r.returncode}):\n{tail(r.stderr or r.stdout)}", {}
            medians.append(float(m.group(1)))
            errors += [float(v) for v in re.findall(r"max_abs_error=([0-9.eE+-]+|nan|inf)", r.stdout)]
            lv = re.findall(r"levels consumed (\d+)", r.stdout)
            levels = max(int(v) for v in lv) if lv else levels
            passes.append(bool(re.search(r"^PASS", r.stdout, re.M)) and r.returncode == 0)
        return None, {"medians": medians, "max_err": max(errors) if errors else float("nan"),
                      "levels": levels or 0, "pass": all(passes) and len(passes) == self.ckks_processes}

    def _finish_ckks(self, result: EvaluationResult, build_dir: Path, res) -> EvaluationResult:
        self._gen_metrics(build_dir, res.stdout, result)
        err = self._build_he(build_dir)
        if err:
            result.stage_failed, result.error_message = "seal_build", err
            return result
        err, out = self._run_ckks_main(build_dir)
        if err:
            result.stage_failed, result.error_message = "seal_execution", err
            return result
        result.latency_samples_ms = out["medians"]
        result.execution_latency_ms = statistics.median(out["medians"])
        result.ckks_max_abs_error = out["max_err"]
        result.ckks_levels_consumed = out["levels"]
        result.ckks_tolerance = self.ckks_tolerance
        result.correctness_checked = True
        result.correct = out["pass"]
        result.correctness_detail = (
            f"max abs error {out['max_err']:.3e} vs expected outputs (tolerance {self.ckks_tolerance:g})"
        )
        if not result.correct:
            result.stage_failed = "correctness"
            return result
        result.success = True
        return result

    def _reference_expression(self, io: IoFile) -> str:
        if self.ckks_kind == "quad":
            return "(* x x)"
        if self.ckks_kind == "batchnorm":
            return "(+ (* x A) B)"
        if self.ckks_kind == "chebyshev":
            return chebyshev_reference(io)
        return relu_reference(io)

    def _ckks_setup(self):
        r = run(self.gen_cmd, self.build_dir, 300, self.env)
        io_path = self.build_dir / "fhe_io_example.txt"
        if r.returncode != 0 or not io_path.exists():
            raise HarnessError(f"generator failed: {tail(r.stderr or r.stdout)}")
        self.base_io = parse_io_file(io_path)
        if len(self.base_io.outputs) != 1:
            raise HarnessError("CKKS candidate evaluation supports single-output benchmarks only")
        self.available_vars = set(self.base_io.inputs)
        self.input_types = {
            l: "ciphertext" if c else ("plaintext scalar" if len(v) == 1 else "plaintext vector")
            for l, (c, v) in self.base_io.inputs.items()
        }

        # Baseline: the benchmark program as written, compiled by CHEHAB.
        base = EvaluationResult(success=False, candidate_expr=f"{self.benchmark_name} (as written)", scheme="CKKS")
        self._clear_generated(self.build_dir)
        err, res = self._compile(self.ckks_baseline_cmd, self.build_dir)
        if err:
            base.stage_failed, base.error_message = "chehab_compiler", err
        else:
            self._finish_ckks(base, self.build_dir, res)
        self._unoptimized = base
        if not base.success:
            raise HarnessError(
                f"unoptimized baseline failed at {base.stage_failed}: {base.error_message or base.correctness_detail}"
            )
        self._egraph = EvaluationResult(
            success=False, candidate_expr="", scheme="CKKS", stage_failed="not_applicable",
            error_message="CHEHAB's e-graph vectorizer is BFV-only (Func::require_bfv); no CKKS e-graph baseline",
        )

        # The same computation as an S-expression, verified through the candidate path.
        self.unoptimized_expression = self._reference_expression(self.base_io)
        ref = self._ckks_candidate(self.unoptimized_expression, parse_sexpr(self.unoptimized_expression))
        self.reference_result = ref
        if not ref.success:
            raise HarnessError(
                "the reference S-expression failed through the candidate path "
                f"({ref.stage_failed}: {ref.error_message or ref.correctness_detail}); refusing to continue"
            )

    def _ckks_candidate(self, expr: str, ast: ASTNode) -> EvaluationResult:
        result = EvaluationResult(success=False, candidate_expr=expr, scheme="CKKS")
        err = check_ckks_expression(ast, self.available_vars)
        if err:
            result.stage_failed, result.error_message = "syntax_validation", err
            return result
        mapping: Dict[str, str] = {}
        rewritten = replace_literals(ast, mapping)
        io = self.base_io
        lines = list(io.raw_input_lines)
        for value, label in mapping.items():
            lines.append(f"{label} 0 1 {float(value)!r}")
        out_label = io.output_order[0]
        out_line = f"{out_label} 1 {' '.join(io.outputs[out_label])}"
        io_text = f"{io.slot_count} {len(lines)} 1\n" + "\n".join(lines) + "\n" + out_line + "\n"

        self._clear_generated(self.cand_dir)
        (self.cand_dir / "fhe_io_example.txt").write_text(io_text)
        (self.cand_dir / "candidate_expr.txt").write_text(rewritten.to_sexpr() + "\n")
        err, res = self._compile(f"./ckks_candidate candidate_expr.txt {io.slot_count} 1 1 1", self.cand_dir)
        if err:
            result.stage_failed, result.error_message = "chehab_compiler", err
            return result
        return self._finish_ckks(result, self.cand_dir, res)

    # -- public API ----------------------------------------------------------

    def setup(self):
        """Generate inputs, measure baselines and prepare the reference. Raises HarnessError."""
        if self._setup_done:
            return
        if not self.build_dir.exists():
            raise HarnessError(f"benchmark build directory {self.build_dir} does not exist")
        if self.scheme == "BFV":
            self._bfv_setup()
        else:
            self._ckks_setup()
        self._setup_done = True

    def get_unoptimized_expression(self) -> str:
        self.setup()
        return self.unoptimized_expression

    def get_available_variables(self) -> Set[str]:
        self.setup()
        return set(self.available_vars)

    def get_unoptimized_baseline(self) -> EvaluationResult:
        self.setup()
        return self._unoptimized

    def get_egraph_baseline(self) -> EvaluationResult:
        self.setup()
        return self._egraph

    def evaluate_candidate(self, candidate_expr: str, iteration: int = 1) -> EvaluationResult:
        self.setup()
        expr = candidate_expr.strip()
        if self.scheme == "BFV":
            ok, msg, ast = validate_expression(expr, available_vars=self.available_vars)
        else:
            try:
                ast, ok, msg = parse_sexpr(expr), True, ""
            except SyntaxError as e:
                ast, ok, msg = None, False, f"Syntax Error: {e}"
        if not ok or ast is None:
            return EvaluationResult(success=False, candidate_expr=expr, scheme=self.scheme,
                                    stage_failed="syntax_validation", error_message=msg)
        if self.scheme == "BFV":
            result = self._bfv_candidate(expr, ast, iteration)
            result.static_cost = evaluate_cost_metrics(ast).get("cost", 0.0)
        else:
            result = self._ckks_candidate(expr, ast)
        return result
