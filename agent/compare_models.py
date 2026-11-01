#!/usr/bin/env python3
"""Compare full runs of the optimization loop made with different models, on the
strict selection rule (agent/selection.py): a speedup counts only if the best
kernel's circuit differs from the baseline's and it beat the baseline in
interleaved re-timing.

  python agent/compare_models.py \\
      --run haiku:api_haiku --run sonnet_v2:20261025-1230..20261025-2359 \\
      --run opus:20261101-1024..20261101-2359 --out agent/results/model_comparison.md

A --run is name:<window>. A window is either since..until over run timestamps
(the loop's own strict verdict is read from each results.json; runs made before
the strict rule existed are not accepted here) or the name of a set in
agent/results/remeasured.csv (an older run whose best kernels were re-timed under
the strict rule by agent/remeasure_runs.py).

Columns per run and task: speedup (strict), whether the circuit changed,
correct attempts out of 8, and the best kernel's circuit. Correctness is the
run's own check: every output within the task's tolerance (CKKS) or exactly
equal (BFV) on every case; a task's best kernel is always correct by
construction, so the accuracy column reports how many of the 8 proposals were.
"""
import argparse
import csv
import json
import math
import statistics
from collections import defaultdict
from pathlib import Path

AGENT = Path(__file__).resolve().parent
RUNS = AGENT / "runs"
REMEASURED = AGENT / "results" / "remeasured.csv"

GROUPS = [
    ("Chebyshev activations (CKKS)", lambda t: t.startswith("chebyshev_")),
    ("ReLU composite, Quad, BatchNorm (CKKS)", lambda t: t in ("relu_composite", "quad", "batchnorm1d", "batchnorm2d")),
    ("Add and Multiply (CKKS)", lambda t: t.startswith("elementwise_")),
    ("Extraction, table, embedding (CKKS)", lambda t: t.startswith(("extract", "table_mult", "embedding"))),
    ("Linear, Conv2d, AvgPool2d (BFV)", lambda t: t.startswith("nn_")),
]


def load_window(since: str, until: str) -> dict:
    """Latest strict-rule run per task in [since, until)."""
    out = {}
    for path in sorted(RUNS.glob("*/*/results.json")):
        r = json.loads(path.read_text())
        if r["model"] == "files" or not since <= r["timestamp"] < until:
            continue
        if "final_remeasure" not in r:
            raise SystemExit(f"{path} predates the strict selection rule; use a remeasured.csv set for it")
        cands = [a for a in r["attempts"][1:] if a.get("kind") == "candidate"]
        if not cands:
            continue
        if r["task"] not in out or r["timestamp"] > out[r["task"]]["timestamp"]:
            fr = r.get("final_remeasure") or {}
            best = r["best"]
            out[r["task"]] = {
                "timestamp": r["timestamp"], "model": r["model"], "effort": r.get("effort"),
                "speedup": r["speedup"] if (r.get("structural_change") and fr.get("confirmed", True)) else 1.0,
                "structural": bool(r.get("structural_change")),
                "confirmed": fr.get("confirmed") if fr else None,
                "correct": sum(1 for a in cands if a["evaluation"]["correct"]), "attempts": r["iterations"],
                "chehab_errors": sum(1 for a in cands if a["evaluation"]["status"] == "chehab_error"),
                "baseline_ms": r["baseline"]["eval_time_ms"], "best_ms": best["eval_time_ms"],
                "circuit": circuit(r["baseline"], best),
                "wall_s": r["wall_time_s"], "cost": r.get("equivalent_api_cost_usd", r.get("approx_cost_usd", 0)),
            }
    return out


