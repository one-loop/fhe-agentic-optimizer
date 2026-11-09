#!/usr/bin/env python3
"""Batch benchmark runner for LLM-Guided FHE Compiler Optimization.

Runs multi-turn closed-loop optimization experiments across the benchmark suite
using Claude Haiku 5.5 (or any specified model), logs all trajectories, and aggregates
comparative results into unified CSV and Markdown tables.

Capstone Project: Optimizing Fully Homomorphic Encryption Code Using LLM-Guided Compiler Feedback
NYU Abu Dhabi
"""

import argparse
import csv
import json
import os
import platform
import sys
import time
from datetime import datetime
from pathlib import Path
from typing import List, Dict, Any, Optional

from llm_agent.agent import FHEOptimizationAgent
from llm_agent.client import create_llm_client, load_env_file


# Benchmark registry with recommended slot counts, schemes, and categories
BENCHMARK_SUITE = [
    # 1. Neural Network Core Operators (BFV)
    {"name": "nn_linear", "slot_count": 4, "category": "Neural Network", "scheme": "BFV"},
    {"name": "nn_conv2d", "slot_count": 8, "category": "Neural Network", "scheme": "BFV"},
    {"name": "nn_avgpool2d", "slot_count": 8, "category": "Neural Network", "scheme": "BFV"},

    # 2. Linear Algebra & Arithmetic Primitives (BFV)
    {"name": "dot_product", "slot_count": 4, "category": "Linear Algebra", "scheme": "BFV"},
    {"name": "matrix_mul", "slot_count": 4, "category": "Linear Algebra", "scheme": "BFV"},
    {"name": "l2_distance", "slot_count": 4, "category": "Distance Metric", "scheme": "BFV"},
    {"name": "hamming_dist", "slot_count": 4, "category": "Distance Metric", "scheme": "BFV"},

    # 3. Regression Kernels (BFV)
    {"name": "lin_reg", "slot_count": 4, "category": "Regression", "scheme": "BFV"},
    {"name": "poly_reg", "slot_count": 4, "category": "Regression", "scheme": "BFV"},

    # 4. Image Processing & Spatial Convolutions (BFV)
    {"name": "box_blur", "slot_count": 4, "category": "Image Processing", "scheme": "BFV"},
    {"name": "gx_kernel", "slot_count": 4, "category": "Image Processing", "scheme": "BFV"},
    {"name": "gy_kernel", "slot_count": 4, "category": "Image Processing", "scheme": "BFV"},
    {"name": "roberts_cross", "slot_count": 4, "category": "Image Processing", "scheme": "BFV"},

    # 5. CKKS Neural Network & DL Activation / Normalization Operators
    {"name": "quad_ckks", "slot_count": 8, "category": "CKKS Activation", "scheme": "CKKS"},
    {"name": "batchnorm1d_ckks", "slot_count": 8, "category": "CKKS Normalization", "scheme": "CKKS"},
    {"name": "batchnorm2d_ckks", "slot_count": 8, "category": "CKKS Normalization", "scheme": "CKKS"},
    {"name": "chebyshev_sigmoid_d5_ckks", "slot_count": 9, "category": "CKKS Polynomial Activation", "scheme": "CKKS"},
    {"name": "chebyshev_relu_d2_ckks", "slot_count": 9, "category": "CKKS Polynomial Activation", "scheme": "CKKS"},
    {"name": "chebyshev_gelu_d7_ckks", "slot_count": 9, "category": "CKKS Polynomial Activation", "scheme": "CKKS"},
    {"name": "relu_ckks", "slot_count": 9, "category": "CKKS Minimax Sign", "scheme": "CKKS"},

    # 6. CKKS Arithmetic & Embedding Primitives
    {"name": "add_ckks", "slot_count": 8, "category": "CKKS Elementwise", "scheme": "CKKS"},
    {"name": "mul_ckks", "slot_count": 8, "category": "CKKS Elementwise", "scheme": "CKKS"},
    {"name": "add_plain_ckks", "slot_count": 8, "category": "CKKS Elementwise", "scheme": "CKKS"},
    {"name": "mul_plain_ckks", "slot_count": 8, "category": "CKKS Elementwise", "scheme": "CKKS"},
    {"name": "extract_ckks", "slot_count": 8, "category": "CKKS Extraction", "scheme": "CKKS"},
    {"name": "embedding_ckks", "slot_count": 8, "category": "CKKS Embedding", "scheme": "CKKS"},
]


