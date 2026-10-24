#!/usr/bin/env python3
"""Neutral scorer: compares kernels for one task under the same protocol.

Every kernel is first fully evaluated (fixed, dense and held-out cases must
pass). Then the generated dense programs are timed in interleaved rounds
(round r runs every kernel once, in a rotated order), so drift of the machine
affects all kernels alike. Each run reports the median of `repeats` timed fhe()
calls after one warmup; the score is the median over rounds.

  python3 -m llm_loop.score --task chebyshev_gelu_d7 --kernel mine=path/to/kernel.cpp [--kernel ...]

The baseline is always included. Kernels from any loop (this one or a
teammate's) can be scored, as long as they use the `void fhe()` format.
"""
import argparse
import json
import re
import statistics
from pathlib import Path
from typing import Dict

from llm_loop.evaluate import Evaluator, _run
from llm_loop.tasks import TASKS, Task


def time_program(evaluator: Evaluator, task: Task, tag: str, repeats: int) -> float:
    build = evaluator.program_dir(task, tag, "dense") / "he" / "build"
    out = _run(["./main", "--tol", repr(task.tolerance), "--repeats", repeats], build, evaluator.timeout,
               evaluator.env)
    m = re.search(r"fhe_eval_median_\(ms\):\s*([0-9.eE+-]+)", out.stdout)
    if out.returncode != 0 or not m:
        raise RuntimeError(f"{task.name}/{tag}: timing run failed\n{out.stdout[-1500:]}")
    return float(m.group(1))


def score(task: Task, kernels: Dict[str, str], evaluator: Evaluator, rounds: int = 5, repeats: int = 10) -> dict:
    kernels = {"baseline": task.baseline(), **kernels}
    results = {}
    for tag, source in kernels.items():
        ev = evaluator.evaluate(task, source, heldout=True, tag=tag)
        results[tag] = {"status": ev.status, "correct": ev.correct, "message": ev.message[:500],
                        "static": ev.static, "depth": ev.depth, "xdepth": ev.xdepth,
                        "max_abs_error": ev.max_abs_error, "times_ms": []}
    timed = [t for t in kernels if results[t]["correct"]]
    for r in range(rounds if timed else 0):
        shift = r % len(timed)
        for tag in timed[shift:] + timed[:shift]:
            results[tag]["times_ms"].append(time_program(evaluator, task, tag, repeats))
    base = statistics.median(results["baseline"]["times_ms"]) if results["baseline"]["times_ms"] else None
    for tag in timed:
        times = results[tag]["times_ms"]
        results[tag].update(median_ms=statistics.median(times), min_ms=min(times), max_ms=max(times),
                            speedup=base / statistics.median(times) if base else None)
    return {"task": task.name, "rounds": rounds, "repeats": repeats, "kernels": results}


def main():
    p = argparse.ArgumentParser(description=__doc__.split("\n\n")[0])
    p.add_argument("--task", required=True, choices=sorted(TASKS))
    p.add_argument("--kernel", action="append", default=[], help="label=path to a kernel file")
    p.add_argument("--rounds", type=int, default=5)
    p.add_argument("--repeats", type=int, default=10)
    p.add_argument("--output", type=Path)
    args = p.parse_args()
    kernels = {}
    for spec in args.kernel:
        label, path = spec.split("=", 1)
        kernels[label] = Path(path).read_text()
    result = score(TASKS[args.task], kernels, Evaluator(), args.rounds, args.repeats)
    for tag, r in result["kernels"].items():
        print(f"{tag:<20} {r['status']:<13} median={r.get('median_ms')} speedup={r.get('speedup')} {r['static']}")
    if args.output:
        args.output.write_text(json.dumps(result, indent=1) + "\n")


if __name__ == "__main__":
    main()
