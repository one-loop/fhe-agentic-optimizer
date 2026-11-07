#!/usr/bin/env python3
"""Repeated correctness check: evaluate chosen final kernels, and their baselines, several times on all
cases (fixed, dense, held-out). CKKS encryption noise is random, so a kernel close to the tolerance can
pass once and fail later. No model calls, no timing.

  python3 -m llm_loop.experiments.repeat_correctness --runs llm_loop/runs/phase2-opus-focus \
      --tasks chebyshev_gelu_d7 chebyshev_sigmoid_d7 --run 1 --repeats 10 --out llm_loop/results/phase2_opus_focus
"""
import argparse
import csv
from pathlib import Path

from llm_loop.evaluate import WORK, Evaluator
from llm_loop.tasks import TASKS

FIELDS = ["task", "kernel", "repeat", "status", "correct", "tolerance", "max_abs_error", "worst_case", "message"]


def main():
    p = argparse.ArgumentParser(description=__doc__.split("\n\n")[0])
    p.add_argument("--runs", type=Path, required=True, help="root holding archive/<task>/run<k>/best.cpp")
    p.add_argument("--tasks", nargs="+", required=True)
    p.add_argument("--run", type=int, required=True)
    p.add_argument("--repeats", type=int, default=10)
    p.add_argument("--out", type=Path, required=True)
    p.add_argument("--work", type=Path, default=WORK / "repeat-correctness")
    args = p.parse_args()

    rows = []
    for name in args.tasks:
        task = TASKS[name]
        kernels = {"baseline": task.baseline(),
                   f"archive/run{args.run}": (args.runs / "archive" / name / f"run{args.run}" / "best.cpp").read_text()}
        evaluator = Evaluator(work=args.work / name, repeats=1)
        for label, source in kernels.items():
            for i in range(1, args.repeats + 1):
                ev = evaluator.evaluate(task, source, heldout=True, tag=label.replace("/", "_"))
                errs = [c for c in ev.cases if c.max_abs_error is not None]
                worst = max(errs, key=lambda c: c.max_abs_error) if errs else None
                rows.append({"task": name, "kernel": label, "repeat": i, "status": ev.status, "correct": ev.correct,
                             "tolerance": task.tolerance, "max_abs_error": worst.max_abs_error if worst else "",
                             "worst_case": worst.name if worst else "", "message": ev.message.split("\n")[0][:200]})
                print(f"   {name} {label} #{i} {ev.status} err={rows[-1]['max_abs_error']}", flush=True)

    args.out.mkdir(parents=True, exist_ok=True)
    with open(args.out / "repeat_correctness.csv", "w", newline="") as f:
        w = csv.DictWriter(f, fieldnames=FIELDS)
        w.writeheader()
        w.writerows(rows)
    md = ["# Repeated correctness check", "",
          f"Each kernel evaluated {args.repeats} times on the fixed, dense and held-out cases (fresh encryption",
          "each time). Written by `experiments/repeat_correctness.py`. No timing.", "",
          "| task | kernel | passed | worst max abs error | tolerance |", "|---|---|---:|---:|---:|"]
    for name in args.tasks:
        for label in ("baseline", f"archive/run{args.run}"):
            rs = [r for r in rows if r["task"] == name and r["kernel"] == label]
            errs = [r["max_abs_error"] for r in rs if r["max_abs_error"] != ""]
            md.append(f"| {name} | {label} | {sum(r['correct'] is True for r in rs)} / {len(rs)} | "
                      f"{max(errs):.3g} | {TASKS[name].tolerance:g} |" if errs else
                      f"| {name} | {label} | {sum(r['correct'] is True for r in rs)} / {len(rs)} | - | "
                      f"{TASKS[name].tolerance:g} |")
    (args.out / "repeat_correctness.md").write_text("\n".join(md) + "\n")
    print(f"wrote {args.out / 'repeat_correctness.md'}")


if __name__ == "__main__":
    main()
