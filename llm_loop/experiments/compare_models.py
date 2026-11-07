#!/usr/bin/env python3
"""Model comparison table: Haiku, Sonnet and Opus 5.5 in my loop, archive strategy, hints none.

Reads the neutral re-scoring of each experiment (results/<dir>/runs.csv) and writes
results/model_comparison/{README.md,comparison.csv}. No model calls, no timing.
A set whose re-scoring is missing is shown as pending, never filled with in-loop numbers.

  python3 -m llm_loop.experiments.compare_models
"""
import csv
import math
import statistics
from pathlib import Path

from llm_loop.experiments.campaign import NN, OTHERS, PILOT, QUAD_BN, REST
from llm_loop.tasks import REPO, TASKS

RESULTS = REPO / "llm_loop" / "results"
OUT = RESULTS / "model_comparison"
MODELS = ["haiku", "sonnet", "opus"]
# Re-scoring folders per model; each covers the tasks it lists in its runs.csv.
SOURCES = {
    "haiku": ["phase2_pilot_5runs", "phase2_chebyshev_rest", "phase2_quad_batchnorm", "phase2_nn_haiku",
              "phase2_others_haiku"],
    "sonnet": ["phase2_sonnet", "phase2_sonnet_quad_batchnorm", "phase2_nn_sonnet", "phase2_others_sonnet"],
    "opus": ["phase2_opus_focus", "phase2_nn_opus_focus", "phase2_others_opus"],
}
# Runs that were made but not yet neutrally re-scored.
PENDING = {}
GROUPS = [
    ("9 Chebyshev activations", PILOT + REST, ["haiku", "sonnet"]),
    ("GELU, Sigmoid d7, ELU", PILOT, MODELS),
    ("nn_conv2d (BFV)", ["nn_conv2d"], MODELS),
    ("Quad, BatchNorm1d/2d, nn_linear, nn_avgpool2d", QUAD_BN + ["nn_linear", "nn_avgpool2d"], ["haiku", "sonnet"]),
    ("The other 14 operators", OTHERS, MODELS),
]
ORDER = PILOT + REST + QUAD_BN + NN + OTHERS


def load():
    """{model: {task: [run rows]}}, archive strategy only."""
    data = {m: {} for m in MODELS}
    for m, dirs in SOURCES.items():
        for d in dirs:
            path = RESULTS / d / "runs.csv"
            if not path.exists():
                continue
            for r in csv.DictReader(open(path)):
                if r["strategy"] == "archive":
                    data[m].setdefault(r["task"], []).append(r)
    return data


def cell(rows):
    if not rows:
        return None
    sp = [float(r["speedup"]) for r in rows]
    bad = sum(r["correct"] != "True" for r in rows)
    text = (f"x{sp[0]:.2f}" if len(sp) == 1 else
            f"x{statistics.median(sp):.2f} ({min(sp):.2f}-{max(sp):.2f}, {len(sp)} runs)")
    return text + (f", {bad} incorrect" if bad else "")


def gmean(xs):
    return math.exp(sum(math.log(x) for x in xs) / len(xs))


def main():
    data = load()
    OUT.mkdir(parents=True, exist_ok=True)
    table, rows = [], []
    for t in ORDER:
        task = TASKS[t]
        cells = []
        for m in MODELS:
            c = cell(data[m].get(t))
            cells.append(c or ("pending re-timing" if t in PENDING.get(m, ()) else "not run"))
            for r in data[m].get(t, []):
                rows.append({"task": t, "owner": task.owner, "scheme": task.scheme, "model": m, "run": r["run"],
                             "speedup": r["speedup"], "correct": r["correct"], "final_is_baseline": r["final_is_baseline"],
                             "ct_ct_mul": r["ct_ct_mul"], "source_models": r["models"]})
        table.append(f"| {t} | {task.owner} | {task.scheme.upper()} | " + " | ".join(cells) + " |")

    summary = []
    for name, tasks, models in GROUPS:
        if not all(data[m].get(t) for m in models for t in tasks):
            summary.append(f"| {name} | {len(tasks)} | " + " | ".join("pending" if m in models else "-" for m in MODELS) + " |")
            continue
        means = {m: gmean([statistics.mean(float(r["speedup"]) for r in data[m][t]) for t in tasks]) for m in models}
        summary.append(f"| {name} | {len(tasks)} | " + " | ".join(f"x{means[m]:.3f}" if m in models else "-"
                                                                  for m in MODELS) + " |")

    n_runs = sum(len(v) for m in MODELS for v in data[m].values())
    n_bad = sum(r["correct"] != "True" for m in MODELS for v in data[m].values() for r in v)
    md = [
        "# Model comparison: Haiku, Sonnet and Opus 5.5 in my loop",
        "",
        "Written by `experiments/compare_models.py` from the neutral re-scoring of each experiment. Archive",
        "strategy, hints none, measured feedback, 8 model calls per run, all through `claude -p` on the",
        "subscription. Speedup = baseline eval-only median / final-kernel eval-only median, both from the same",
        "re-scoring session. The final kernel is the strategy's own selection.",
        "",
        "- **Runs differ per task.** 3 runs per model for the Chebyshev tasks and nn_conv2d (Haiku: 5 for GELU,",
        "  Sigmoid d7, ELU), 1 run for the rest. A single run says nothing about run-to-run spread.",
        "- **Sessions differ.** Each model's runs were re-scored in their own session on the same machine. Speedups",
        "  are ratios to the baseline of the same session, for which the same kernel moved by about 0.1% (median) across sessions earlier.",
        "- **Correctness.** Every final kernel is checked on the fixed, dense and 6 held-out cases;"
        f" {n_runs - n_bad} of {n_runs} finals pass.",
        "- **Repeated check.** Opus run 1 on GELU (x2.63) failed 1 of 20 repeated checks (error 1.06e-06,",
        "  tolerance 1e-06), so it is not reliably correct; Sigmoid d7 run 1 (x2.55) passed 20 of 20",
        "  (`../phase2_opus_focus/repeat_correctness.md`). Counting GELU run 1 as x1.00, the Opus geometric",
        "  mean over GELU, Sigmoid d7 and ELU is x1.488 instead of x1.661.",
        "- **Noisier session.** Sonnet and Opus on the other 14 were re-timed on battery with more spread",
        "  (max-min up to about 14%, more on sub-millisecond kernels). Speedups are medians, and the",
        "  improved kernels agree across models; differences of a few percent here are noise.",
        "- **relu_composite.** Its baseline itself failed a held-out case once (error 1.35e-06, tolerance",
        "  1e-06), so the Opus run was redone from scratch.",
        "- These are my loop's results only. Teammates' kernels are compared in `../phase2_teammates_rescore/`.",
        "",
        "## Across tasks (geometric mean of per-task mean speedups, same tasks for every model in a row)",
        "",
        "| tasks | n | Haiku | Sonnet | Opus |",
        "|---|---:|---:|---:|---:|",
        *summary,
        "",
        "## Every operator (median, min-max over runs)",
        "",
        "| operator | owner | scheme | Haiku | Sonnet | Opus |",
        "|---|---|---|---|---|---|",
        *table,
        "",
    ]
    (OUT / "README.md").write_text("\n".join(md))
    with open(OUT / "comparison.csv", "w", newline="") as f:
        w = csv.DictWriter(f, fieldnames=list(rows[0]))
        w.writeheader()
        w.writerows(rows)
    print(f"wrote {OUT / 'README.md'} and comparison.csv ({len(rows)} runs)")


if __name__ == "__main__":
    main()