def parse_args():
    parser = argparse.ArgumentParser(
        description="Run LLM-guided optimization experiments across CHEHAB BFV & CKKS benchmarks."
    )
    parser.add_argument(
        "--suite",
        type=str,
        default="nn",
        choices=["nn", "ckks", "bfv", "core", "all"],
        help="Predefined benchmark suite: 'nn' (DL operators), 'ckks' (all CKKS kernels), 'bfv' (all BFV kernels), 'core' (DL + matmul + dot), 'all' (all 26 kernels).",
    )
    parser.add_argument(
        "--benchmarks",
        "-b",
        type=str,
        default=None,
        help="Comma-separated custom benchmark list (e.g. 'quad_ckks,chebyshev_sigmoid_d5_ckks,nn_linear'). Overrides --suite.",
    )
    parser.add_argument(
        "--iterations",
        "-i",
        type=int,
        default=8,
        help="Number of LLM optimization turns/attempts per benchmark (default: 8).",
    )
    parser.add_argument(
        "--provider",
        "-p",
        type=str,
        default="anthropic",
        choices=["anthropic", "gemini", "openai", "mock"],
        help="LLM provider backend (default: anthropic).",
    )
    parser.add_argument(
        "--model",
        "-m",
        type=str,
        default="claude-haiku-5-5",
        help="Model identifier (default: claude-haiku-5-5, can also use claude-sonnet-5-5, haiku, sonnet).",
    )
    parser.add_argument(
        "--output-tag",
        type=str,
        default=None,
        help="Results sub-directory name under results/ (default: <model>_<timestamp>).",
    )
    parser.add_argument("--warmup", type=int, default=1, help="BFV: unmeasured runs of the generated main.")
    parser.add_argument("--repeats", type=int, default=5, help="BFV: measured runs of the generated main.")
    parser.add_argument("--ckks-processes", type=int, default=3, help="CKKS: measured processes.")
    parser.add_argument("--ckks-eval-repeats", type=int, default=5, help="CKKS: timed fhe() runs per process.")
    parser.add_argument("--ckks-tolerance", type=float, default=1e-6, help="CKKS: max abs error accepted.")
    return parser.parse_args()


def filter_benchmarks(args) -> List[Dict[str, Any]]:
    """Determine which benchmarks to run based on CLI arguments."""
    if args.benchmarks:
        selected_names = [b.strip() for b in args.benchmarks.split(",") if b.strip()]
        selected = []
        for name in selected_names:
            match = next((b for b in BENCHMARK_SUITE if b["name"] == name), None)
            if match:
                selected.append(match)
            else:
                is_ckks = "ckks" in name
                selected.append({
                    "name": name,
                    "slot_count": 8 if is_ckks else 4,
                    "category": "Custom",
                    "scheme": "CKKS" if is_ckks else "BFV",
                })
        return selected

    if args.suite == "nn":
        return [
            b for b in BENCHMARK_SUITE
            if b["category"] in ("Neural Network", "CKKS Activation", "CKKS Normalization", "CKKS Polynomial Activation", "CKKS Minimax Sign")
        ]
    elif args.suite == "ckks":
        return [b for b in BENCHMARK_SUITE if b.get("scheme") == "CKKS"]
    elif args.suite == "bfv":
        return [b for b in BENCHMARK_SUITE if b.get("scheme") == "BFV"]
    elif args.suite == "core":
        return [
            b for b in BENCHMARK_SUITE
            if b["name"] in ("nn_linear", "dot_product", "quad_ckks", "batchnorm1d_ckks", "chebyshev_sigmoid_d5_ckks")
        ]
    else:  # all
        return BENCHMARK_SUITE



def _fmt_ms(v):
    return f"{v:.2f}" if isinstance(v, (int, float)) else "—"


def _fmt_x(v):
    return f"{v:.2f}x" if isinstance(v, (int, float)) else "—"


