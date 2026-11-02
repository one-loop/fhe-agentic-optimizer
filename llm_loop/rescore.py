#!/usr/bin/env python3
"""Neutral re-scoring of loop runs, with run_benchmarks.py's methodology.

For each task, the baseline and the final kernel of every run are measured in
one session on this machine:
  - the final kernel is the run's best.cpp, the strategy's own selection within
    its budget; re-scoring measures it and never chooses a different attempt
    (identical kernels are measured once and shared);
  - correctness on the fixed, dense and held-out cases (task tolerance);
  - 3 CHEHAB compiles (median compile time, reported only), (depth, xdepth);
  - operation counts and levels from the generated code;
  - 1 warmup process, then 10 measured processes of ./main --repeats 10,
    interleaved round-robin across the task's kernels;
  - evaluation-only time = median of the per-process fhe() medians;
    end-to-end time = median of CHEHAB's execution_time_(ms);
    FHE error = max over every measured process and correctness case;
  - speedup = baseline eval median / kernel eval median, same session.
Approximation error (polynomial vs true activation, from the generator) is a
property of the task and is reported separately from FHE error.

  python3 -m llm_loop.rescore --runs llm_loop/runs/phase2-pilot --out llm_loop/results/phase2_pilot
"""
import argparse
import csv
import hashlib
import json
import math
import platform
import re
import statistics
import subprocess
import tempfile
from datetime import datetime
from pathlib import Path
from typing import Dict, List

from llm_loop.evaluate import WORK, Evaluator, _run
from llm_loop.loop import git_state, normalize
from llm_loop.tasks import TASKS, Task

WARMUP, MEASURED, EVAL_REPEATS, COMPILE_RUNS = 1, 10, 10, 3


def sha1(text: str) -> str:
    return hashlib.sha1(text.encode()).hexdigest()


def discover(root: Path) -> List[dict]:
    """Every completed run under <root>/<strategy>/<task>/run<k>/."""
    runs = []
    for result in sorted(root.glob("*/*/run*/result.json")):
        r = json.loads(result.read_text())
        r["best_source"] = (result.parent / "best.cpp").read_text()
        r["dir"] = str(result.parent)
        runs.append(r)
    return runs


def approximation_error(task: Task) -> Dict[str, float]:
    """The generator's approximation error (polynomial vs true activation), per case."""
    out = {}
    for case in task.cases:
        with tempfile.TemporaryDirectory() as d:
            text = _run(case.command, d, 300).stdout
        if m := re.search(r"on these points: ([0-9.eE+-]+)", text):
            out[f"approx_error_{case.name}"] = float(m.group(1))
        if m := re.search(r"max_fit_error \(fitting grid[^)]*\): ([0-9.eE+-]+)", text):
            out["max_fit_error"] = float(m.group(1))
    return out


def time_once(evaluator: Evaluator, task: Task, tag: str) -> dict:
    build = evaluator.program_dir(task, tag, "dense") / "he" / "build"
    out = _run(["./main", "--tol", repr(task.tolerance), "--repeats", EVAL_REPEATS], build, evaluator.timeout,
               evaluator.env).stdout
    get = lambda pat: [float(v) for v in re.findall(pat, out)]
    return {"eval": get(r"fhe_eval_median_\(ms\):\s*([0-9.eE+-]+)"), "exec": get(r"execution_time_\(ms\):\s*([0-9.eE+-]+)"),
            "err": get(r"max_abs_error=([0-9.eE+-]+)"), "levels": get(r"levels consumed (\d+)\)"),
            "pass": "PASS" in out}


