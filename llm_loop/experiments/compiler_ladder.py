#!/usr/bin/env python3
"""Compiler ladder: unoptimized code vs CHEHAB's rewrite rulesets vs the LLM loop's kernel, one session per task.

For every task, these programs are measured in one interleaved session (rescore.py's protocol):
  base-none            the task's baseline kernel, unoptimized: no term rewriting, no CSE, operand
                       ordering or constant folding (driver mode `none`)
  base-simplification  the baseline through CHEHAB's simplification ruleset: the benchmarks' setting and
                       the reference of every loop speedup
  base-depth, base-ops_cost, base-joined
                       the baseline through CHEHAB's other rulesets (a ruleset CHEHAB rejects for the
                       task, e.g. joined on CKKS, is reported and not timed)
  llm                  the LLM loop's best kernel for the task, through the simplification ruleset, as it
                       was found. It is the run final with the highest strict speedup in an earlier neutral
                       re-scoring (--prior), re-measured here in a fresh session, so the speedup reported
                       here is not the one it was selected on.

Every program is checked on the fixed, dense and 6 held-out cases; programs faster than the
unoptimized baseline get the repeated correctness checks of protocol v1.1 (20 per case, BFV 2),
and a program failing any check is reported as not reliably correct. Timing: 1 warmup + 10 measured
processes of ./main --repeats 10, interleaved; eval-only medians. The CHEHAB e-graph vectorizer (BFV,
packed layout) is not part of this ladder: PROTOCOL.md section 6.

  python3 -m llm_loop.experiments.compiler_ladder --out llm_loop/results/compiler_ladder \\
      --prior llm_loop/results/feedback_pilot_haiku-5-5/runs1-2 --runs llm_loop/runs/feedback-pilot-haiku-5-5
"""
import argparse
import csv
import hashlib
import json
import math
import platform
import statistics
import subprocess
from datetime import datetime
from pathlib import Path
from typing import Dict, List, Optional

from llm_loop.evaluate import WORK, Evaluator
from llm_loop.loop import git_state, normalize
from llm_loop.machine import machine_state, require_stable_power
from llm_loop.rescore import (BFV_CORRECTNESS_REPEATS, COMPILE_RUNS, CORRECTNESS_REPEATS, EVAL_REPEATS, MEASURED,
                              WARMUP, discover, repeat_correctness, sha1, time_once)
from llm_loop.tasks import TASKS, Task

RULESETS = ("simplification", "depth", "ops_cost", "joined")
CONFIGS = ["base-none"] + [f"base-{r}" for r in RULESETS] + ["llm"]


def llm_kernels(run_roots: List[Path], prior_dirs: List[Path]) -> Dict[str, dict]:
    """Per task, the run final with the highest strict speedup in the prior re-scorings (reliably correct only)."""
    sources = {}
    for root in run_roots:
        for r in discover(root):
            sources.setdefault(sha1(normalize(r["best_source"])), (r["best_source"], f"{root.name}/{r['strategy']}/run{r['run']}"))
    best: Dict[str, dict] = {}
    for d in prior_dirs:
        for row in csv.DictReader(open(d / "rescore.csv")):
            if row["kernel"] == "baseline" or row["reliably_correct"] != "True" or not row["speedup"]:
                continue
            if row["sha1"] not in sources:
                continue
            s = float(row["speedup"])
            if s > best.get(row["task"], {}).get("prior_speedup", 0):
                src, origin = sources[row["sha1"]]
                best[row["task"]] = {"source": src, "origin": origin, "prior_speedup": s, "prior": str(d),
                                     "sha1": row["sha1"]}
    return best


def code_sha1(evaluator: Evaluator, task: Task, config: str) -> str:
    """Hash of the SEAL code CHEHAB generated for every case: equal hashes mean the same program."""
    h = hashlib.sha1()
    for f in sorted(evaluator.work.glob(f"eval/{task.name}/{config}/*/stage/he/_gen_he_fhe.cpp")):
        h.update(f.parent.parent.parent.name.encode() + b"\0" + f.read_bytes())
    return h.hexdigest()


