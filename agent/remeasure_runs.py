#!/usr/bin/env python3
"""Re-time earlier runs' best kernels the way the loop now reports speedups, so runs made
before the interleaved remeasurement can be compared with newer ones. No model calls.

  python agent/remeasure_runs.py --set api:..20261024 --set cli:20261024..20261024-1506 --out agent/results/remeasured.csv

Each --set is name:since..until (either end may be empty) over run timestamps (latest run per task in the window).
For every task, the baseline is compiled once; each set's best kernel whose circuit differs
from the baseline is compiled and timed against it in alternating batches. A best kernel with
the baseline's circuit counts as x1.00.
"""
import argparse
import csv
import shutil
import sys
import tempfile
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
from collect import load_runs  # noqa: E402
from evaluate import evaluate, snapshot  # noqa: E402
from loop import interleaved  # noqa: E402
from selection import FINAL_BATCHES, confirm, fingerprint  # noqa: E402
from tasks import TASKS  # noqa: E402


def main():
    p = argparse.ArgumentParser(description=__doc__.split("\n\n")[0])
    p.add_argument("--set", action="append", required=True, help="name:since..until")
    p.add_argument("--out", required=True)
    p.add_argument("--seal-prefix", required=True)
    p.add_argument("--repeats", type=int, default=10)
    args = p.parse_args()

    sets = {}
    for spec in args.set:
        name, window = spec.split(":", 1)
        since, _, until = window.partition("..")
        sets[name], _ = load_runs(False, since, until or "~")
    tasks = sorted(set().union(*sets.values()))
    rows, tmp = [], Path(tempfile.mkdtemp(prefix="remeasure-"))
    for t in tasks:
        task = TASKS[t]
        evaluate(task, task.baseline(), args.seal_prefix, args.repeats)  # builds the baseline program
        snapshot(task, tmp / t / "baseline")
        for name, runs in sets.items():
            r = runs.get(t)
            if r is None:
                continue
            row = {"task": t, "set": name, "run": str(r["_path"].name), "structural_change": False,
                   "speedup": 1.0, "wins": "", "batches": "", "confirmed": ""}
            if fingerprint(r["best"]) != fingerprint(r["baseline"]):
                ev = evaluate(task, (r["_path"] / "best.cpp").read_text(), args.seal_prefix, args.repeats)
                if not ev.correct:
                    raise RuntimeError(f"{t} {name}: best kernel no longer correct: {ev.status}")
                snapshot(task, tmp / t / name)
                c = confirm(*interleaved(task, tmp / t / name, tmp / t / "baseline", FINAL_BATCHES, args.repeats))
                row.update(structural_change=True, speedup=round(c["speedup"], 3), wins=c["wins"],
                           batches=c["batches"], confirmed=c["confirmed"])
            print(row, flush=True)
            rows.append(row)
    shutil.rmtree(tmp)
    with open(args.out, "w", newline="") as f:
        w = csv.DictWriter(f, fieldnames=list(rows[0]))
        w.writeheader()
        w.writerows(rows)


if __name__ == "__main__":
    main()