def summarize(bench_info: Dict[str, Any], summary: Dict[str, Any]) -> Dict[str, Any]:
    """Turn one agent summary into a table row. Only measured, verified numbers are reported."""
    unopt = summary.get("unoptimized_baseline") or {}
    egraph = summary.get("egraph_baseline") or {}
    best = summary.get("best_result") or {}
    scheme = summary.get("scheme", bench_info.get("scheme"))
    row = {
        "benchmark": bench_info["name"],
        "scheme": scheme,
        "category": bench_info["category"],
        "slot_count": bench_info["slot_count"],
        "unoptimized_latency_ms": unopt.get("execution_latency_ms") if unopt.get("success") else None,
        "egraph_latency_ms": egraph.get("execution_latency_ms") if egraph.get("success") else None,
        "egraph_note": "" if egraph.get("success") else (
            "n/a (vectorizer is BFV-only)" if egraph.get("stage_failed") == "not_applicable"
            else f"failed ({egraph.get('stage_failed')})" if egraph else ""),
        "llm_best_latency_ms": best.get("execution_latency_ms") if best.get("success") else None,
        "speedup_vs_unoptimized": summary.get("speedup_vs_unoptimized"),
        "speedup_vs_egraph": summary.get("speedup_vs_egraph"),
        "egraph_speedup_vs_unoptimized": summary.get("egraph_speedup_vs_unoptimized"),
        "valid_candidates": f"{summary.get('valid_candidates', 0)}/{summary.get('iterations')}",
        "best_depth": f"({best.get('depth')}, {best.get('multiplicative_depth')})" if best else "",
        "best_ops": ", ".join(f"{k}={v}" for k, v in sorted((best.get("op_counts") or {}).items()) if v),
        "best_check": best.get("correctness_detail", ""),
        "best_noise_budget_bits": best.get("remaining_noise_budget") if best and scheme == "BFV" else None,
        "best_ckks_max_abs_error": best.get("ckks_max_abs_error") if best and scheme == "CKKS" else None,
        "best_ckks_levels": best.get("ckks_levels_consumed") if best and scheme == "CKKS" else None,
        "best_candidate_expr": best.get("candidate_expr", ""),
        "result_file": summary.get("_result_file", ""),
    }
    if summary.get("harness_error"):
        row["status"] = "SETUP_FAILED: " + summary["harness_error"][:160]
    elif best.get("success"):
        row["status"] = "OK"
    else:
        row["status"] = "NO_VERIFIED_CANDIDATE"
    return row