def ladder_task(task: Task, evaluator: Evaluator, llm: Optional[dict], correctness_repeats: int) -> List[dict]:
    programs = []  # (config, source, compiler mode)
    for config in CONFIGS:
        if config == "llm":
            if llm:
                programs.append((config, llm["source"], "simplification"))
        else:
            programs.append((config, task.baseline(), config.removeprefix("base-")))
    evals = {}
    for config, source, mode in programs:
        evaluator.compiler_mode = mode
        ev = evaluator.evaluate(task, source, heldout=True, tag=config)
        compile_ms = evaluator.compile_times(task, config, COMPILE_RUNS) if ev.correct else []
        evals[config] = (ev, compile_ms)
        print(f"   {task.name} {config:<20} {ev.status:<13} {ev.message.splitlines()[0][:100] if ev.message else ''}",
              flush=True)
    evaluator.compiler_mode = "simplification"
    timed = [c for c, _, _ in programs if evals[c][0].correct]
    samples: Dict[str, list] = {c: [] for c in timed}
    for i in range(WARMUP + MEASURED):
        shift = i % len(timed) if timed else 0
        for c in timed[shift:] + timed[:shift]:
            t = time_once(evaluator, task, c)
            if i >= WARMUP:
                samples[c].append(t)
    power = machine_state()
    med = {c: statistics.median(x["eval"][0] for x in samples[c] if x["eval"]) for c in timed
           if any(x["eval"] for x in samples[c])}
    ref = med.get("base-none")
    n_rep = BFV_CORRECTNESS_REPEATS if task.scheme == "bfv" else correctness_repeats
    rows = []
    codes = {c: code_sha1(evaluator, task, c) for c, _, _ in programs if evals[c][0].correct}
    repeated: Dict[str, tuple] = {}  # code hash -> (config, repeat result): identical programs are re-checked once
    for config, source, mode in programs:
        ev, compile_ms = evals[config]
        s = samples.get(config, [])
        rep = {}
        same_as = next((c for c, _, _ in programs if c != config and c in codes and codes[c] == codes.get(config)
                        and CONFIGS.index(c) < CONFIGS.index(config)), "")
        faster = config in med and ref is not None and ref / med[config] > 1.0
        if ev.correct and n_rep > 0 and (faster or config in ("base-none", "base-simplification")):
            if codes[config] in repeated:
                rep = dict(repeated[codes[config]][1], repeat_shared_with=repeated[codes[config]][0])
            else:
                rep = repeat_correctness(evaluator, task, config, n_rep)
                repeated[codes[config]] = (config, rep)
        all_pass = all(x["pass"] for x in s) if s else False
        reliable = bool(ev.correct and all_pass and (not rep or rep["repeat_passes"] == rep["repeat_checks"]))
        rows.append({
            "task": task.name, "scheme": task.scheme, "config": config, "compiler_mode": mode,
            "status": ev.status, "message": ev.message.split("\n")[0][:200] if ev.message else "",
            "correct": ev.correct, "all_timing_runs_pass": all_pass if s else "",
            "repeat_checks": rep.get("repeat_checks", 0), "repeat_passes": rep.get("repeat_passes", 0),
            "repeat_worst_error": rep.get("repeat_worst_error", ""), "reliably_correct": reliable,
            "eval_median_ms": med.get(config, ""),
            "eval_min_ms": min((x["eval"][0] for x in s if x["eval"]), default=""),
            "eval_max_ms": max((x["eval"][0] for x in s if x["eval"]), default=""),
            "levels": ev.static.get("levels"), "depth": ev.depth, "xdepth": ev.xdepth,
            "ct_ct_mul": ev.static.get("ct_ct_mul"), "ct_pt_mul": ev.static.get("ct_pt_mul"),
            "rotate": ev.static.get("rotate"), "align_mul": ev.static.get("align_mul"),
            "add": ev.static.get("add"), "mod_switch": ev.static.get("mod_switch"),
            "compile_median_ms": statistics.median(compile_ms) if compile_ms else "",
            "code_sha1": codes.get(config, ""), "same_code_as": same_as,
            "repeats_shared_with": rep.get("repeat_shared_with", ""),
            "measured_processes": len([x for x in s if x["eval"]]),
            "llm_origin": llm["origin"] if config == "llm" and llm else "",
            "llm_prior_speedup": llm["prior_speedup"] if config == "llm" and llm else "",
            "power_after_timing": power["power_source"], "lid_closed_after_timing": power["lid_closed"],
        })
    return rows


def ratio(a, b) -> Optional[float]:
    return a / b if a not in ("", None) and b not in ("", None) and b > 0 else None


