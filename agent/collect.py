#!/usr/bin/env python3
"""Collect the latest LLM run of every task into one table.

  python agent/collect.py                   # writes agent/results/summary.md and summary.csv
  python agent/collect.py --include-offline # also count runs of the offline file replayer

A run counts only if at least one model call succeeded; runs that stopped on an
API error (for example an invalid key) are listed separately.
"""
import argparse
import csv
import json
from pathlib import Path

AGENT = Path(__file__).resolve().parent
RUNS = AGENT / "runs"
OUT = AGENT / "results"

FIELDS = ["task", "model", "effort", "baseline_ms", "best_ms", "speedup", "best_attempt", "attempts",
          "correct_attempts", "baseline_levels", "best_levels", "baseline_ct_ct_mul", "best_ct_ct_mul",
          "baseline_ct_pt_mul", "best_ct_pt_mul", "baseline_rotations", "best_rotations", "wall_time_s",
          "cost_usd", "run"]


def load_runs(include_offline: bool):
    latest, failed = {}, {}
    for path in sorted(RUNS.glob("*/*/results.json")):
        r = json.loads(path.read_text())
        if r["model"] == "files" and not include_offline:
            continue
        candidates = [a for a in r["attempts"][1:] if a.get("kind") == "candidate"]
        if not candidates:
            failed[r["task"]] = (path.parent, next((a.get("error", a.get("kind")) for a in r["attempts"][1:]), ""))
            continue
        if r["task"] not in latest or r["timestamp"] > latest[r["task"]]["timestamp"]:
            latest[r["task"]] = dict(r, _path=path.parent, _candidates=candidates)
    return latest, {t: f for t, f in failed.items() if t not in latest}


def row(r):
    b, best = r["baseline"], r["best"]
    return {
        "task": r["task"], "model": r["model"], "effort": r["effort"],
        "baseline_ms": round(b["eval_time_ms"], 2), "best_ms": round(best["eval_time_ms"], 2),
        "speedup": round(r["speedup"], 3), "best_attempt": r["best_iteration"], "attempts": r["iterations"],
        "correct_attempts": sum(1 for a in r["_candidates"] if a["evaluation"]["correct"]),
        "baseline_levels": b["levels"], "best_levels": best["levels"],
        "baseline_ct_ct_mul": b["ops"].get("ct_ct_mul"), "best_ct_ct_mul": best["ops"].get("ct_ct_mul"),
        "baseline_ct_pt_mul": b["ops"].get("ct_pt_mul"), "best_ct_pt_mul": best["ops"].get("ct_pt_mul"),
        "baseline_rotations": b["ops"].get("rotate"), "best_rotations": best["ops"].get("rotate"),
        "wall_time_s": round(r["wall_time_s"]), "cost_usd": round(r["approx_cost_usd"], 4),
        "run": str(r["_path"].relative_to(AGENT)),
    }


def main():
    p = argparse.ArgumentParser(description=__doc__.split("\n\n")[0])
    p.add_argument("--include-offline", action="store_true")
    args = p.parse_args()

    latest, failed = load_runs(args.include_offline)
    rows = [row(latest[t]) for t in sorted(latest)]
    OUT.mkdir(exist_ok=True)
    with (OUT / "summary.csv").open("w", newline="") as f:
        writer = csv.DictWriter(f, fieldnames=FIELDS)
        writer.writeheader()
        writer.writerows(rows)

    lines = ["# LLM optimization results (latest run per task)", "",
             "fhe() time is the median evaluation time on the dense case. Levels, ciphertext-ciphertext "
             "multiplications (ct x ct), ciphertext-plaintext multiplications (ct x pt) and rotations are "
             "CHEHAB's counts for the baseline and for the best correct kernel.", "",
             "| task | baseline ms | best ms | speedup | best attempt | correct | levels | ct x ct | ct x pt "
             "| rotations | cost USD |",
             "|---|---:|---:|---:|---:|---:|---|---|---|---|---:|"]
    for r in rows:
        lines.append(
            f"| {r['task']} | {r['baseline_ms']} | {r['best_ms']} | x{r['speedup']:.2f} | {r['best_attempt']} | "
            f"{r['correct_attempts']}/{r['attempts']} | {r['baseline_levels']} to {r['best_levels']} | "
            f"{r['baseline_ct_ct_mul']} to {r['best_ct_ct_mul']} | {r['baseline_ct_pt_mul']} to {r['best_ct_pt_mul']} "
            f"| {r['baseline_rotations']} to {r['best_rotations']} | {r['cost_usd']} |")
    if rows:
        improved = [r for r in rows if r["speedup"] >= 1.03]
        lines += ["", f"{len(rows)} tasks; {len(improved)} improved by 3% or more. Total cost "
                      f"${sum(r['cost_usd'] for r in rows):.2f}. Models: "
                      f"{', '.join(sorted({r['model'] for r in rows}))}."]
    if failed:
        lines += ["", "Runs with no successful model call (rerun these):", ""]
        lines += [f"- {t}: {str(err).splitlines()[0][:120]}" for t, (_, err) in sorted(failed.items())]
    (OUT / "summary.md").write_text("\n".join(lines) + "\n")
    print("\n".join(lines))
    print(f"\nwritten: {(OUT / 'summary.md').relative_to(AGENT.parent)}, {(OUT / 'summary.csv').relative_to(AGENT.parent)}")


if __name__ == "__main__":
    main()
