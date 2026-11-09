#!/usr/bin/env python3
"""Fixed vs sized CKKS modulus chain: my loop's final kernels re-timed with the chain sized to each kernel.

The fixed chain is CHEHAB's shared CKKS parameters (N = 32768, {60, 40 x 10, 60}) for every kernel. The sized
chain keeps one 40-bit prime per level the kernel consumes (CkksParams::trim_to_levels_used, from the teammates'
sized-chain-and-history branch), so unused levels no longer slow a shallow kernel down and consuming extra
levels (to make later operations cheaper) is no longer rewarded. The baseline gets its own sized chain.

Reads the fixed-chain re-scoring through compare_models.load() and the sized-chain re-scoring from
results/<dir>/runs.csv (strategy column = model), and writes results/sized_chain_rescore/README.md and
comparison.csv. No model calls, no timing.

  python3 -m llm_loop.experiments.compare_sized_chain
"""
import csv
import math
import statistics

from llm_loop.experiments.campaign import OTHERS, PILOT, QUAD_BN, REST
from llm_loop.experiments.compare_models import MODELS, RESULTS, load
from llm_loop.tasks import TASKS

OUT = RESULTS / "sized_chain_rescore"
SIZED = ["sized_chain_rescore"]  # sized-chain re-scorings; later runs are added here
ORDER = PILOT + REST + QUAD_BN + OTHERS
GROUPS = [
    ("9 Chebyshev activations", PILOT + REST),
    ("GELU, Sigmoid d7, ELU", PILOT),
    ("Quad, BatchNorm1d/2d", QUAD_BN),
    ("The other 14 operators", OTHERS),
]


def load_sized():
    data = {m: {} for m in MODELS}
    for d in SIZED:
        path = RESULTS / d / "runs.csv"
        if path.exists():
            for r in csv.DictReader(open(path)):
                data[r["strategy"]].setdefault(r["task"], []).append(r)
    return data


def gmean(xs):
    return math.exp(sum(math.log(x) for x in xs) / len(xs))


def mean_speedup(rows):
    return statistics.mean(float(r["speedup"]) for r in rows)


def main():
    fixed, sized = load(), load_sized()
    base_levels = {}
    for d in SIZED:
        for r in csv.DictReader(open(RESULTS / d / "rescore.csv")):
            if r["kernel"] == "baseline":
                base_levels[r["task"]] = r["levels_run"]

    summary = []
    for name, tasks in GROUPS:
        cells = []
        for m in MODELS:
            if all(sized[m].get(t) and fixed[m].get(t) for t in tasks):
                cells.append(f"x{gmean([mean_speedup(fixed[m][t]) for t in tasks]):.3f} -> "
                             f"**x{gmean([mean_speedup(sized[m][t]) for t in tasks]):.3f}**")
            else:
                cells.append("-")
        summary.append(f"| {name} | {len(tasks)} | " + " | ".join(cells) + " |")

    table, rows = [], []
    fixed_by_run = {(t, m, r["run"]): r for m in MODELS for t, rs in fixed[m].items() for r in rs}
    for t in ORDER:
        cells = []
        for m in MODELS:
            rs = sized[m].get(t)
            if not rs:
                cells.append("not run")
                continue
            levels = ",".join(sorted({r["levels"] for r in rs}))
            bad = sum(r["correct"] != "True" for r in rs)
            cells.append(f"x{mean_speedup(fixed[m][t]):.2f} -> x{mean_speedup(rs):.2f} (L {levels})"
                         + (f", {bad} incorrect" if bad else ""))
            for r in rs:
                f = fixed_by_run.get((t, m, r["run"]), {})
                rows.append({"task": t, "model": m, "run": r["run"], "speedup_fixed": f.get("speedup", ""),
                             "speedup_sized": r["speedup"], "levels": r["levels"], "baseline_levels": base_levels[t],
                             "ct_ct_mul": r["ct_ct_mul"], "correct_sized": r["correct"],
                             "fhe_max_abs_error_sized": r["fhe_max_abs_error"]})
        table.append(f"| {t} | {TASKS[t].owner} | {base_levels[t]} | " + " | ".join(cells) + " |")

    burned = [r for r in rows if r["levels"] and int(r["levels"]) > int(r["baseline_levels"])]
    n_bad = sum(r["correct_sized"] != "True" for r in rows)
    md = [
        "# Fixed vs sized CKKS modulus chain",
        "",
        "Written by `experiments/compare_sized_chain.py`. My loop's CKKS final kernels (archive strategy, hints none,",
        "the same kernels as `../model_comparison/`), re-timed with the coefficient modulus sized to each kernel: one",
        "40-bit prime per level it consumes, plus the 60-bit first and special primes, instead of always 10 levels.",
        "The baseline gets its own sized chain. The kernels were not re-generated: the loop itself ran on the fixed chain.",
        "",
        "- **Why.** With the fixed chain, levels a kernel does not use are carried by every operation, so saving",
        "  levels earned little, and consuming extra levels made later operations cheaper. The sized chain removes",
        "  both effects. It is `CkksParams::trim_to_levels_used` from the teammates' `sized-chain-and-history` branch.",
        "- **How.** One re-scoring session, all models' kernels of a task interleaved with one baseline, the same",
        "  protocol as `rescore.py` (8 correctness cases, 1 warmup + 10 measured processes). Run in a separate,",
        "  git-ignored worktree with the compiler change applied; this branch's compiler is unchanged. The worktree's",
        "  changes are in `worktree.patch` (compiler, `trim_to_levels_used = true` in the driver, runs labelled by",
        "  model folder).",
        f"- **Correctness.** {len(rows) - n_bad} of {len(rows)} kernels pass all 8 cases under the sized chain.",
        f"- **Extra levels.** {len(burned)} kernels consume more levels than their baseline: "
        + "; ".join(f"{r['model']} {r['task']} run {r['run']} ({r['levels']} vs {r['baseline_levels']}, "
                    f"x{float(r['speedup_fixed']):.2f} -> x{float(r['speedup_sized']):.2f})" for r in burned) + ".",
        "- **Caveats.** One session on one laptop, on battery. 1 run per task outside the Chebyshev tasks.",
        "  The BFV tasks are not affected (CHEHAB selects BFV parameters per program) and are not re-timed.",
        "",
        "## Across tasks (geometric mean of per-task mean speedups, fixed -> sized)",
        "",
        "| tasks | n | Haiku | Sonnet | Opus |",
        "|---|---:|---|---|---|",
        *summary,
        "",
        "## Every CKKS operator (mean over runs, fixed -> sized; L = levels of the final kernels)",
        "",
        "| operator | owner | baseline levels | Haiku | Sonnet | Opus |",
        "|---|---|---:|---|---|---|",
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