def report(out: Path, rows: List[dict], meta: dict) -> None:
    with open(out / "ladder.csv", "w", newline="") as fh:
        w = csv.DictWriter(fh, fieldnames=list(dict.fromkeys(k for r in rows for k in r)))
        w.writeheader()
        w.writerows(rows)
    by = {(r["task"], r["config"]): r for r in rows}
    tasks = sorted({r["task"] for r in rows})

    def ms(task, config):  # eval median of a reliably correct program, else None
        r = by.get((task, config))
        return r["eval_median_ms"] if r and r["reliably_correct"] and r["eval_median_ms"] != "" else None

    lines = ["# Compiler ladder: unoptimized vs CHEHAB rulesets vs the LLM loop", "",
             f"Commit {meta['git']['commit'][:10]} ({'dirty' if meta['git']['dirty'] else 'clean'}), {meta['machine']}, "
             f"{meta['compiler']}, SEAL {meta['seal']}, CKKS chain {meta['ckks_chain']}. {meta['started']} to "
             f"{meta['finished']}.", "",
             "Each task's programs were measured in one interleaved session (1 warmup + 10 measured processes of "
             f"./main --repeats {EVAL_REPEATS}); speedups are ratios of eval-only medians from that session. "
             "Every program was checked on the fixed, dense and 6 held-out cases; programs faster than the "
             "unoptimized baseline were re-checked 20 times per case (BFV: 2). A program that is not reliably "
             "correct is shown as `fail` and not counted. `CHEHAB best` = the fastest reliably correct ruleset "
             "for that task. `llm` = the LLM loop's best final kernel (selected in an earlier neutral session, "
             "re-measured here); `-` = no LLM kernel for the task on this machine.", "",
             "## Speedup over the unoptimized baseline", "",
             "| task | scheme | unopt. (ms) | simplification | depth | ops_cost | joined | CHEHAB best | llm | "
             "llm / CHEHAB default | llm / CHEHAB best |",
             "|---|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|"]
    agg = {k: [] for k in ("simplification", "best", "llm", "llm_vs_default", "llm_vs_best", "llm_tasks_best",
                           "llm_tasks_default")}
    for t in tasks:
        ref = ms(t, "base-none")
        cells = []
        for rs in RULESETS:
            r = by.get((t, f"base-{rs}"))
            if r is None:
                cells.append("-")
            elif r["status"] != "ok":
                cells.append("rejected")
            elif not r["reliably_correct"]:
                cells.append("fail")
            else:
                cells.append(f"x{ratio(ref, ms(t, f'base-{rs}')):.3f}" if ref else "-")
        rules_ms = [ms(t, f"base-{rs}") for rs in RULESETS if ms(t, f"base-{rs}")]
        best_ms = min(rules_ms) if rules_ms else None
        llm_r = by.get((t, "llm"))
        llm_ms = ms(t, "llm")
        llm_cell = "-" if llm_r is None else (f"x{ratio(ref, llm_ms):.3f}" if llm_ms and ref else "fail")
        vs_def = ratio(ms(t, "base-simplification"), llm_ms)
        vs_best = ratio(best_ms, llm_ms)
        lines.append(f"| {t} | {by[(t, 'base-none')]['scheme']} | {ref:.2f} | " if ref else f"| {t} | - | - | ")
        lines[-1] += " | ".join(cells) + f" | {'x%.3f' % ratio(ref, best_ms) if ref and best_ms else '-'} | {llm_cell} | " \
                     f"{'x%.3f' % vs_def if vs_def else '-'} | {'x%.3f' % vs_best if vs_best else '-'} |"
        if ref and ms(t, "base-simplification"):
            agg["simplification"].append(ratio(ref, ms(t, "base-simplification")))
        if ref and best_ms:
            agg["best"].append(ratio(ref, best_ms))
        if ref and llm_ms:
            agg["llm"].append(ratio(ref, llm_ms))
            agg["llm_tasks_default"].append(ratio(ref, ms(t, "base-simplification")))
            agg["llm_tasks_best"].append(ratio(ref, best_ms))
        if vs_def:
            agg["llm_vs_default"].append(vs_def)
        if vs_best:
            agg["llm_vs_best"].append(vs_best)
    geo = lambda v: math.exp(statistics.mean(math.log(x) for x in v)) if v else None
    fmtg = lambda v: f"x{geo(v):.3f} ({len(v)} tasks)" if v else "-"
    lines += ["", "## Across tasks (geometric means)", "",
              "| comparison | geometric mean |", "|---|---:|",
              f"| CHEHAB simplification vs unoptimized, every task | {fmtg(agg['simplification'])} |",
              f"| CHEHAB best ruleset vs unoptimized, every task | {fmtg(agg['best'])} |",
              f"| CHEHAB simplification vs unoptimized, tasks with an LLM kernel | {fmtg(agg['llm_tasks_default'])} |",
              f"| CHEHAB best ruleset vs unoptimized, tasks with an LLM kernel | {fmtg(agg['llm_tasks_best'])} |",
              f"| LLM kernel vs unoptimized | {fmtg(agg['llm'])} |",
              f"| LLM kernel vs CHEHAB simplification (the loops' reference) | {fmtg(agg['llm_vs_default'])} |",
              f"| LLM kernel vs CHEHAB best ruleset | {fmtg(agg['llm_vs_best'])} |", "",
              "Identical generated code measured under two configurations shows the session's noise floor; "
              "differences of a few percent between such configurations are noise.", "",
              "## Programs", "",
              "| task | config | status | reliably correct | repeats | eval (ms) | levels | ct x ct | ct x pt | rot | align | "
              "C (ms) | note |", "|---|---|---|---|---:|---:|---:|---:|---:|---:|---:|---:|---|"]
    for r in rows:
        note = r["llm_origin"] + (f" (prior x{r['llm_prior_speedup']:.3f})" if r["llm_prior_speedup"] else "")
        note = note or (f"same SEAL code as {r['same_code_as']}" if r.get("same_code_as") else r["message"][:80])
        lines.append(f"| {r['task']} | {r['config']} | {r['status']} | {r['reliably_correct']} | "
                     f"{r['repeat_passes']}/{r['repeat_checks']} | "
                     f"{'-' if r['eval_median_ms'] == '' else round(r['eval_median_ms'], 2)} | {r['levels']} | "
                     f"{r['ct_ct_mul']} | {r['ct_pt_mul']} | {r['rotate']} | {r['align_mul']} | "
                     f"{'-' if r['compile_median_ms'] == '' else round(r['compile_median_ms'], 2)} | {note} |")
    (out / "ladder.md").write_text("\n".join(lines) + "\n")