def rescore_task(task: Task, runs: List[dict], evaluator: Evaluator) -> List[dict]:
    kernels: Dict[str, dict] = {}
    base_src = task.baseline()
    kernels[sha1(normalize(base_src))] = {"source": base_src, "label": "baseline", "runs": []}
    for r in runs:
        key = sha1(normalize(r["best_source"]))
        kernels.setdefault(key, {"source": r["best_source"], "label": f"k{len(kernels)}", "runs": []})
        kernels[key]["runs"].append(f"{r['strategy']}/run{r['run']}")
    for key, k in kernels.items():
        ev = evaluator.evaluate(task, k["source"], heldout=True, tag=k["label"])
        k["ev"] = ev
        k["compile_ms"] = evaluator.compile_times(task, k["label"], COMPILE_RUNS) if ev.correct else []
        print(f"   {task.name} {k['label']:<8} {ev.status:<12} runs={k['runs']}", flush=True)
    # The baseline is always timed, as the speedup reference, even if it fails a case (reported as correct=False).
    timed = [k for k in kernels.values() if k["ev"].correct or k["label"] == "baseline"]
    samples = {k["label"]: [] for k in timed}
    for i in range(WARMUP + MEASURED):
        shift = i % len(timed)
        for k in timed[shift:] + timed[:shift]:
            t = time_once(evaluator, task, k["label"])
            if i >= WARMUP:
                samples[k["label"]].append(t)
    approx = approximation_error(task)
    base_eval = statistics.median(s["eval"][0] for s in samples["baseline"])
    rows = []
    for key, k in kernels.items():
        ev, s = k["ev"], samples.get(k["label"], [])
        evals = [x["eval"][0] for x in s if x["eval"]]
        execs = [x["exec"][0] for x in s if x["exec"]]
        errors = [e for x in s for e in x["err"]] + [c.max_abs_error for c in ev.cases if c.max_abs_error is not None]
        row = {
            "task": task.name, "kernel": k["label"], "sha1": key, "runs": " ".join(k["runs"]),
            "status": ev.status, "message": ev.message.split("\n")[0][:200], "correct": ev.correct, "all_timing_runs_pass": all(x["pass"] for x in s) if s else "",
            "tolerance": task.tolerance, "fhe_max_abs_error": max(errors) if errors else "",
            "eval_median_ms": statistics.median(evals) if evals else "",
            "eval_min_ms": min(evals) if evals else "", "eval_max_ms": max(evals) if evals else "",
            "e2e_median_ms": statistics.median(execs) if execs else "",
            "speedup": base_eval / statistics.median(evals) if evals else "",
            "depth": ev.depth, "xdepth": ev.xdepth, "levels_static": ev.static.get("levels"),
            "levels_run": int(max(l for x in s for l in x["levels"])) if s and s[0]["levels"] else "",
            "ct_ct_mul": ev.static.get("ct_ct_mul"), "ct_pt_mul": ev.static.get("ct_pt_mul"),
            "align_mul": ev.static.get("align_mul"), "rescale": ev.static.get("rescale"),
            "mod_switch": ev.static.get("mod_switch"), "rotate": ev.static.get("rotate"),
            "add": ev.static.get("add"), "add_plain": ev.static.get("add_plain"),
            "compile_median_ms": statistics.median(k["compile_ms"]) if k["compile_ms"] else "",
            "compile_runs_ms": " ".join(f"{c:.3f}" for c in k["compile_ms"]),
            "measured_processes": len(evals), **approx,
        }
        rows.append(row)
    return rows


def fmt(value, spec: str) -> str:
    return "-" if value in ("", None) else format(value, spec)


def describe(values: List[float]) -> str:
    if not values:
        return "-"
    sd = statistics.stdev(values) if len(values) > 1 else 0.0
    return (f"median x{statistics.median(values):.3f}, mean x{statistics.mean(values):.3f}, "
            f"min x{min(values):.3f}, max x{max(values):.3f}, sd {sd:.3f}")