def load_remeasured(set_name: str) -> dict:
    """An older run re-timed under the strict rule (agent/remeasure_runs.py)."""
    if not REMEASURED.exists():
        raise SystemExit(f"{REMEASURED} is missing")
    out = {}
    for row in csv.DictReader(REMEASURED.open()):
        if row["set"] != set_name:
            continue
        structural = row["structural_change"] == "True"
        confirmed = row["confirmed"] == "True" if row["confirmed"] else None
        speedup = float(row["speedup"]) if structural and confirmed else 1.0
        # the run itself, for attempts/correctness/circuit
        run_dir = next(iter(RUNS.glob(f"{row['task']}/{row['run']}")), None)
        r = json.loads((run_dir / "results.json").read_text()) if run_dir else None
        cands = [a for a in r["attempts"][1:] if a.get("kind") == "candidate"] if r else []
        out[row["task"]] = {
            "timestamp": row["run"], "model": r["model"] if r else "?", "effort": r.get("effort") if r else None,
            "speedup": speedup, "structural": structural, "confirmed": confirmed,
            "correct": sum(1 for a in cands if a["evaluation"]["correct"]), "attempts": r["iterations"] if r else 8,
            "chehab_errors": sum(1 for a in cands if a["evaluation"]["status"] == "chehab_error"),
            "baseline_ms": r["baseline"]["eval_time_ms"] if r else None, "best_ms": r["best"]["eval_time_ms"] if r else None,
            "circuit": circuit(r["baseline"], r["best"]) if r else "",
            "wall_s": r["wall_time_s"] if r else 0, "cost": (r.get("equivalent_api_cost_usd", r.get("approx_cost_usd", 0)) if r else 0),
        }
    if not out:
        raise SystemExit(f"no rows for set {set_name} in {REMEASURED}")
    return out


def circuit(b: dict, best: dict) -> str:
    def c(e):
        ops = e.get("ops", {})
        lv = e.get("levels")
        parts = [f"ct×ct {ops.get('ct_ct_mul', 0)}", f"ct×pt {ops.get('ct_pt_mul', 0)}", f"rot {ops.get('rotate', 0)}"]
        if lv is not None:
            parts.insert(0, f"lv {lv}")
        return ", ".join(parts)
    return c(b) if c(b) == c(best) else f"{c(b)} → {c(best)}"


def gmean(xs):
    xs = [x for x in xs if x]
    return math.exp(sum(map(math.log, xs)) / len(xs)) if xs else float("nan")


