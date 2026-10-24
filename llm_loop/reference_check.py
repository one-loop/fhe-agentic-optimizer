#!/usr/bin/env python3
"""Step 2 check: hand-derived reference kernels (llm_loop/reference/).

Compiles every reference kernel through CHEHAB for the tasks it applies to,
checks it on the fixed, dense and held-out cases, and times it against the
baseline with the neutral scorer (interleaved rounds). This confirms which
headroom from the research is reachable under the DSL rules and gives the
"gap to optimum" reference for comparing LLM loops.

  python3 -m llm_loop.reference_check [--tasks ...] [--rounds 5]
"""
import argparse
import json
from pathlib import Path

from llm_loop.evaluate import Evaluator
from llm_loop.score import score
from llm_loop.tasks import TASKS

REFERENCE = Path(__file__).resolve().parent / "reference"
RESULTS = Path(__file__).resolve().parent / "results"

# reference family -> tasks it applies to (by which Chebyshev coefficients are non-negligible)
FAMILIES = {
    "even_d7": ["chebyshev_gelu_d7", "chebyshev_silu_d7", "chebyshev_softplus_d7", "chebyshev_relu_d7"],
    "odd_d7": ["chebyshev_sigmoid_d7"],
    "odd_d7_noc0": ["chebyshev_hardshrink_d7"],
    "full_d7": ["chebyshev_elu_d7", "chebyshev_selu_d7", "chebyshev_mish_d7"],
    "odd_d5": ["chebyshev_sigmoid_d5"],
    "extract_sparse": ["extract_sparse"],
}


def references_for(task_name: str):
    for family, tasks in FAMILIES.items():
        if task_name in tasks:
            for path in sorted(REFERENCE.glob(f"{family}--*.cpp")):
                yield path.stem.split("--", 1)[1], path.read_text()


def main():
    p = argparse.ArgumentParser(description=__doc__.split("\n\n")[0])
    p.add_argument("--tasks", nargs="+", default=[t for ts in FAMILIES.values() for t in ts])
    p.add_argument("--rounds", type=int, default=5)
    p.add_argument("--repeats", type=int, default=10)
    p.add_argument("--output", type=Path, default=RESULTS / "reference_check.json")
    args = p.parse_args()
    evaluator = Evaluator()
    results = []
    for name in args.tasks:
        kernels = dict(references_for(name))
        result = score(TASKS[name], kernels, evaluator, args.rounds, args.repeats)
        results.append(result)
        for tag, r in result["kernels"].items():
            s = r["static"]
            print(f"{name:<24} {tag:<18} {r['status']:<13} "
                  f"lv={s.get('levels')} ctct={s.get('ct_ct_mul')} ctpt={s.get('ct_pt_mul')} "
                  f"align={s.get('align_mul')} modsw={s.get('mod_switch')} rot={s.get('rotate')} "
                  f"err={r['max_abs_error']} med={r.get('median_ms')} x{r.get('speedup') or 0:.2f}"
                  + (f"  {r['message'][:200]!r}" if not r["correct"] else ""), flush=True)
    RESULTS.mkdir(exist_ok=True)
    args.output.write_text(json.dumps(results, indent=1) + "\n")


if __name__ == "__main__":
    main()