def main():
    p = argparse.ArgumentParser(description=__doc__.split("\n\n")[0])
    p.add_argument("--tasks", nargs="+", default=sorted(TASKS), help="default: all 34 tasks")
    p.add_argument("--runs", type=Path, nargs="*", default=[], help="loop run roots holding the LLM kernels")
    p.add_argument("--prior", type=Path, nargs="*", default=[], help="rescore result dirs used to pick the LLM kernel")
    p.add_argument("--out", type=Path, required=True)
    p.add_argument("--work", type=Path, default=WORK / "compiler-ladder")
    p.add_argument("--ckks-chain", choices=("fixed", "sized"), default="sized")
    p.add_argument("--correctness-repeats", type=int, default=CORRECTNESS_REPEATS)
    p.add_argument("--allow-unstable-power", action="store_true")
    args = p.parse_args()
    power_at_start = require_stable_power(args.allow_unstable_power)
    llm = llm_kernels(args.runs, args.prior)
    print("LLM kernels: " + (", ".join(f"{t} ({k['origin']}, prior x{k['prior_speedup']:.3f})"
                                       for t, k in sorted(llm.items())) or "none"), flush=True)
    evaluator = Evaluator(work=args.work, chain=args.ckks_chain)
    args.out.mkdir(parents=True, exist_ok=True)
    partial = args.out / "partial_rows.json"
    rows = json.loads(partial.read_text()) if partial.exists() else []
    done = {r["task"] for r in rows}
    started = datetime.now().isoformat(timespec="seconds")
    for name in args.tasks:
        if name in done:
            print(f"== {name}: already measured (partial_rows.json)", flush=True)
            continue
        print(f"== {name}", flush=True)
        rows += ladder_task(TASKS[name], evaluator, llm.get(name), args.correctness_repeats)
        partial.write_text(json.dumps(rows, default=str))  # resume point
    meta = {
        "started": started, "finished": datetime.now().isoformat(timespec="seconds"), "git": git_state(),
        "machine": f"{platform.system()} {platform.machine()} {platform.processor()}",
        "compiler": subprocess.run(["c++", "--version"], capture_output=True, text=True).stdout.splitlines()[0],
        "seal": "4.1", "ckks_chain": args.ckks_chain, "tasks": args.tasks,
        "protocol": {"warmup_processes": WARMUP, "measured_processes": MEASURED, "fhe_runs_per_process": EVAL_REPEATS,
                     "compile_runs": COMPILE_RUNS, "heldout_correctness": True,
                     "correctness_repeats": args.correctness_repeats, "bfv_correctness_repeats": BFV_CORRECTNESS_REPEATS},
        "llm_kernels": {t: {k: v for k, v in d.items() if k != "source"} for t, d in llm.items()},
        "power_at_start": power_at_start, "power_at_end": machine_state(),
    }
    (args.out / "ladder.meta.json").write_text(json.dumps(meta, indent=1) + "\n")
    report(args.out, rows, meta)
    print(f"wrote {args.out / 'ladder.md'}, ladder.csv, ladder.meta.json", flush=True)


if __name__ == "__main__":
    main()
