#!/usr/bin/env python3
"""Feedback ablation report: does compiler feedback to the model improve the result?

Reads the runs (<runs>/<strategy>/<task>/run<k>/) and their neutral re-scoring
(<rescore>/runs.csv from llm_loop/rescore.py, one session) and writes
<out>/feedback_ablation.md and feedback_ablation.csv.

Reference strategy: `independent` (no feedback). For every other strategy:
  - final kernel: neutral speedup per run, per-task mean, geometric mean over tasks;
  - effect: per-task ratio of mean speedups (strategy / independent) and its
    geometric mean, with a 95% bootstrap interval (runs resampled within each
    task, 10,000 draws) and an exact two-sided sign-flip permutation test on the
    log-speedup differences of runs paired by task and run number (runs with the
    same number were made next to each other in the session);
  - search behaviour (from attempts.jsonl, in-loop timings, not neutral):
    best-so-far speedup after k calls, correct attempts, duplicates, failures.
Call 1 gets the same prompt under `independent`, `linear` and `linear-minimal`,
so their call-1 results are samples of the same distribution: a sanity check.

Feedback study (PROTOCOL.md, section 10): the declared primary comparisons
(PRIMARY, those whose two strategies are present) get a Holm correction; every
strategy against `independent` is reported as secondary. Diversity of each
strategy's proposals (loop.diversity) is reported for every run, including runs
made before it was recorded.

  python3 -m llm_loop.experiments.compare_feedback --runs llm_loop/runs/feedback-ablation \
      --rescore llm_loop/results/feedback_ablation --out llm_loop/results/feedback_ablation
"""
import argparse
import csv
import itertools
import json
import math
import statistics
from pathlib import Path
from typing import Dict, List, Optional

import numpy as np

from llm_loop.loop import diversity

REFERENCE = "independent"
# The feedback study's primary comparisons, declared before its runs: (strategy, against, question).
PRIMARY = [
    ("linear", "independent", "does feedback to the model help?"),
    ("linear", "linear-minimal", "does the performance content of the feedback help (beyond correctness)?"),
    ("linear-explore", "linear", "does steering toward new approaches help the dialogue?"),
    ("archive", "linear", "does search on top of feedback help?"),
    ("archive-explore", "archive", "does steering toward new approaches help the search?"),
]
MC_DRAWS = 100_000
FAILURES = ("rejected", "compile_error", "chehab_error", "build_error", "run_error", "wrong_output", "heldout_fail",
            "no_code", "predicted_slower")


def geomean(values: List[float]) -> float:
    return math.exp(statistics.mean(math.log(v) for v in values))


def load_runs(root: Path) -> List[dict]:
    """Every completed run with its per-call trace (in-loop measurements)."""
    runs = []
    for result in sorted(root.glob("*/*/run*/result.json")):
        r = json.loads(result.read_text())
        attempts = [json.loads(line) for line in (result.parent / "attempts.jsonl").read_text().splitlines()
                    if line.strip()] if (result.parent / "attempts.jsonl").exists() else []
        base = r["baseline"]["eval_time_ms"]
        best, curve = base, []
        for a in sorted(attempts, key=lambda a: a["iteration"]):
            ev = a.get("evaluation") or {}
            if ev.get("correct") and ev.get("eval_time_ms"):
                best = min(best, ev["eval_time_ms"])
            curve.append(base / best)
        r["curve"] = curve
        r["attempts"] = attempts
        r["diversity"] = diversity(attempts, r["baseline"].get("static"))
        runs.append(r)
    return runs


def load_neutral(rescore: Path) -> Dict[tuple, Optional[float]]:
    """(task, strategy, run) -> neutral speedup of the run's final kernel. Re-scorings with repeated correctness
    checks give `speedup_strict` (x1.000 for a kernel that is not reliably correct), which is used when present;
    older ones give None for a kernel that failed the re-check."""
    out = {}
    with open(rescore / "runs.csv") as f:
        for row in csv.DictReader(f):
            value = row.get("speedup_strict") or row["speedup"]
            out[(row["task"], row["strategy"], int(row["run"]))] = float(value) if value else None
    return out


