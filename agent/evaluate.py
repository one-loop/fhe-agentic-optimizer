"""Compile, run and measure one candidate CHEHAB DSL kernel (the "CHEHAB
compiler environment" of the proposal's feedback loop).

evaluate(task, kernel_source) returns an Evaluation:
  1. static check of the kernel (DSL only: no preprocessor, I/O or compiler internals);
  2. compile phase: agent_kernel (agent/driver.cpp + the kernel) is rebuilt and run;
     CHEHAB compiles the kernel and generates SEAL CKKS code, and its Quantifier
     reports depth and operation counts;
  3. execution phase, for every case of the task: the generated program is built and
     run on encrypted inputs; the decrypted outputs are compared with the expected
     outputs within the task's tolerance.
A candidate is correct when every phase succeeds and every case passes.
"""
import hashlib
import json
import os
import re
import shutil
import statistics
import subprocess
from dataclasses import asdict, dataclass, field
from pathlib import Path
from typing import Dict, List, Optional

REPO = Path(__file__).resolve().parent.parent
BUILD = REPO / "build"
KERNEL_INC = BUILD / "agent" / "kernel" / "kernel.inc"
DRIVER = BUILD / "agent" / "agent_kernel"
WORK = BUILD / "agent" / "work"

# Tokens a kernel may not contain: it must stay plain CHEHAB DSL.
FORBIDDEN = ["#", "Compiler::", "util::", "std::system", "system(", "fopen", "fstream", "ifstream", "ofstream",
             "FILE", "asm", "__", "main(", "getenv", "exec", "popen", "operators::"]

QUANTIFIER_TOTALS = {
    "mul": "ct_ct_mul", "mul_plain": "ct_pt_mul", "relin": "relin", "rotate": "rotate",
    "rescale": "rescale", "mod_switch": "mod_switch", "match_scale": "match_scale", "he_add": "add",
}


@dataclass
class CaseResult:
    name: str
    slot_count: int
    passed: bool = False
    max_abs_error: Optional[float] = None
    levels_consumed: Optional[int] = None
    exec_time_ms: Optional[float] = None  # parse + encode + encrypt + evaluate (CHEHAB convention)
    eval_time_ms: Optional[float] = None  # fhe() alone, median of the timed runs
    output: str = ""


@dataclass
class Evaluation:
    status: str  # "ok", "rejected", "compile_error", "chehab_error", "build_error", "run_error", "wrong_output"
    correct: bool = False
    message: str = ""  # short explanation for the LLM and the log
    compile_time_ms: Optional[float] = None
    depth: Optional[int] = None
    mult_depth: Optional[int] = None
    terms: Optional[int] = None
    ops: Dict[str, int] = field(default_factory=dict)
    cases: List[CaseResult] = field(default_factory=list)

    @property
    def eval_time_ms(self) -> Optional[float]:
        """Primary objective: fhe() time on the task's timing case."""
        timed = [c.eval_time_ms for c in self.cases if c.eval_time_ms is not None and c.name == "dense"]
        timed = timed or [c.eval_time_ms for c in self.cases if c.eval_time_ms is not None]
        return timed[0] if timed else None

    @property
    def levels(self) -> Optional[int]:
        levels = [c.levels_consumed for c in self.cases if c.levels_consumed is not None]
        return max(levels) if levels else None

    @property
    def max_abs_error(self) -> Optional[float]:
        errors = [c.max_abs_error for c in self.cases if c.max_abs_error is not None]
        return max(errors) if errors else None

    def to_dict(self):
        d = asdict(self)
        d.update(eval_time_ms=self.eval_time_ms, levels=self.levels, max_abs_error=self.max_abs_error)
        for c in d["cases"]:
            c["output"] = c["output"][-2000:]
        return d

    def summary(self) -> str:
        """One-paragraph metrics summary, shown to the LLM."""
        if not self.correct:
            return f"status: {self.status}\n{self.message}"
        ops = ", ".join(f"{k}={v}" for k, v in self.ops.items())
        lines = [
            "status: ok (correct on every case)",
            f"fhe() evaluation time (dense case, median): {self.eval_time_ms:.2f} ms",
            f"CKKS levels consumed: {self.levels}",
            f"CHEHAB depth (total, multiplicative): ({self.depth}, {self.mult_depth}); IR terms: {self.terms}",
            f"operations: {ops}",
            f"max abs error vs expected: {self.max_abs_error:.3g}",
        ]
        return "\n".join(lines)


