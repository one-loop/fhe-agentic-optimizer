#!/usr/bin/env python3
"""Step 1 check: every baseline kernel reproduces its benchmark.

For each task:
  - the oracle reproduces the generator's expected outputs (fixed and dense),
    which validates the oracle used for the held-out cases;
  - the baseline kernel compiles through the loop's driver and passes the
    fixed, dense and held-out cases within the task's tolerance;
  - levels, (depth, xdepth) and every operation count of the generated dense
    code equal the Phase 1 record of the benchmark (results/*.csv).

  python3 -m llm_loop.check_baselines [--tasks ...] [--jobs 4] [--records extra.csv ...]

Timings measured here are not used (jobs run in parallel).
"""
import argparse
import csv
import json
from concurrent.futures import ProcessPoolExecutor
from pathlib import Path

import numpy as np

from llm_loop import iofile
from llm_loop.evaluate import WORK, Evaluator
from llm_loop.tasks import REPO, TASKS

RECORD_FILES = [REPO / "results" / f for f in
                ("unified_baseline.csv", "ckks_add_mul_relu.csv", "ckks_extract_embedding.csv")]
COMPARED = ["op_add", "op_add_plain", "op_sub", "op_sub_plain", "op_multiply", "op_multiply_plain", "op_square",
            "op_relinearize", "op_rescale", "op_mod_switch", "op_rotate", "op_negate"]


def load_records(extra):
    records = {}
    for path in [*RECORD_FILES, *extra]:
        for row in csv.DictReader(open(path)):
            if row.get("scheme") == "CKKS" and row.get("case") == "dense":
                records.setdefault(row["benchmark"], row)
    return records


def oracle_error(task, evaluator: Evaluator) -> float:
    worst = 0.0
    for case in ("fixed", "dense"):
        io = iofile.read(evaluator.case_dir(task, case) / "fhe_io_example.txt")
        expected = task.oracle(io)
        for label, values in io.outputs.items():
            scale = max(1.0, float(np.max(np.abs(values))))
            worst = max(worst, float(np.max(np.abs(expected[label][:len(values)] - values))) / scale)
    return worst


def check_group(names, worker, records):
    evaluator = Evaluator(work=WORK / f"check{worker}", repeats=3)
    results = []
    for name in names:
        task = TASKS[name]
        evaluator.prepare(task)
        ev = evaluator.evaluate(task, task.baseline(), heldout=True)
        row = {"task": name, "owner": task.owner, "status": ev.status, "message": ev.message[:300],
               "oracle_rel_error": oracle_error(task, evaluator), "max_abs_error": ev.max_abs_error,
               "tolerance": task.tolerance, "levels": ev.static.get("levels"),
               "case_levels": {c.name: c.levels for c in ev.cases}, "depth": ev.depth, "xdepth": ev.xdepth,
               "terms": ev.terms, "static": ev.static, "mismatches": []}
        rec = records.get(task.record)
        if rec is None:
            row["mismatches"].append("no Phase 1 record")
        else:
            for f in COMPARED:
                if int(rec[f] or 0) != ev.ops.get(f, 0):
                    row["mismatches"].append(f"{f}: record {rec[f]}, baseline {ev.ops.get(f, 0)}")
            if rec["ckks_levels_consumed"] and int(rec["ckks_levels_consumed"]) != row["levels"]:
                row["mismatches"].append(f"levels: record {rec['ckks_levels_consumed']}, static {row['levels']}")
            if rec["depth"] and (int(rec["depth"]), int(rec["xdepth"])) != (ev.depth, ev.xdepth):
                row["mismatches"].append(f"(depth, xdepth): record ({rec['depth']}, {rec['xdepth']}), "
                                         f"baseline ({ev.depth}, {ev.xdepth})")
        if any(c.levels is not None and c.levels != row["levels"] for c in ev.cases if c.name == "dense"):
            row["mismatches"].append("static levels differ from the run's chain index")
        row["ok"] = ev.correct and not row["mismatches"] and row["oracle_rel_error"] < 1e-9
        results.append(row)
        print(f"{name:<26} {'OK' if row['ok'] else 'FAIL':<5} {ev.status:<13} err={ev.max_abs_error} "
              f"levels={row['levels']} {'; '.join(row['mismatches'])}", flush=True)
    return results


def main():
    p = argparse.ArgumentParser(description=__doc__.split("\n\n")[0])
    p.add_argument("--tasks", nargs="+", default=list(TASKS))
    p.add_argument("--jobs", type=int, default=4)
    p.add_argument("--records", nargs="*", default=[], help="extra run_benchmarks.py CSVs (e.g. sigmoid_d5)")
    p.add_argument("--output", type=Path, default=Path(__file__).resolve().parent / "results" / "baseline_check.json")
    args = p.parse_args()
    records = load_records(args.records)
    groups = [args.tasks[i::args.jobs] for i in range(args.jobs)]
    with ProcessPoolExecutor(args.jobs) as pool:
        results = [r for rs in pool.map(check_group, groups, range(args.jobs), [records] * args.jobs) for r in rs]
    results.sort(key=lambda r: list(TASKS).index(r["task"]))
    args.output.parent.mkdir(parents=True, exist_ok=True)
    args.output.write_text(json.dumps(results, indent=1) + "\n")
    bad = [r["task"] for r in results if not r["ok"]]
    print(f"{len(results) - len(bad)}/{len(results)} baselines reproduce their benchmark"
          + (f"; failing: {', '.join(bad)}" if bad else ""))
    raise SystemExit(1 if bad else 0)


if __name__ == "__main__":
    main()
