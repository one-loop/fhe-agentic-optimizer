"""Final speedup measurement, shared with Umair's loop.

Same protocol as his neutral re-scoring (llm_loop/rescore.py on umair/llm-loop),
so both agents' final numbers are measured the same way:
  - the baseline and every distinct final kernel of a task, in one session;
  - 1 warmup round, then 10 measured rounds; each round runs every program once
    as a separate process (`./main --repeats 10`), round-robin, the starting
    program rotating by one each round;
  - a program's time = median of its per-process fhe() medians;
  - speedup = baseline time / kernel time.
Like his, a timing process that fails its tolerance is recorded, not fatal.

Correctness is not checked here; programs come from evaluate() + snapshot().
"""
import re
import statistics
from pathlib import Path
from typing import Dict

from evaluate import _run

WARMUP, MEASURED, EVAL_REPEATS = 1, 10, 10


def normalize(source: str) -> str:
    """Source without comments or whitespace: identical kernels are measured once."""
    code = re.sub(r"//[^\n]*", "", re.sub(r"/\*.*?\*/", " ", source, flags=re.S))
    return re.sub(r"\s+", "", code)


def run_program(dest: Path, tolerance: float, repeats: int = EVAL_REPEATS, timeout: float = 900) -> dict:
    """One process of a program kept by evaluate.snapshot(): its fhe() median and whether it passed."""
    out = _run(["./main", "--tol", repr(tolerance), "--repeats", str(repeats)], dest / "build", timeout).stdout
    m = re.search(r"fhe_eval_median_\(ms\):\s*([0-9.eE+-]+)", out)
    errors = [float(v) for v in re.findall(r"max_abs_error=([0-9.eE+-]+|nan|inf)", out)]
    return {"ms": float(m.group(1)) if m else None, "passed": "PASS" in out,
            "max_abs_error": max(errors) if errors else None}


def score_programs(task, programs: Dict[str, Path], baseline: str = "baseline") -> Dict[str, dict]:
    """{label: program dir} -> {label: {"ms", "speedup", "processes", "all_passed", "max_abs_error"}}."""
    labels = list(programs)
    samples = {label: [] for label in labels}
    for i in range(WARMUP + MEASURED):
        shift = i % len(labels)
        for label in labels[shift:] + labels[:shift]:
            s = run_program(programs[label], task.tolerance)
            if i >= WARMUP:
                samples[label].append(s)
    times = {label: [s["ms"] for s in samples[label] if s["ms"] is not None] for label in labels}
    base = statistics.median(times[baseline])
    out = {}
    for label in labels:
        ms = statistics.median(times[label]) if times[label] else None
        errors = [s["max_abs_error"] for s in samples[label] if s["max_abs_error"] is not None]
        out[label] = {"ms": ms, "speedup": base / ms if ms else None, "processes": len(times[label]),
                      "all_passed": all(s["passed"] for s in samples[label]),
                      "max_abs_error": max(errors) if errors else None}
    return out
