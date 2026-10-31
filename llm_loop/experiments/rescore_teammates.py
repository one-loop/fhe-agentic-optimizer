#!/usr/bin/env python3
"""Head-to-head neutral measurement of final kernels from different loops.

For each of Umair's 12 tasks, one session (rescore.rescore_task) measures the
baseline, the final kernel of every no-hints run of this loop, and the kernels
selected by the team's loop on shared/llm-feedback-loop (api_haiku, sonnet_v1,
sonnet_v2, as listed in its agent/results/remeasured.csv). Every kernel goes
through the same correctness checks (fixed, dense, held-out) and interleaved
timing. Kernels are read from git; nothing in the team's branch changes.

This compares kernels, not search methods: the loops differ in model, number of
runs and search budget.

  python3 -m llm_loop.experiments.rescore_teammates --out llm_loop/results/phase2_teammates_rescore
"""
import argparse
import csv
import json
import platform
import statistics
import subprocess
from datetime import datetime
from pathlib import Path

from llm_loop.evaluate import WORK, Evaluator
from llm_loop.loop import git_state
from llm_loop.rescore import discover, rescore_task
from llm_loop.tasks import REPO, TASKS

BRANCH = "shared/llm-feedback-loop"
SETS = ["api_haiku", "sonnet_v1", "sonnet_v2"]
MY_RUNS = ["phase2-pilot", "phase2-chebyshev-rest", "phase2-quad-batchnorm"]
MY_TASKS = [n for n, t in TASKS.items() if t.owner == "Umair"]


def git(*args):
    return subprocess.run(["git", *args], cwd=REPO, capture_output=True, text=True, check=True).stdout


def team_kernels(task):
    """The team's selected kernel per configuration, per their remeasured.csv."""
    rows = csv.DictReader(git("show", f"{BRANCH}:agent/results/remeasured.csv").splitlines())
    out = []
    for r in rows:
        if r["task"] == task and r["set"] in SETS:
            out.append({"task": task, "strategy": r["set"], "run": 1, "team_run": r["run"],
                        "best_source": git("show", f"{BRANCH}:agent/runs/{task}/{r['run']}/best.cpp")})
    return out


def my_kernels(task):
    out = []
    for root in MY_RUNS:
        out += [r for r in discover(REPO / "llm_loop" / "runs" / root) if r["task"] == task]
    return out


def fmt(values):
    return "-" if not values else " ".join(f"x{v:.3f}" for v in values)


def main():
    p = argparse.ArgumentParser(description=__doc__.split("\n\n")[0])
    p.add_argument("--tasks", nargs="+", default=MY_TASKS)
    p.add_argument("--out", type=Path, required=True)
    p.add_argument("--work", type=Path, default=WORK / "teammates")
    args = p.parse_args()
    started = datetime.now().isoformat(timespec="seconds")
    evaluator = Evaluator(work=args.work)
    args.out.mkdir(parents=True, exist_ok=True)
    rows, sources = [], {}
    for name in args.tasks:
        mine, team = my_kernels(name), team_kernels(name)
        print(f"== {name}: {len(mine)} of my finals, {len(team)} team kernels", flush=True)
        sources[name] = {r["strategy"]: r["team_run"] for r in team}
        rows += rescore_task(TASKS[name], mine + team, evaluator)
        with open(args.out / "kernels.csv", "w", newline="") as f:  # kept current: survives an interruption
            w = csv.DictWriter(f, fieldnames=list(rows[0]))
            w.writeheader()
            w.writerows(rows)

    def scores(task, label):
        return [(r, float(r["speedup"]) if r["speedup"] != "" else None) for r in rows
                if r["task"] == task and any(u.startswith(label + "/") for u in r["runs"].split())]

    lines = ["# Head-to-head neutral measurement of final kernels", "",
             f"Commit {git_state()['commit'][:10]}; team kernels from {BRANCH} at {git('rev-parse', '--short', BRANCH).strip()}. "
             "One session per task; correctness on fixed, dense and held-out cases; speedup = baseline eval-only "
             "median / kernel eval-only median. Compares kernels, not search methods: models, runs and budgets differ.",
             "", "| task | baseline (ms) | my archive (no hints), median [all] | my linear (no hints), median [all] | "
             + " | ".join(SETS) + " |", "|---|---:|---|---|" + "---:|" * len(SETS)]
    for name in args.tasks:
        base = next(r for r in rows if r["task"] == name and r["kernel"] == "baseline")
        cells = []
        for label in ("archive", "linear"):
            vals = []
            for r in rows:
                if r["task"] == name:
                    n = sum(u.startswith(label + "/") for u in r["runs"].split())
                    vals += [float(r["speedup"]) if r["speedup"] != "" else float("nan")] * n
            cells.append(f"x{statistics.median(vals):.3f} [{fmt(vals)}]" if vals else "-")
        for s in SETS:
            found = scores(name, s)
            if not found:
                # identical to the baseline circuit text: measured with the baseline
                cells.append("-")
            else:
                r, v = found[0]
                cells.append(f"x{v:.3f}" if v is not None else f"FAILED: {r['message'][:60]}")
        lines.append(f"| {name} | {float(base['eval_median_ms']):.1f} | " + " | ".join(cells) + " |")
    (args.out / "headtohead.md").write_text("\n".join(lines) + "\n")
    (args.out / "meta.json").write_text(json.dumps({
        "started": started, "finished": datetime.now().isoformat(timespec="seconds"), "git": git_state(),
        "team_branch": BRANCH, "team_commit": git("rev-parse", BRANCH).strip(), "team_runs": sources,
        "machine": f"{platform.system()} {platform.machine()}"}, indent=1) + "\n")
    print(f"wrote {args.out}")


if __name__ == "__main__":
    main()