def report(out: Path, kernel_rows: List[dict], runs: List[dict], meta: dict) -> None:
    by_key = {(r["task"], r["sha1"]): r for r in kernel_rows}
    run_rows = []
    for r in runs:
        k = by_key[(r["task"], sha1(normalize(r["best_source"])))]
        f = r.get("failures", {})
        run_rows.append({
            "task": r["task"], "strategy": r["strategy"], "run": r["run"], "started_at": r.get("started_at", ""),
            "final_kernel": k["kernel"], "final_attempt": r["best_id"], "final_is_baseline": r["best_id"] == 0,
            "speedup": k["speedup"], "eval_median_ms": k["eval_median_ms"], "e2e_median_ms": k["e2e_median_ms"],
            "correct": k["correct"], "fhe_max_abs_error": k["fhe_max_abs_error"],
            "depth": k["depth"], "xdepth": k["xdepth"], "levels": k["levels_run"],
            "ct_ct_mul": k["ct_ct_mul"], "ct_pt_mul": k["ct_pt_mul"], "align_mul": k["align_mul"],
            "rescale": k["rescale"], "mod_switch": k["mod_switch"], "compile_median_ms": k["compile_median_ms"],
            "calls": r["calls"], "correct_attempts": r["correct_attempts"], "duplicates": r.get("duplicates", ""),
            "compile_failures": f.get("compile_error", 0) + f.get("rejected", 0),
            "chehab_rejections": f.get("chehab_error", 0), "build_failures": f.get("build_error", 0),
            "runtime_failures": f.get("run_error", 0), "wrong_output": f.get("wrong_output", 0),
            "heldout_failures": f.get("heldout_fail", 0), "no_code": f.get("no_code", 0),
            "predicted_slower": r.get("predicted_slower", ""),
            "input_tokens": r.get("tokens", {}).get("input", ""), "output_tokens": r.get("tokens", {}).get("output", ""),
            "thinking_tokens": r.get("tokens", {}).get("thinking", ""),
            "search_seconds": round(r.get("search_seconds", 0), 1), "models": " ".join(r.get("models_used", [])),
        })
    run_rows.sort(key=lambda x: (x["task"], x["strategy"], x["run"]))
    for name, rows in (("rescore.csv", kernel_rows), ("runs.csv", run_rows)):
        with open(out / name, "w", newline="") as fh:
            w = csv.DictWriter(fh, fieldnames=list(rows[0]))
            w.writeheader()
            w.writerows(rows)

    lines = ["# Phase-2 pilot: neutral re-scoring", "",
             f"Commit {meta['git']['commit'][:10]} ({'dirty' if meta['git']['dirty'] else 'clean'}), {meta['machine']}, "
             f"{meta['compiler']}, SEAL {meta['seal']}. Re-scored {meta['finished']}.", "",
             "Speedup = baseline eval-only median / kernel eval-only median, both from the same neutral session "
             f"({WARMUP} warmup + {MEASURED} measured processes of ./main --repeats {EVAL_REPEATS}, interleaved). "
             "The final kernel of each run is the strategy's own selection; re-scoring never re-selects. "
             "E2E = CHEHAB execution_time_(ms) (parse + encode + encrypt + fhe()). Compile time: median of 3, "
             "reported only.", "",
             "## Every run", "",
             "| task | strategy | run | final | speedup | eval (ms) | e2e (ms) | FHE err | depth | xdepth | levels | "
             "ct x ct | ct x pt | align | rescale | mod sw | C (ms) | correct / 8 | dup | failures | out tok | "
             "think tok | search (s) |",
             "|---|---|---:|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---|---:|---:|---:|"]
    for x in run_rows:
        labels = {"compile_failures": "compile", "chehab_rejections": "chehab", "build_failures": "build",
                  "runtime_failures": "runtime", "wrong_output": "wrong", "heldout_failures": "held-out",
                  "no_code": "no-code"}
        fails = ", ".join(f"{label} {x[k]}" for k, label in labels.items() if x[k]) or "-"
        lines.append(
            f"| {x['task']} | {x['strategy']} | {x['run']} | #{x['final_attempt']} ({x['final_kernel']}) | "
            f"x{fmt(x['speedup'], '.3f')} | {fmt(x['eval_median_ms'], '.1f')} | {fmt(x['e2e_median_ms'], '.1f')} | "
            f"{fmt(x['fhe_max_abs_error'], '.2e')} | "
            f"{x['depth']} | {x['xdepth']} | {x['levels']} | {x['ct_ct_mul']} | {x['ct_pt_mul']} | {x['align_mul']} | "
            f"{x['rescale']} | {x['mod_switch']} | {fmt(x['compile_median_ms'], '.2f')} | {x['correct_attempts']} / {x['calls']} | "
            f"{x['duplicates']} | {fails} | {x['output_tokens']} | {x['thinking_tokens']} | {x['search_seconds']} |")
    lines += ["", "## Per task and strategy (3 runs each)", "", "| task | strategy | runs | speedup |", "|---|---|---:|---|"]
    per_task: Dict[str, Dict[str, List[float]]] = {}
    for x in run_rows:
        if x["speedup"] != "":  # a final kernel that failed the neutral check has no speedup (reported above)
            per_task.setdefault(x["strategy"], {}).setdefault(x["task"], []).append(x["speedup"])
    for strategy, tasks in sorted(per_task.items()):
        for task, v in sorted(tasks.items()):
            lines.append(f"| {task} | {strategy} | {len(v)} | {describe(v)} |")
    lines += ["", "## Across tasks (geometric mean of the per-task mean speedups)", "",
              "| strategy | tasks | geometric mean |", "|---|---:|---:|"]
    for strategy, tasks in sorted(per_task.items()):
        means = [statistics.mean(v) for v in tasks.values()]
        lines.append(f"| {strategy} | {len(means)} | x{math.exp(statistics.mean(math.log(m) for m in means)):.3f} |")
    lines += ["", "## Approximation error (task property, separate from FHE error)", "",
              "| task | max_fit_error (fitting grid) | approx. error, fixed points | approx. error, dense points |",
              "|---|---:|---:|---:|"]
    for task in sorted({r["task"] for r in kernel_rows}):
        r = next(k for k in kernel_rows if k["task"] == task)
        lines.append(f"| {task} | {r.get('max_fit_error', '-')} | {r.get('approx_error_fixed', '-')} | "
                     f"{r.get('approx_error_dense', '-')} |")
    lines += ["", "## Re-scored kernels", "", "| task | kernel | used by | correct | eval (ms) | speedup |",
              "|---|---|---|---|---:|---:|"]
    for k in kernel_rows:
        lines.append(f"| {k['task']} | {k['kernel']} | {k['runs'] or 'baseline'} | {k['correct']} | "
                     f"{k['eval_median_ms'] if k['eval_median_ms'] == '' else round(k['eval_median_ms'], 2)} | "
                     f"{'' if k['speedup'] == '' else round(k['speedup'], 3)} |")
    (out / "phase2_pilot.md").write_text("\n".join(lines) + "\n")