def main():
    args = parse_args()
    load_env_file()

    benchmarks_to_run = filter_benchmarks(args)
    tag = args.output_tag or f"{args.model}_{datetime.now().strftime('%Y%m%d_%H%M%S')}"
    repo_root = Path(__file__).resolve().parent
    run_dir = repo_root / "results" / tag
    run_dir.mkdir(parents=True, exist_ok=True)
    csv_path = run_dir / "suite_summary.csv"
    md_path = run_dir / "suite_summary.md"

    compiler_kwargs = {
        "exec_warmup": args.warmup,
        "exec_repeats": args.repeats,
        "ckks_processes": args.ckks_processes,
        "ckks_eval_repeats": args.ckks_eval_repeats,
        "ckks_tolerance": args.ckks_tolerance,
    }

    print("=" * 85)
    print(" CHEHAB LLM-GUIDED COMPILER OPTIMIZATION BENCHMARK SUITE")
    print(f" Provider/model: {args.provider} / {args.model}   Iterations: {args.iterations}")
    print(f" Benchmarks: {[b['name'] for b in benchmarks_to_run]}")
    print(f" Output: {run_dir}")
    print("=" * 85, flush=True)

    rows = []
    suite_start = time.perf_counter()
    for idx, bench_info in enumerate(benchmarks_to_run, start=1):
        name, slots = bench_info["name"], bench_info["slot_count"]
        print("\n" + "#" * 85)
        print(f" [{idx}/{len(benchmarks_to_run)}] {name} ({bench_info['category']}, slots={slots})")
        print("#" * 85, flush=True)
        try:
            client = create_llm_client(provider=args.provider, model=args.model, benchmark_name=name, slot_count=slots)
            agent = FHEOptimizationAgent(
                benchmark_name=name, slot_count=slots, client=client, max_iterations=args.iterations,
                repo_root=str(repo_root), verbose=True, compiler_kwargs=compiler_kwargs, results_tag=tag,
            )
            summary = agent.run()
            summary["_result_file"] = agent.result_file.name if agent.result_file else ""
            rows.append(summarize(bench_info, summary))
        except Exception as e:  # report, never substitute numbers
            import traceback
            traceback.print_exc()
            rows.append({"benchmark": name, "scheme": bench_info["scheme"], "category": bench_info["category"],
                         "slot_count": slots, "status": f"ERROR: {str(e)[:160]}"})

    total_s = time.perf_counter() - suite_start
    fields = ["benchmark", "scheme", "category", "slot_count", "status", "unoptimized_latency_ms",
              "egraph_latency_ms", "egraph_note", "llm_best_latency_ms", "speedup_vs_unoptimized",
              "speedup_vs_egraph", "egraph_speedup_vs_unoptimized", "valid_candidates", "best_depth", "best_ops",
              "best_noise_budget_bits", "best_ckks_max_abs_error", "best_ckks_levels", "best_check",
              "best_candidate_expr", "result_file"]
    with open(csv_path, "w", newline="") as f:
        w = csv.DictWriter(f, fieldnames=fields, extrasaction="ignore")
        w.writeheader()
        w.writerows(rows)

    md = [
        "# LLM-Guided FHE Optimization: Benchmark Suite",
        f"- **Date**: {datetime.now().strftime('%Y-%m-%d %H:%M:%S')}",
        f"- **Model**: `{args.model}` ({args.provider}), {args.iterations} iterations per benchmark",
        f"- **Host**: {platform.system()} {platform.machine()} {platform.mac_ver()[0] or platform.release()}, "
        "Microsoft SEAL 4.1.1",
        f"- **Wall-clock**: {total_s / 60:.1f} min",
        "",
        "## How the numbers were obtained",
        "- Every latency is measured: the program is compiled by CHEHAB, the generated SEAL code is built and run.",
        f"- BFV latency: median end-to-end time of the generated `main` (parse + encode + encrypt + evaluate), "
        f"{args.warmup} warmup + {args.repeats} measured runs.",
        f"- CKKS latency: median over {args.ckks_processes} processes of the eval-only median "
        f"(1 warmup + {args.ckks_eval_repeats} runs of `fhe()`).",
        "- Correctness is verified for every reported program. BFV: decrypted output slots must equal the "
        "benchmark's scalar expression evaluated mod t on the generator's inputs (cross-checked against the "
        "generator's expected outputs). CKKS: decrypted output vs the generator's expected output, "
        f"max abs error <= {args.ckks_tolerance:g}. Only verified candidates can be 'best'.",
        "- CKKS has no e-graph baseline: CHEHAB's vectorizer is BFV-only.",
        "- '—' means no valid measurement exists; no value is ever substituted.",
        "",
        "## Results",
        "",
        "| Benchmark | Scheme | Slots | Unoptimized (ms) | CHEHAB e-graph (ms) | Best LLM (ms) | LLM vs unopt | "
        "LLM vs e-graph | e-graph vs unopt | Verified candidates | Status |",
        "| :--- | :--- | ---: | ---: | ---: | ---: | ---: | ---: | ---: | :---: | :--- |",
    ]
    for r in rows:
        eg = _fmt_ms(r.get("egraph_latency_ms")) if r.get("egraph_latency_ms") is not None else (r.get("egraph_note") or "—")
        md.append(
            f"| `{r['benchmark']}` | {r['scheme']} | {r['slot_count']} | {_fmt_ms(r.get('unoptimized_latency_ms'))} | "
            f"{eg} | {_fmt_ms(r.get('llm_best_latency_ms'))} | {_fmt_x(r.get('speedup_vs_unoptimized'))} | "
            f"{_fmt_x(r.get('speedup_vs_egraph'))} | {_fmt_x(r.get('egraph_speedup_vs_unoptimized'))} | "
            f"{r.get('valid_candidates', '—')} | {r['status']} |"
        )
    md += ["", "## Best verified candidates", ""]
    for r in rows:
        md.append(f"### `{r['benchmark']}`")
        if r.get("best_candidate_expr"):
            extra = (f"noise budget {r['best_noise_budget_bits']} bits" if r["scheme"] == "BFV"
                     else f"max abs error {r['best_ckks_max_abs_error']:.2e}, {r['best_ckks_levels']} levels")
            md += [f"- Depth {r['best_depth']}; SEAL ops: {r['best_ops']}; {extra}",
                   f"- Check: {r['best_check']}", "```lisp", r["best_candidate_expr"], "```", ""]
        else:
            md += [f"- {r['status']}", ""]
    md_path.write_text("\n".join(md) + "\n")

    print("\n" + "=" * 120)
    print(f"{'Benchmark':<26} {'Unopt':>10} {'E-graph':>10} {'LLM':>10} {'vs unopt':>9} {'vs egraph':>10} {'valid':>6}  status")
    for r in rows:
        print(f"{r['benchmark']:<26} {_fmt_ms(r.get('unoptimized_latency_ms')):>10} "
              f"{_fmt_ms(r.get('egraph_latency_ms')):>10} {_fmt_ms(r.get('llm_best_latency_ms')):>10} "
              f"{_fmt_x(r.get('speedup_vs_unoptimized')):>9} {_fmt_x(r.get('speedup_vs_egraph')):>10} "
              f"{r.get('valid_candidates', '—'):>6}  {r['status']}")
    print(f"Suite finished in {total_s / 60:.1f} min. Summary: {md_path}")


if __name__ == "__main__":
    main()
