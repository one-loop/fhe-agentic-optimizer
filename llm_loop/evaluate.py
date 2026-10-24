"""Evaluate one candidate kernel: the environment of the LLM loop.

  1. static check of the kernel source (plain CHEHAB DSL only);
  2. compile: the driver is rebuilt with the kernel and run once per case;
     CHEHAB compiles it and generates SEAL CKKS code; static analysis of the
     dense case's code gives levels and operation counts (cheap pre-screen);
  3. execute: for every case (fixed, dense and, when asked, the held-out
     cases) the generated program is built and run on encrypted inputs and
     the decrypted outputs are compared with the expected outputs within the
     task's tolerance.

Work files live under build/llm_loop/ (ignored by git). Not a sandbox: the
kernel is C++ compiled and run on this machine; the token blocklist only
stops accidental or obvious misuse.
"""
import filecmp
import os
import re
import shutil
import subprocess
import time
from dataclasses import asdict, dataclass, field
from pathlib import Path
from typing import Dict, List, Optional

from llm_loop import iofile
from llm_loop.static_analysis import StaticInfo, analyze
from llm_loop.tasks import HELDOUT_SEEDS, REPO, SRC_BENCH, Task

WORK = REPO / "build" / "llm_loop"
DRIVER_SRC = Path(__file__).resolve().parent / "driver"
HE_CMAKE = SRC_BENCH / "quad_ckks" / "he" / "CMakeLists.txt"  # generic: builds main.cpp + _gen_he_fhe.cpp

FORBIDDEN = ["#", "Compiler::", "util::", "operators::", "system", "popen", "exec", "fork", "fopen", "fstream",
             "ofstream", "ifstream", "FILE", "remove(", "rename(", "unlink", "filesystem", "getenv", "asm",
             "__", "main(", "thread", "extern", "reinterpret_cast", "goto"]


@dataclass
class CaseResult:
    name: str
    slot_count: int
    passed: bool = False
    max_abs_error: Optional[float] = None
    levels: Optional[int] = None
    exec_time_ms: Optional[float] = None  # parse + encode + encrypt + fhe() (CHEHAB convention)
    eval_time_ms: Optional[float] = None  # fhe() alone: median of the timed runs
    output_tail: str = ""


@dataclass
class Evaluation:
    status: str  # ok | rejected | compile_error | chehab_error | build_error | run_error | wrong_output
    correct: bool = False
    message: str = ""
    compile_time_ms: Optional[float] = None
    depth: Optional[int] = None
    xdepth: Optional[int] = None
    terms: Optional[int] = None
    static: Dict[str, int] = field(default_factory=dict)
    ops: Dict[str, int] = field(default_factory=dict)
    cases: List[CaseResult] = field(default_factory=list)
    seconds: float = 0.0

    @property
    def eval_time_ms(self) -> Optional[float]:
        """Primary objective: fhe() time on the dense case."""
        return next((c.eval_time_ms for c in self.cases if c.name == "dense"), None)

    @property
    def max_abs_error(self) -> Optional[float]:
        errors = [c.max_abs_error for c in self.cases if c.max_abs_error is not None]
        return max(errors) if errors else None

    def to_dict(self) -> dict:
        d = asdict(self)
        d.update(eval_time_ms=self.eval_time_ms, max_abs_error=self.max_abs_error)
        return d


def check_kernel(source: str) -> Optional[str]:
    code = re.sub(r"//[^\n]*", "", re.sub(r"/\*.*?\*/", " ", source, flags=re.S))
    if not re.search(r"\bvoid\s+fhe\s*\(\s*\)", code):
        return "the kernel must define `void fhe()` (no parameters)"
    for token in FORBIDDEN:
        if token in code:
            return f"the kernel may not contain `{token}`: only CHEHAB DSL code is allowed"
    return None


def _run(cmd, cwd, timeout, env=None) -> subprocess.CompletedProcess:
    return subprocess.run([str(c) for c in cmd], cwd=cwd, stdout=subprocess.PIPE, stderr=subprocess.STDOUT,
                          text=True, timeout=timeout, env=env)


def _env() -> dict:
    env = os.environ.copy()
    if Path("/usr/bin/gcc").exists():  # same toolchain as run_benchmarks.py
        env.setdefault("CC", "/usr/bin/gcc")
        env.setdefault("CXX", "/usr/bin/g++")
    return env


def _sync(src: Path, dst: Path) -> None:
    """Copy src over dst only if it differs, so make does not rebuild unchanged files."""
    if not dst.exists() or not filecmp.cmp(src, dst, shallow=False):
        shutil.copy2(src, dst)