def main():
    p = argparse.ArgumentParser(description=__doc__.split("\n\n")[0])
    p.add_argument("--runs", type=Path, required=True, help="root holding <strategy>/<task>/run<k>/")
    p.add_argument("--tasks", nargs="+", help="default: every task found")
    p.add_argument("--out", type=Path, required=True)
    p.add_argument("--work", type=Path, default=WORK / "rescore")
    args = p.parse_args()
    runs = discover(args.runs)
    tasks = args.tasks or sorted({r["task"] for r in runs})
    started = datetime.now().isoformat(timespec="seconds")
    evaluator = Evaluator(work=args.work)
    kernel_rows = []
    for name in tasks:
        print(f"== re-scoring {name}: {sum(r['task'] == name for r in runs)} runs", flush=True)
        kernel_rows += rescore_task(TASKS[name], [r for r in runs if r["task"] == name], evaluator)
    args.out.mkdir(parents=True, exist_ok=True)
    meta = {
        "started": started, "finished": datetime.now().isoformat(timespec="seconds"), "git": git_state(),
        "machine": f"{platform.system()} {platform.machine()} {platform.processor()}",
        "compiler": subprocess.run(["c++", "--version"], capture_output=True, text=True).stdout.splitlines()[0],
        "seal": "4.1", "protocol": {"warmup_processes": WARMUP, "measured_processes": MEASURED,
                                    "fhe_runs_per_process": EVAL_REPEATS, "compile_runs": COMPILE_RUNS,
                                    "interleaved": True, "heldout_correctness": True},
        "proposers": sorted({json.dumps(r["proposer"], sort_keys=True) for r in runs}),
        "baselines": {t: sha1(TASKS[t].baseline()) for t in tasks},
        "runs_root": str(args.runs), "runs": len([r for r in runs if r["task"] in tasks]),
    }
    (args.out / "rescore.meta.json").write_text(json.dumps(meta, indent=1) + "\n")
    report(args.out, kernel_rows, [r for r in runs if r["task"] in tasks], meta)
    print(f"wrote {args.out / 'phase2_pilot.md'}, rescore.csv, runs.csv, rescore.meta.json")


if __name__ == "__main__":
    main()
