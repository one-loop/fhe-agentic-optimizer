#!/usr/bin/env python3
"""Re-score several runs' final kernels in one session, with the shared protocol of
score.py (the same as Umair's neutral re-scoring), so runs made at different times
or by different loops can be compared on equal terms. No model calls.

  python agent/remeasure_runs.py --set api:..20261024 --set cli:20261024..20261024-1506 --out agent/results/rescored.csv

Each --set is name:since..until (either end may be empty) over run timestamps (latest run per task in the window).
Per task: every set's best.cpp is checked for correctness on the task's cases; the baseline and every
distinct correct kernel (identical sources are measured once) are timed together by score.py.
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
from score import normalize, score_programs  # noqa: E402
from selection import fingerprint  # noqa: E402
from tasks import TASKS  # noqa: E402


def main():
    p = argparse.ArgumentParser(description=__doc__.split("\n\n")[0])
    p.add_argument("--set", action="append", required=True, help="name:since..until")
    p.add_argument("--out", required=True)
    p.add_argument("--seal-prefix", required=True)
    p.add_argument("--repeats", type=int, default=10, help="timed fhe() runs in the correctness evaluation")
    p.add_argument("--tasks", default="", help="comma-separated task names to restrict to (default: all)")
    args = p.parse_args()

    sets = {}
    for spec in args.set:
        name, window = spec.split(":", 1)
        since, _, until = window.partition("..")
        sets[name], _ = load_runs(False, since, until or "~")
    tasks = sorted(set().union(*sets.values()))
    if args.tasks:
        tasks = [t for t in tasks if t in args.tasks.split(",")]
    rows, tmp = [], Path(tempfile.mkdtemp(prefix="rescore-"))
    for t in tasks:
        task = TASKS[t]
        base_src = task.baseline()
        evaluate(task, base_src, args.seal_prefix, args.repeats)  # builds the baseline program
        snapshot(task, tmp / t / "baseline")
        programs = {"baseline": tmp / t / "baseline"}
        label_of = {normalize(base_src): "baseline"}
        task_rows = []
        for name, runs in sets.items():
            r = runs.get(t)
            if r is None:
                continue
            src = (r["_path"] / "best.cpp").read_text()
            row = {"task": t, "set": name, "run": str(r["_path"].name),
                   "structural_change": fingerprint(r["best"]) != fingerprint(r["baseline"]),
                   "kernel": "", "correct": "", "speedup": "", "ms": "", "baseline_ms": "", "processes": "",
                   "all_timing_runs_passed": "", "max_abs_error": "", "note": ""}
            key = normalize(src)
            if key not in label_of:
                ev = evaluate(task, src, args.seal_prefix, args.repeats)
                if ev.correct:
                    label_of[key] = f"k{len(label_of)}"
                    snapshot(task, tmp / t / label_of[key])
                    programs[label_of[key]] = tmp / t / label_of[key]
                else:
                    label_of[key] = None
                    row.update(correct=False, note=f"{ev.status}: {ev.message.splitlines()[-1][:160]}")
            row["kernel"] = label_of[key] or ""
            task_rows.append(row)
        scores = score_programs(task, programs)
        for row in task_rows:
            s = scores.get(row["kernel"])
            if s:
                row.update(correct=True, speedup=round(s["speedup"], 3), ms=round(s["ms"], 3),
                           baseline_ms=round(scores["baseline"]["ms"], 3), processes=s["processes"],
                           all_timing_runs_passed=s["all_passed"], max_abs_error=s["max_abs_error"])
            print(row, flush=True)
        rows += task_rows
    shutil.rmtree(tmp)
    with open(args.out, "w", newline="") as f:
        w = csv.DictWriter(f, fieldnames=list(rows[0]))
        w.writeheader()
        w.writerows(rows)


if __name__ == "__main__":
    main()