def sign_flip_p(diffs: List[float], seed: int = 0) -> float:
    """Two-sided sign-flip permutation p-value for the mean of paired differences: exact up to 20 pairs, else
    Monte Carlo with MC_DRAWS random sign vectors (p = (hits + 1) / (draws + 1))."""
    if not diffs:
        return float("nan")
    observed = abs(statistics.mean(diffs))
    if len(diffs) > 20:
        d = np.asarray(diffs)
        signs = np.random.default_rng(seed).choice((-1.0, 1.0), size=(MC_DRAWS, len(d)))
        hits = int((np.abs((signs * d).mean(axis=1)) >= observed - 1e-12).sum())
        return (hits + 1) / (MC_DRAWS + 1)
    hits = total = 0
    for signs in itertools.product((1, -1), repeat=len(diffs)):
        total += 1
        hits += abs(statistics.mean(s * d for s, d in zip(signs, diffs))) >= observed - 1e-12
    return hits / total


def holm(pvalues: List[float]) -> List[float]:
    """Holm-adjusted p-values, in the input order."""
    order = sorted(range(len(pvalues)), key=lambda i: pvalues[i])
    adjusted, running = [0.0] * len(pvalues), 0.0
    for rank, i in enumerate(order):
        running = max(running, min(1.0, (len(pvalues) - rank) * pvalues[i]))
        adjusted[i] = running
    return adjusted


def compare(per_task: Dict[str, Dict[str, List[float]]], speed: Dict[tuple, float], a: str, b: str) -> Optional[dict]:
    """Strategy a against b: geometric-mean ratio over tasks, bootstrap interval, paired sign-flip test."""
    ts = [t for t in per_task if per_task[t].get(a) and per_task[t].get(b)]
    if not ts:
        return None
    ratio = geomean([statistics.mean(per_task[t][a]) / statistics.mean(per_task[t][b]) for t in ts])
    lo, hi = bootstrap_ratio(per_task, a, b)
    diffs = [math.log(speed[(t, s, k)] / speed[(t, b, k)]) for (t, s, k) in speed if s == a and (t, b, k) in speed]
    better = sum(d > math.log(1.02) for d in diffs)
    worse = sum(d < -math.log(1.02) for d in diffs)
    return {"strategy": a, "reference": b, "tasks": len(ts), "geomean_ratio": ratio, "ci_low": lo, "ci_high": hi,
            "paired_runs": len(diffs), "sign_flip_p": sign_flip_p(diffs), "better": better, "worse": worse,
            "tied": len(diffs) - better - worse}


def bootstrap_ratio(per_task: Dict[str, Dict[str, List[float]]], strategy: str, reference: str = REFERENCE,
                    draws: int = 10000, seed: int = 0) -> tuple:
    """95% interval of the geometric mean over tasks of mean(strategy) / mean(reference)."""
    rng = np.random.default_rng(seed)
    tasks = [t for t in per_task if per_task[t].get(strategy) and per_task[t].get(reference)]
    stats = []
    for _ in range(draws):
        logs = []
        for t in tasks:
            a = rng.choice(per_task[t][strategy], len(per_task[t][strategy]))
            b = rng.choice(per_task[t][reference], len(per_task[t][reference]))
            logs.append(math.log(a.mean() / b.mean()))
        stats.append(math.exp(statistics.mean(logs)))
    return float(np.percentile(stats, 2.5)), float(np.percentile(stats, 97.5))