def main():
    p = argparse.ArgumentParser(description=__doc__.split("\n\n")[0], formatter_class=argparse.RawDescriptionHelpFormatter,
                                epilog=__doc__)
    p.add_argument("--run", action="append", required=True, help="name:<since..until | remeasured set name>")
    p.add_argument("--out", required=True)
    p.add_argument("--notes", help="markdown file appended to the report (e.g. remarks on individual kernels)")
    args = p.parse_args()

    runs = {}
    for spec in args.run:
        name, window = spec.split(":", 1)
        if ".." in window:
            since, _, until = window.partition("..")
            runs[name] = load_window(since, until or "~")
        else:
            runs[name] = load_remeasured(window)
    names = list(runs)
    tasks = sorted(set().union(*(set(r) for r in runs.values())))

    lines = ["# Model comparison on the optimization loop", ""]
    lines.append("All speedups use the strict rule of agent/selection.py: a task counts as improved only if the best "
                 "kernel's circuit differs from the baseline's and it beat the baseline in interleaved re-timing "
                 "(≥3% median gain, ≥80% batch wins). Otherwise the speedup is x1.00. Runs made before the strict "
                 "rule were re-timed under it by agent/remeasure_runs.py. Each run: 8 attempts per task, effort "
                 "medium. Raw milliseconds come from different sessions and are not compared; speedups and circuits are.")
    lines.append("")
    lines.append("Correctness is test-based: a proposal is correct only if every output is within the task's "
                 "tolerance (CKKS, 1e-6; 1e-4 for the rotation-heavy extraction and embedding tasks) or exactly "
                 "equal to the expected integers (BFV) on every test case. 'correct' counts such proposals out of 8.")
    lines.append("")

    # Per-run model line
    lines.append("| run | model | tasks | wall time | equivalent API cost |")
    lines.append("|---|---|---:|---:|---:|")
    for n in names:
        rs = runs[n]
        models = sorted({v["model"] for v in rs.values()})
        lines.append(f"| {n} | {', '.join(models)} | {len(rs)} | {sum(v['wall_s'] for v in rs.values())/60:.0f} min "
                     f"| ${sum(v['cost'] for v in rs.values()):.2f} |")
    lines.append("")

    # Main table
    last = names[-1]
    head = ("| task | " + " | ".join(f"{n} speedup | {n} correct" for n in names)
            + f" | {last} ms baseline → best | circuit (best run) |")
    sep = "|---|" + "---:|---:|" * len(names) + "---:|---|"
    for title, pred in GROUPS:
        group = [t for t in tasks if pred(t)]
        if not group:
            continue
        lines += [f"## {title}", "", head, sep]
        for t in group:
            cells = []
            for n in names:
                v = runs[n].get(t)
                if not v:
                    cells.append("— | —")
                    continue
                mark = " ✓" if v["structural"] and (v["confirmed"] is not False) and v["speedup"] >= 1.03 else ""
                cells.append(f"x{v['speedup']:.2f}{mark} | {v['correct']}/{v['attempts']}")
            # circuit from the run with the highest strict speedup
            bestrun = max((n for n in names if runs[n].get(t)), key=lambda n: runs[n][t]["speedup"], default=None)
            circ = runs[bestrun][t]["circuit"] if bestrun else ""
            v = runs[last].get(t)
            ms = f"{v['baseline_ms']:.2f} → {v['best_ms']:.2f}" if v and v["baseline_ms"] is not None else "—"
            lines.append(f"| {t} | " + " | ".join(cells) + f" | {ms} | {circ} |")
        # group summary
        sums = []
        for n in names:
            vals = [runs[n][t]["speedup"] for t in group if t in runs[n]]
            if not vals:
                sums.append("— | —")
                continue
            sums.append(f"x{gmean(vals):.3f} | {sum(runs[n][t]['correct'] for t in group if t in runs[n])}/"
                        f"{sum(runs[n][t]['attempts'] for t in group if t in runs[n])}")
        lines.append("| **geometric mean / total** | " + " | ".join(sums) + " | | |")
        lines.append("")

    # Overall
    lines += ["## Overall", "", "| metric | " + " | ".join(names) + " |", "|---|" + "---:|" * len(names)]
    common = [t for t in tasks if all(t in runs[n] for n in names)]
    rowdefs = [
        (f"geometric-mean speedup, {len(common)} tasks in every run",
         lambda rs: f"x{gmean([rs[t]['speedup'] for t in common]):.3f}"),
        ("geometric-mean speedup, all its tasks", lambda rs: f"x{gmean([v['speedup'] for v in rs.values()]):.3f}"),
        ("tasks improved (strict)", lambda rs: str(sum(1 for v in rs.values() if v['speedup'] >= 1.03))),
        ("tasks with a changed circuit", lambda rs: str(sum(1 for v in rs.values() if v['structural']))),
        ("correct proposals", lambda rs: f"{sum(v['correct'] for v in rs.values())}/{sum(v['attempts'] for v in rs.values())}"),
        ("CHEHAB errors", lambda rs: str(sum(v['chehab_errors'] for v in rs.values()))),
        ("best single speedup", lambda rs: f"x{max(v['speedup'] for v in rs.values()):.2f} ({max(rs, key=lambda t: rs[t]['speedup'])})"),
    ]
    for label, f in rowdefs:
        lines.append(f"| {label} | " + " | ".join(f(runs[n]) for n in names) + " |")
    lines.append("")
    missing = {n: sorted(set(tasks) - set(runs[n])) for n in names}
    for n, m in missing.items():
        if m:
            lines.append(f"Not in {n}: {', '.join(m)}.")
    if args.notes:
        lines += ["", Path(args.notes).read_text().rstrip()]
    Path(args.out).write_text("\n".join(lines) + "\n")
    print("\n".join(lines))
    print(f"\nwritten: {args.out}")


if __name__ == "__main__":
    main()