def _run(cmd, cwd, timeout):
    return subprocess.run(cmd, cwd=cwd, stdout=subprocess.PIPE, stderr=subprocess.STDOUT, text=True,
                          timeout=timeout, env=_env())


def _env():
    env = os.environ.copy()
    # Same toolchain choice as run_benchmarks.py.
    if Path("/usr/bin/gcc").exists():
        env.setdefault("CC", "/usr/bin/gcc")
        env.setdefault("CXX", "/usr/bin/g++")
    return env


def strip_comments(source: str) -> str:
    source = re.sub(r"/\*.*?\*/", " ", source, flags=re.S)
    return re.sub(r"//[^\n]*", "", source)


def check_kernel(source: str) -> Optional[str]:
    """None if the kernel is acceptable, else the reason it is rejected."""
    code = strip_comments(source)
    if "void fhe()" not in code:
        return "the kernel must define `void fhe()` (no parameters)"
    for token in FORBIDDEN:
        if token in code:
            return f"the kernel may not contain `{token}`: only CHEHAB DSL code is allowed"
    return None


def _compiler_errors(output: str) -> str:
    """Keep the compiler diagnostics that concern the kernel, with their source line."""
    lines = output.splitlines()
    keep = []
    for i, line in enumerate(lines):
        if "kernel.inc:" in line:
            keep.append(re.sub(r"^.*kernel\.inc:", "kernel:", line))
            keep += [l for l in lines[i + 1:i + 3] if "kernel.inc:" not in l and not l.startswith("make")]
    text = "\n".join(keep) if keep else output
    return text[-3000:]


def _quantifier(stdout: str, ev: Evaluation):
    if m := re.search(r"^([0-9.eE+-]+) ms$", stdout, re.M):
        ev.compile_time_ms = float(m.group(1))
    if m := re.search(r"max:\s*\((\d+),\s*(\d+)\)", stdout):
        ev.depth, ev.mult_depth = int(m.group(1)), int(m.group(2))
    if m := re.search(r"\|terms\|:\s*(\d+)", stdout):
        ev.terms = int(m.group(1))
    # Each "|op| (...): count" section ends with "total: N".
    for name, key in QUANTIFIER_TOTALS.items():
        m = re.search(r"^\|" + re.escape(name) + r"\|[^\n]*\n(?:[^\n]*\n)*?total: (\d+)", stdout, re.M)
        if m:
            ev.ops[key] = int(m.group(1))


def _prepare_case(task, case, case_dir: Path) -> int:
    """Write the case's fhe_io_example.txt into case_dir; return its slot count."""
    bench_dir = BUILD / "benchmarks" / task.bench_dir
    out = _run(case.generator, bench_dir, 300)
    if out.returncode != 0:
        raise RuntimeError(f"input generator failed: {' '.join(case.generator)}\n{out.stdout}")
    slot = case.slot_count if case.slot_count else int(out.stdout.splitlines()[0])
    case_dir.mkdir(parents=True, exist_ok=True)
    shutil.copy(bench_dir / "fhe_io_example.txt", case_dir / "fhe_io_example.txt")
    he = case_dir / "he"
    he.mkdir(exist_ok=True)
    for name in ("ckks_utils.cpp", "ckks_utils.hpp"):
        shutil.copy(REPO / "benchmarks" / name, he / name)
    shutil.copy(REPO / "benchmarks" / "relu_ckks" / "he" / "CMakeLists.txt", he / "CMakeLists.txt")
    return slot