def main():
    p = argparse.ArgumentParser(description=__doc__.split("\n\n")[0])
    p.add_argument("--runs", type=Path, required=True)
    p.add_argument("--rescore", type=Path, help="directory with rescore.py's runs.csv (neutral timings)")
    p.add_argument("--out", type=Path, required=True)
    args = p.parse_args()

    runs = load_runs(args.runs)
    if not runs:
        raise SystemExit(f"no completed runs under {args.runs}")
    neutral = load_neutral(args.rescore) if args.rescore and (args.rescore / "runs.csv").exists() else {}
    source = "neutral re-scoring" if neutral else "IN-LOOP timings (not neutral: run rescore.py first)"
    strategies = sorted({r["strategy"] for r in runs}, key=lambda s: (s != REFERENCE, s))
    tasks = sorted({r["task"] for r in runs})
    calls = max(r["calls"] for r in runs)

    speed: Dict[tuple, float] = {}
    for r in runs:
        key = (r["task"], r["strategy"], r["run"])
        s = neutral.get(key) if neutral else r["speedup_in_loop"]
        if s is not None:
            speed[key] = s
    per_task: Dict[str, Dict[str, List[float]]] = {t: {} for t in tasks}
    for (t, s, _), v in speed.items():
        per_task[t].setdefault(s, []).append(v)

    lines = ["# Feedback ablation", "",
             f"Speedups of each run's final kernel from {source}. Reference: `{REFERENCE}` (no feedback to the "
             f"model; the loop still verifies every kernel and keeps the fastest correct one). {calls} model calls "
             "per run. Models: " + ", ".join(sorted({m for r in runs for m in r.get("models_used", [])}) or ["-"])
             + ".", "",
             "## Final kernels (mean speedup over runs; runs in brackets)", "",
             "| task | " + " | ".join(strategies) + " |", "|---|" + "---:|" * len(strategies)]
    for t in tasks:
        cells = []
        for s in strategies:
            v = per_task[t].get(s, [])
            cells.append(f"x{statistics.mean(v):.3f} ({len(v)}; {min(v):.2f}-{max(v):.2f})" if v else "-")
        lines.append(f"| {t} | " + " | ".join(cells) + " |")
    common = [t for t in tasks if all(per_task[t].get(s) for s in strategies)]
    if common:
        lines.append(f"| **geometric mean ({len(common)} tasks)** | " + " | ".join(
            f"**x{geomean([statistics.mean(per_task[t][s]) for t in common]):.3f}**" for s in strategies) + " |")

    csv_rows = []
    header = ["| strategy | against | tasks | geo. mean ratio | 95% bootstrap interval | paired runs | sign-flip p | "
              "{}runs better / worse / tied (2%) |", "|---|---|---:|---:|---|---:|---:|{}---|"]
    primary = [c for c in (compare(per_task, speed, a, b) for a, b, _ in PRIMARY if {a, b} <= set(strategies)) if c]
    if primary:
        for c, adj in zip(primary, holm([c["sign_flip_p"] for c in primary])):
            c["holm_p"], c["family"] = adj, "primary"
        questions = {(a, b): q for a, b, q in PRIMARY}
        lines += ["", "## Primary comparisons (declared before the runs; Holm-corrected)", "",
                  "Ratio > 1: the first strategy's final kernels were faster.", "",
                  header[0].format("Holm p | "), header[1].format("---:|")]
        for c in primary:
            lines.append(f"| {c['strategy']} | {c['reference']} | {c['tasks']} | x{c['geomean_ratio']:.3f} | "
                         f"x{c['ci_low']:.3f} - x{c['ci_high']:.3f} | {c['paired_runs']} | {c['sign_flip_p']:.4f} | "
                         f"{c['holm_p']:.4f} | {c['better']} / {c['worse']} / {c['tied']} |")
        lines += [""] + [f"- {c['strategy']} vs {c['reference']}: {questions[(c['strategy'], c['reference'])]}"
                         for c in primary]
        csv_rows += primary
    secondary = [c for c in (compare(per_task, speed, s, REFERENCE) for s in strategies
                             if s != REFERENCE and (s, REFERENCE) not in {(a, b) for a, b, _ in PRIMARY}) if c] \
        if REFERENCE in strategies else []
    if secondary:
        for c in secondary:
            c["holm_p"], c["family"] = "", "secondary"
        lines += ["", f"## Every other strategy vs `{REFERENCE}` (secondary, uncorrected)", "",
                  header[0].format(""), header[1].format("")]
        for c in secondary:
            lines.append(f"| {c['strategy']} | {c['reference']} | {c['tasks']} | x{c['geomean_ratio']:.3f} | "
                         f"x{c['ci_low']:.3f} - x{c['ci_high']:.3f} | {c['paired_runs']} | {c['sign_flip_p']:.4f} | "
                         f"{c['better']} / {c['worse']} / {c['tied']} |")
        csv_rows += secondary
    if csv_rows:
        lines += ["", "Treat a ratio whose interval includes x1.000 as \"no detectable effect\", not as \"no "
                  "effect\"."]

    lines += ["", "## Diversity of the proposals (per run, mean over runs)", "",
              "distinct kernels: proposals that were not identical to an earlier kernel; distinct structures: new "
              "operation-count fingerprints (ct x ct, ct x pt, rotations, levels, alignments, mod switches), the "
              "baseline's excluded; structure repeats: evaluated proposals with a fingerprint already seen; "
              "techniques: distinct `Technique:` lines (explore arms only).", "",
              "| strategy | runs | distinct kernels | duplicates | distinct structures | structure repeats | "
              "distinct techniques |", "|---|---:|---:|---:|---:|---:|---:|"]
    for s in strategies:
        ds = [r["diversity"] for r in runs if r["strategy"] == s]
        mean = lambda k: statistics.mean(d[k] for d in ds)
        lines.append(f"| {s} | {len(ds)} | {mean('distinct_kernels'):.2f} | {mean('duplicates'):.2f} | "
                     f"{mean('distinct_structures'):.2f} | {mean('structure_repeats'):.2f} | "
                     f"{mean('distinct_techniques'):.2f} |")

    lines += ["", "## Search behaviour (in-loop timings, not neutral)", "",
              "Best-so-far speedup after k calls, mean over runs and tasks (the baseline counts as found at k = 0).",
              "", "| strategy | " + " | ".join(f"k={k}" for k in range(1, calls + 1)) + " |",
              "|---|" + "---:|" * calls]
    for s in strategies:
        rs = [r for r in runs if r["strategy"] == s]
        cells = []
        for k in range(calls):
            v = [r["curve"][k] for r in rs if len(r["curve"]) > k]
            cells.append(f"x{statistics.mean(v):.3f}" if v else "-")
        lines.append(f"| {s} | " + " | ".join(cells) + " |")
    lines += ["", "| strategy | runs | correct attempts | duplicates | improved on baseline | failures (all runs) |",
              "|---|---:|---:|---:|---:|---|"]
    for s in strategies:
        rs = [r for r in runs if r["strategy"] == s]
        n_calls = sum(r["calls"] for r in rs)
        fails = {k: sum(r.get("failures", {}).get(k, 0) for r in rs) for k in FAILURES[:-1]}
        fails["predicted_slower"] = sum(r.get("predicted_slower", 0) for r in rs)
        fail_text = ", ".join(f"{k} {v}" for k, v in fails.items() if v) or "-"
        lines.append(f"| {s} | {len(rs)} | {sum(r['correct_attempts'] for r in rs)} / {n_calls} | "
                     f"{sum(r.get('duplicates', 0) for r in rs)} | {sum(r['best_id'] != 0 for r in rs)} / {len(rs)} | "
                     f"{fail_text} |")

    first = {s: [r["curve"][0] for r in runs if r["strategy"] == s and r["curve"]]
             for s in ("independent", "linear", "linear-minimal") if s in strategies}
    if len(first) > 1:
        lines += ["", "## Sanity check: call 1", "",
                  "Call 1 is the same prompt under these strategies, so they should agree up to sampling "
                  "noise.", "", "| strategy | runs | mean call-1 speedup | correct at call 1 |", "|---|---:|---:|---:|"]
        for s, v in first.items():
            ok = sum(1 for r in runs if r["strategy"] == s and r["attempts"]
                     and (r["attempts"][0].get("evaluation") or {}).get("correct"))
            lines.append(f"| {s} | {len(v)} | x{statistics.mean(v):.3f} | {ok} / {len(v)} |")

    args.out.mkdir(parents=True, exist_ok=True)
    (args.out / "feedback_ablation.md").write_text("\n".join(lines) + "\n")
    with open(args.out / "feedback_ablation.csv", "w", newline="") as f:
        div = ["distinct_kernels", "distinct_structures", "structure_repeats", "distinct_techniques"]
        fields = ["task", "strategy", "run", "speedup", "speedup_source", "calls", "correct_attempts", "duplicates",
                  "best_attempt"] + div + [f"best_after_{k}" for k in range(1, calls + 1)]
        w = csv.DictWriter(f, fieldnames=fields)
        w.writeheader()
        for r in sorted(runs, key=lambda r: (r["task"], r["strategy"], r["run"])):
            w.writerow({"task": r["task"], "strategy": r["strategy"], "run": r["run"],
                        "speedup": speed.get((r["task"], r["strategy"], r["run"]), ""),
                        "speedup_source": "neutral" if neutral else "in-loop", "calls": r["calls"],
                        "correct_attempts": r["correct_attempts"], "duplicates": r.get("duplicates", 0),
                        "best_attempt": r["best_id"], **{k: r["diversity"][k] for k in div},
                        **{f"best_after_{k + 1}": round(v, 4) for k, v in enumerate(r["curve"])}})
    if csv_rows:
        with open(args.out / "feedback_effect.csv", "w", newline="") as f:
            w = csv.DictWriter(f, fieldnames=list(dict.fromkeys(k for c in csv_rows for k in c)))
            w.writeheader()
            w.writerows(csv_rows)
    print(f"wrote {args.out / 'feedback_ablation.md'}")


if __name__ == "__main__":
    main()