class Evaluator:
    def __init__(self, seal_prefix: Optional[str] = None, work: Path = WORK, repeats: int = 10,
                 timeout: float = 900):
        self.seal_prefix = seal_prefix or os.environ.get("CMAKE_PREFIX_PATH", str(Path.home() / ".local"))
        self.work, self.repeats, self.timeout = Path(work), repeats, timeout
        self.kernel_dir = self.work / "kernel"
        self.driver_build = self.work / "driver"
        self.env = _env()
        self._driver_configured = False

    # ------------------------------------------------------------------ inputs
    def case_dir(self, task: Task, case: str) -> Path:
        return self.work / "cases" / task.name / case

    def prepare(self, task: Task) -> Dict[str, int]:
        """Generate (once) every case's fhe_io_example.txt; returns case -> slot count."""
        slots = {}
        for case in task.cases:
            d = self.case_dir(task, case.name)
            marker = d / "slot_count"
            if not marker.exists():
                d.mkdir(parents=True, exist_ok=True)
                out = _run(case.command, d, 300, self.env)
                if out.returncode != 0:
                    raise RuntimeError(f"{task.name}/{case.name}: generator failed\n{out.stdout}")
                slot = case.slot_count or int(out.stdout.splitlines()[0])
                marker.write_text(str(slot))
            slots[case.name] = int(marker.read_text())
        dense = iofile.read(self.case_dir(task, "dense") / "fhe_io_example.txt")
        for seed in HELDOUT_SEEDS:
            d = self.case_dir(task, f"heldout{seed}")
            if not (d / "slot_count").exists():
                d.mkdir(parents=True, exist_ok=True)
                iofile.write(task.heldout(dense, seed), d / "fhe_io_example.txt")
                (d / "slot_count").write_text(str(dense.slot_count))
            slots[f"heldout{seed}"] = dense.slot_count
        return slots

    # ----------------------------------------------------------------- compile
    def _build_driver(self, source: str) -> Optional[str]:
        """Rebuild the driver with the kernel; returns compiler errors or None."""
        self.kernel_dir.mkdir(parents=True, exist_ok=True)
        (self.kernel_dir / "kernel.inc").write_text(source)
        if not self._driver_configured:
            cfg = _run(["cmake", "-S", DRIVER_SRC, "-B", self.driver_build, f"-DKERNEL_DIR={self.kernel_dir}",
                        "-DCMAKE_BUILD_TYPE=Release"], REPO, self.timeout, self.env)
            if cfg.returncode != 0:
                raise RuntimeError("driver configuration failed:\n" + cfg.stdout[-3000:])
            self._driver_configured = True
        # make compares timestamps to the second: force the rebuild.
        for stale in (self.driver_build / "CMakeFiles" / "llm_driver.dir" / "driver.cpp.o",
                      self.driver_build / "llm_driver"):
            stale.unlink(missing_ok=True)
        bld = _run(["cmake", "--build", self.driver_build, "-j8"], REPO, self.timeout, self.env)
        if bld.returncode == 0:
            return None
        lines = [l for l in bld.stdout.splitlines() if "kernel.inc" in l or "error" in l]
        return "\n".join(l.replace(str(self.kernel_dir) + "/", "") for l in lines)[-3000:] or bld.stdout[-3000:]

    def _compile_case(self, run_dir: Path, slot: int, ev: Evaluation, quantify: bool) -> Optional[str]:
        """Run the driver for one case in run_dir/stage; returns CHEHAB's error or None."""
        stage = run_dir / "stage"
        (stage / "he").mkdir(parents=True, exist_ok=True)
        shutil.copy(run_dir / "fhe_io_example.txt", stage / "fhe_io_example.txt")
        out = _run([self.driver_build / "llm_driver", slot], stage, self.timeout, self.env)
        if out.returncode != 0:
            return out.stdout[-3000:]
        if quantify:
            if m := re.search(r"^([0-9.eE+-]+) ms$", out.stdout, re.M):
                ev.compile_time_ms = float(m.group(1))
            if m := re.search(r"max:\s*\((\d+),\s*(\d+)\)", out.stdout):
                ev.depth, ev.xdepth = int(m.group(1)), int(m.group(2))
            if m := re.search(r"\|terms\|:\s*(\d+)", out.stdout):
                ev.terms = int(m.group(1))
        return None

    # ----------------------------------------------------------------- execute
    def _build_and_run(self, task: Task, run_dir: Path, case: CaseResult) -> None:
        he = run_dir / "he"
        he.mkdir(exist_ok=True)
        for name in ("_gen_he_fhe.hpp", "_gen_he_fhe.cpp", "main.cpp"):
            _sync(run_dir / "stage" / "he" / name, he / name)
        for src in (SRC_BENCH / "ckks_utils.cpp", SRC_BENCH / "ckks_utils.hpp", HE_CMAKE):
            _sync(src, he / src.name)
        _sync(run_dir / "stage" / "fhe_io_example_adapted.txt", run_dir / "fhe_io_example_adapted.txt")
        if not (he / "build" / "CMakeCache.txt").exists():
            cfg = _run(["cmake", "-S", he, "-B", he / "build", f"-DCMAKE_PREFIX_PATH={self.seal_prefix}",
                        "-DCMAKE_BUILD_TYPE=Release"], he, self.timeout, self.env)
            if cfg.returncode != 0:
                raise _Failure("build_error", "configuring the generated program failed:\n" + cfg.stdout[-2000:])
        bld = _run(["cmake", "--build", he / "build", "-j8"], he, self.timeout, self.env)
        if bld.returncode != 0:
            raise _Failure("build_error", "the SEAL code generated by CHEHAB did not build:\n" + bld.stdout[-3000:])
        run = _run(["./main", "--tol", repr(task.tolerance), "--repeats", self.repeats], he / "build",
                   self.timeout, self.env)
        out = run.stdout
        case.output_tail = out[-1500:]
        errors = [float(v) for v in re.findall(r"max_abs_error=([0-9.eE+-]+|nan|inf)", out)]
        levels = [int(v) for v in re.findall(r"levels consumed (\d+)\)", out)]
        case.max_abs_error = max(errors) if errors else None
        case.levels = max(levels) if levels else None
        if m := re.search(r"execution_time_\(ms\):\s*([0-9.eE+-]+)", out):
            case.exec_time_ms = float(m.group(1))
        if m := re.search(r"fhe_eval_median_\(ms\):\s*([0-9.eE+-]+)", out):
            case.eval_time_ms = float(m.group(1))
        case.passed = run.returncode == 0 and case.max_abs_error is not None
        if not case.passed:
            if case.max_abs_error is None:
                raise _Failure("run_error", f"the encrypted program failed on case {case.name}:\n" + out[-2000:])
            raise _Failure("wrong_output", f"wrong result on case {case.name} ({case.slot_count} slots): max abs "
                           f"error {case.max_abs_error:.3g} > tolerance {task.tolerance:g}\n" + out[-1500:])

    # ------------------------------------------------------------------- entry
    def program_dir(self, task: Task, tag: str, case: str) -> Path:
        """Where a kernel's generated program for one case is built (he/build/main)."""
        return self.work / "eval" / task.name / tag / case

    def evaluate(self, task: Task, source: str, heldout: bool = False, static_only: bool = False,
                 tag: str = "current") -> Evaluation:
        """Full evaluation; static_only stops after compiling the dense case (pre-screen).
        tag names the build directories, so several kernels' programs can be kept and re-timed."""
        start = time.time()
        if reason := check_kernel(source):
            return Evaluation("rejected", message=reason)
        slots = self.prepare(task)
        if errors := self._build_driver(source):
            return Evaluation("compile_error", message="C++ compilation of the kernel failed:\n" + errors,
                              seconds=time.time() - start)
        ev = Evaluation("ok")
        names = ["dense", "fixed"] + ([f"heldout{s}" for s in HELDOUT_SEEDS] if heldout else [])
        try:
            for name in names:
                run_dir = self.program_dir(task, tag, name)
                run_dir.mkdir(parents=True, exist_ok=True)
                _sync(self.case_dir(task, name) / "fhe_io_example.txt", run_dir / "fhe_io_example.txt")
                if err := self._compile_case(run_dir, slots[name], ev, quantify=name == "dense"):
                    raise _Failure("chehab_error", f"CHEHAB rejected the kernel (case {name}):\n{err}")
                if name == "dense":
                    info: StaticInfo = analyze((run_dir / "stage" / "he" / "_gen_he_fhe.cpp").read_text())
                    ev.static, ev.ops = info.summary(), info.ops
                    if static_only:
                        break
                case = CaseResult(name, slots[name])
                ev.cases.append(case)
                self._build_and_run(task, run_dir, case)
        except _Failure as f:
            ev.status, ev.message = f.status, f.message
            ev.seconds = time.time() - start
            return ev
        ev.correct = not static_only
        ev.seconds = time.time() - start
        return ev


class _Failure(Exception):
    def __init__(self, status: str, message: str):
        super().__init__(message)
        self.status, self.message = status, message