def _parse_run(out: str, cr: CaseResult):
    errors = [float(v) for v in re.findall(r"max_abs_error=([0-9.eE+-]+|nan|inf)", out)]
    if errors:
        cr.max_abs_error = max(errors)
    levels = [int(v) for v in re.findall(r"levels consumed (\d+)\)", out)]
    if levels:
        cr.levels_consumed = max(levels)
    if m := re.search(r"execution_time_\(ms\):\s*([0-9.eE+-]+)", out):
        cr.exec_time_ms = float(m.group(1))
    if m := re.search(r"fhe_eval_median_\(ms\):\s*([0-9.eE+-]+)", out):
        cr.eval_time_ms = float(m.group(1))


def evaluate(task, source: str, seal_prefix: str, repeats: int = 10, timeout: float = 900) -> Evaluation:
    if reason := check_kernel(source):
        return Evaluation("rejected", message=reason)

    # Compile phase: rebuild the driver with this kernel.
    KERNEL_INC.parent.mkdir(parents=True, exist_ok=True)
    KERNEL_INC.write_text(source)
    # make tracks timestamps to the second: remove the driver's object and binary so
    # that a kernel written within a second of the previous build is always rebuilt.
    for stale in (BUILD / "agent" / "CMakeFiles" / "agent_kernel.dir" / "driver.cpp.o", DRIVER):
        stale.unlink(missing_ok=True)
    build = _run(["cmake", "--build", str(BUILD), "--target", "agent_kernel", "-j8"], REPO, timeout)
    if build.returncode != 0:
        return Evaluation("compile_error", message="C++ compilation of the kernel failed:\n"
                          + _compiler_errors(build.stdout))

    digest = hashlib.sha1(source.encode()).hexdigest()[:10]
    ev = Evaluation("ok")
    for case in task.cases:
        case_dir = WORK / task.name / case.name
        shutil.rmtree(case_dir, ignore_errors=True)
        slot = _prepare_case(task, case, case_dir)
        cr = CaseResult(case.name, slot)
        ev.cases.append(cr)

        comp = _run([str(DRIVER), str(slot)], case_dir, timeout)
        if comp.returncode != 0:
            ev.status = "chehab_error"
            ev.message = f"CHEHAB rejected the kernel (case {case.name}, {slot} slots):\n" + comp.stdout[-3000:]
            return ev
        # Depth and operation counts are reported for the timing case ("dense").
        if case.name == "dense" or case is task.cases[0]:
            ev.ops = {}
            _quantifier(comp.stdout, ev)
        (case_dir / "compile_output.txt").write_text(comp.stdout)

        he = case_dir / "he"
        cfg = _run(["cmake", "-S", ".", "-B", "build", f"-DCMAKE_PREFIX_PATH={seal_prefix}",
                    "-DCMAKE_BUILD_TYPE=Release"], he, timeout)
        bld = _run(["cmake", "--build", "build", "-j8"], he, timeout) if cfg.returncode == 0 else cfg
        if bld.returncode != 0:
            ev.status = "build_error"
            ev.message = "the SEAL code generated by CHEHAB did not build:\n" + bld.stdout[-3000:]
            return ev

        run = _run(["./main", "--tol", repr(task.tolerance), "--repeats", str(repeats)], he / "build", timeout)
        cr.output = run.stdout
        _parse_run(run.stdout, cr)
        cr.passed = run.returncode == 0 and cr.max_abs_error is not None
        if not cr.passed:
            if cr.max_abs_error is None:
                ev.status = "run_error"
                ev.message = f"the encrypted program failed (case {case.name}):\n" + run.stdout[-3000:]
            else:
                ev.status = "wrong_output"
                ev.message = (f"wrong result on case {case.name} ({slot} slots): max abs error "
                              f"{cr.max_abs_error:.3g} > tolerance {task.tolerance:g}. The kernel does not "
                              "compute the same function as the original.\n" + run.stdout[-1500:])
            return ev

    ev.correct = True
    ev.message = f"kernel {digest}"
    return ev
