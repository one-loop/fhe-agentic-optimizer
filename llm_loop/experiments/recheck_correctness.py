#!/usr/bin/env python3
"""Correctness re-check of every final kernel with the full held-out set; no timing.

For every task: the baseline, the final kernel of every run of this loop, and the
kernels selected by the team's loop on shared/llm-feedback-loop. A baseline failure
means the test itself is wrong, not the kernel. Kernels are only read; a kernel that
fails is reported and never replaced.

  python3 -m llm_loop.experiments.recheck_correctness --out llm_loop/results/correctness_recheck
"""
import argparse
import csv
import json
from concurrent.futures import ThreadPoolExecutor
from datetime import datetime
from pathlib import Path

from llm_loop.evaluate import WORK, Evaluator
from llm_loop.experiments.rescore_teammates import BRANCH, team_kernels
from llm_loop.loop import git_state, normalize
from llm_loop.rescore import discover, sha1
from llm_loop.tasks import HELDOUT_SEEDS, REPO, TASKS

RUN_SETS = ["phase2-pilot", "phase2-chebyshev-rest", "phase2-hints-full", "phase2-static-feedback",
            "phase2-quad-batchnorm", "phase2-sonnet", "phase2-nn-haiku", "phase2-nn-sonnet",
            "phase2-sonnet-quad-batchnorm", "phase2-opus", "phase2-opus-quad-batchnorm", "phase2-nn-opus",
            "phase2-opus-focus", "phase2-nn-opus-focus", "phase2-others-haiku", "phase2-others-sonnet",
            "phase2-others-opus"]
FIELDS = ["task", "owner", "kernel", "sha1", "used_by", "status", "correct", "tolerance", "max_abs_error",
          "worst_case", "cases_passed", "message"]


def finals(task):
    """(label, source) of every final, identical kernels merged; the baseline comes first."""
    kernels = {sha1(normalize(TASKS[task].baseline())): {"source": TASKS[task].baseline(), "used_by": ["baseline"]}}
    for root in RUN_SETS:
        for r in discover(REPO / "llm_loop" / "runs" / root):
            if r["task"] == task:
                k = kernels.setdefault(sha1(normalize(r["best_source"])), {"source": r["best_source"], "used_by": []})
                k["used_by"].append(f"{root}/{r['strategy']}/run{r['run']}")
    for r in team_kernels(task):
        k = kernels.setdefault(sha1(normalize(r["best_source"])), {"source": r["best_source"], "used_by": []})
        k["used_by"].append(f"team/{r['strategy']}")
    return kernels


def check_task(task_name, work):
    task, rows = TASKS[task_name], []
    evaluator = Evaluator(work=work / task_name, repeats=1)
    for i, (key, k) in enumerate(finals(task_name).items()):
        ev = evaluator.evaluate(task, k["source"], heldout=True, tag=f"k{i}")
        errors = [c for c in ev.cases if c.max_abs_error is not None]
        worst = max(errors, key=lambda c: c.max_abs_error) if errors else None
        rows.append({"task": task_name, "owner": task.owner, "kernel": f"k{i}", "sha1": key,
                     "used_by": " ".join(k["used_by"]), "status": ev.status, "correct": ev.correct,
                     "tolerance": task.tolerance, "max_abs_error": worst.max_abs_error if worst else "",
                     "worst_case": worst.name if worst else "", "cases_passed": sum(c.passed for c in ev.cases),
                     "message": ev.message.split("\n")[0][:200]})
        print(f"   {task_name} k{i} {ev.status} err={rows[-1]['max_abs_error']} {k['used_by'][:2]}", flush=True)
    return rows


def main():
    p = argparse.ArgumentParser(description=__doc__.split("\n\n")[0])
    p.add_argument("--out", required=True)
    p.add_argument("--tasks", nargs="*", default=list(TASKS))
    p.add_argument("--jobs", type=int, default=4, help="tasks at once; safe because nothing is timed")
    p.add_argument("--work", default=str(WORK / "recheck"))
    args = p.parse_args()
    out, work = Path(args.out), Path(args.work).resolve()
    out.mkdir(parents=True, exist_ok=True)
    with ThreadPoolExecutor(args.jobs) as pool:
        rows = [row for rs in pool.map(lambda t: check_task(t, work), args.tasks) for row in rs]
    with open(out / "recheck.csv", "w", newline="") as f:
        w = csv.DictWriter(f, FIELDS)
        w.writeheader()
        w.writerows(rows)
    failed = [r for r in rows if not r["correct"]]
    lines = ["# Correctness re-check of final kernels", "",
             f"{len(rows)} distinct kernels, {len(rows) - len(failed)} correct, {len(failed)} failed. "
             f"Cases: fixed, dense, held-out {', '.join(map(str, HELDOUT_SEEDS))}. No timing.", "",
             "| task | kernel | used by | status | message |", "|---|---|---|---|---|"]
    lines += [f"| {r['task']} | {r['kernel']} | {r['used_by']} | {r['status']} | {r['message']} |" for r in failed]
    (out / "recheck.md").write_text("\n".join(lines) + "\n")
    (out / "meta.json").write_text(json.dumps({"git": git_state(), "date": datetime.now().isoformat(timespec="seconds"),
                                               "team_branch": BRANCH, "heldout_seeds": HELDOUT_SEEDS}, indent=1))
    print(f"wrote {out}: {len(rows)} kernels, {len(failed)} failed")


if __name__ == "__main__":
    main()
