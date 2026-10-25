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

from selection import fingerprint

AGENT = Path(__file__).resolve().parent
RUNS = AGENT / "runs"
OUT = AGENT / "results"

FIELDS = ["task", "model", "effort", "baseline_ms", "best_ms", "speedup", "best_attempt", "attempts",
          "correct_attempts", "baseline_levels", "best_levels", "baseline_ct_ct_mul", "best_ct_ct_mul",
          "baseline_ct_pt_mul", "best_ct_pt_mul", "baseline_rotations", "best_rotations", "wall_time_s",
          "cost_usd", "structural_change", "remeasured", "new_circuits", "chehab_errors", "run"]


def load_runs(include_offline: bool, since: str = "", until: str = "~"):
    latest, failed = {}, {}
    for path in sorted(RUNS.glob("*/*/results.json")):
        r = json.loads(path.read_text())
        if r["model"] == "files" and not include_offline or not since <= r["timestamp"] < until:
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
        "structural_change": fingerprint(best) != fingerprint(b),
        "remeasured": bool(r.get("final_remeasure")),
        # distinct correct circuits other than the baseline's: how much of the search was new
        "new_circuits": len({fingerprint(a["evaluation"]) for a in r["_candidates"] if a["evaluation"]["correct"]}
                            - {fingerprint(b)}),
        "chehab_errors": sum(1 for a in r["_candidates"] if a["evaluation"]["status"] == "chehab_error"),
        "run": str(r["_path"].relative_to(AGENT)),
    }


def main():
    p = argparse.ArgumentParser(description=__doc__.split("\n\n")[0])
    p.add_argument("--include-offline", action="store_true")
    p.add_argument("--out", default=str(OUT), help="directory for summary.md and summary.csv")
    p.add_argument("--since", default="", help="only runs with timestamp >= this (e.g. 20261024-1500)")
    p.add_argument("--until", default="~", help="only runs with timestamp < this")
    args = p.parse_args()
    out = Path(args.out).resolve()

    latest, failed = load_runs(args.include_offline, args.since, args.until)
    rows = [row(latest[t]) for t in sorted(latest)]
    out.mkdir(parents=True, exist_ok=True)
    with (out / "summary.csv").open("w", newline="") as f:
        writer = csv.DictWriter(f, fieldnames=FIELDS)
        writer.writeheader()
        writer.writerows(rows)

    lines = ["# LLM optimization results (latest run per task)", "",
             "fhe() time is the median evaluation time on the dense case. Levels, ciphertext-ciphertext "
             "multiplications (ct x ct), ciphertext-plaintext multiplications (ct x pt) and rotations are "
             "CHEHAB's counts for the baseline and for the best correct kernel.", "",
             "Speedup is from baseline and best timed in alternating batches when the run remeasured it "
             "(marked r); older runs only have one median each. 'circuit' says whether the best kernel's "
             "circuit differs from the baseline's; 'new' counts distinct new correct circuits in the run.", "",
             "| task | baseline ms | best ms | speedup | best attempt | correct | circuit | new | CHEHAB errors "
             "| levels | ct x ct | ct x pt | rotations | cost USD |",
             "|---|---:|---:|---:|---:|---:|---|---:|---:|---|---|---|---|---:|"]
    for r in rows:
        lines.append(
            f"| {r['task']} | {r['baseline_ms']} | {r['best_ms']} | x{r['speedup']:.2f}{' r' if r['remeasured'] else ''} | {r['best_attempt']} | "
            f"{r['correct_attempts']}/{r['attempts']} | {'changed' if r['structural_change'] else 'same'} | "
            f"{r['new_circuits']} | {r['chehab_errors']} | {r['baseline_levels']} to {r['best_levels']} | "
            f"{r['baseline_ct_ct_mul']} to {r['best_ct_ct_mul']} | {r['baseline_ct_pt_mul']} to {r['best_ct_pt_mul']} "
            f"| {r['baseline_rotations']} to {r['best_rotations']} | {r['cost_usd']} |")
    if rows:
        improved = [r for r in rows if r["speedup"] >= 1.03]
        structural = [r for r in rows if r["structural_change"]]
        lines += ["", f"{len(rows)} tasks; {len(improved)} measured 3% or more faster, {len(structural)} with a "
                      f"changed circuit; {sum(r['correct_attempts'] for r in rows)} correct attempts, "
                      f"{sum(r['new_circuits'] for r in rows)} new circuits, "
                      f"{sum(r['chehab_errors'] for r in rows)} CHEHAB errors. Total cost "
                      f"${sum(r['cost_usd'] for r in rows):.2f}. Models: "
                      f"{', '.join(sorted({r['model'] for r in rows}))}."]
    if failed:
        lines += ["", "Runs with no successful model call (rerun these):", ""]
        lines += [f"- {t}: {str(err).splitlines()[0][:120]}" for t, (_, err) in sorted(failed.items())]
    (out / "summary.md").write_text("\n".join(lines) + "\n")
    print("\n".join(lines))
    print(f"\nwritten: {(out / 'summary.md').relative_to(AGENT.parent)}, {(out / 'summary.csv').relative_to(AGENT.parent)}")


if __name__ == "__main__":
    main()
