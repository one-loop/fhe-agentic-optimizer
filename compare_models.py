#!/usr/bin/env python3
"""Cross-Model Comparative Evaluation Runner for LLM-Guided FHE Compiler Optimization.

Runs side-by-side experiments comparing Claude Haiku 5.5 and Claude Sonnet 5.5
(or any specified model pair) across BFV and CKKS benchmark kernels. Aggregates
comparative metrics (Execution Latency, Multiplicative Depth, Noise/Precision,
Speedup vs Baseline, Token Cost, and Runtime) into unified Markdown and CSV reports.

NYU Abu Dhabi - Capstone Project
"""

import argparse
import csv
import json
import os
import sys
import time
from datetime import datetime
from pathlib import Path
from typing import List, Dict, Any, Optional

from llm_agent.agent import FHEOptimizationAgent
from llm_agent.client import create_llm_client, load_env_file
from run_llm_benchmarks import BENCHMARK_SUITE, filter_benchmarks


def parse_args():
    parser = argparse.ArgumentParser(
        description="Compare Claude Haiku 5.5 vs Claude Sonnet 5.5 across BFV & CKKS benchmarks."
    )
    parser.add_argument(
        "--models",
        type=str,
        default="claude-haiku-5-5,claude-sonnet-5-5",
        help="Comma-separated model identifiers to compare (default: 'claude-haiku-5-5,claude-sonnet-5-5').",
    )
    parser.add_argument(
        "--suite",
        type=str,
        default="core",
        choices=["nn", "ckks", "bfv", "core", "all"],
        help="Benchmark suite: 'nn' (DL operators), 'ckks' (all CKKS), 'bfv' (all BFV), 'core' (DL + matmul + dot), 'all'.",
    )
    parser.add_argument(
        "--benchmarks",
        "-b",
        type=str,
        default=None,
        help="Comma-separated custom benchmark list (e.g. 'quad_ckks,chebyshev_sigmoid_d5_ckks,dot_product').",
    )
    parser.add_argument(
        "--iterations",
        "-i",
        type=int,
        default=5,
        help="Number of optimization turns per benchmark (default: 5).",
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
        "--output-tag",
        type=str,
        default=None,
        help="Custom output filename tag.",
    )
    return parser.parse_args()


def run_single_experiment(benchmark_info: Dict[str, Any], model: str, provider: str, iterations: int, repo_root: Path) -> Dict[str, Any]:
    """Run an optimization trajectory for a single benchmark and model."""
    bench_name = benchmark_info["name"]
    slot_count = benchmark_info["slot_count"]
    category = benchmark_info["category"]
    scheme = benchmark_info.get("scheme", "CKKS" if "ckks" in bench_name else "BFV")

    t_start = time.perf_counter()
    client = create_llm_client(
        provider=provider,
        model=model,
        benchmark_name=bench_name,
        slot_count=slot_count,
    )

    agent = FHEOptimizationAgent(
        benchmark_name=bench_name,
        slot_count=slot_count,
        client=client,
        max_iterations=iterations,
        repo_root=str(repo_root),
        verbose=True,
    )

    summary = agent.run()
    wall_clock = time.perf_counter() - t_start

    unopt = summary.get("unoptimized_baseline", {})
    egraph = summary.get("egraph_baseline", {})
    best = summary.get("best_result") or {}

    unopt_lat = unopt.get("execution_latency_ms", 0.0)
    egraph_lat = egraph.get("execution_latency_ms", 0.0)
    best_lat = best.get("execution_latency_ms", 0.0)
    speedup_unopt = best.get("speedup_vs_unoptimized", 1.0)
    speedup_egraph = (egraph_lat / best_lat) if (best_lat > 0 and egraph_lat > 0) else 1.0

    return {
        "benchmark": bench_name,
        "scheme": scheme,
        "category": category,
        "slot_count": slot_count,
        "model": model,
        "unoptimized_latency_ms": round(unopt_lat, 2),
        "egraph_latency_ms": round(egraph_lat, 2),
        "llm_best_latency_ms": round(best_lat, 2),
        "speedup_vs_unoptimized": round(speedup_unopt, 2),
        "speedup_vs_egraph": round(speedup_egraph, 2),
        "multiplicative_depth": best.get("multiplicative_depth", unopt.get("multiplicative_depth", 1)),
        "remaining_noise_budget": best.get("remaining_noise_budget", unopt.get("remaining_noise_budget", 0)),
        "ckks_max_abs_error": best.get("ckks_max_abs_error", unopt.get("ckks_max_abs_error", 0.0)),
        "ckks_levels_consumed": best.get("ckks_levels_consumed", unopt.get("ckks_levels_consumed", 0)),
        "best_candidate_expr": best.get("candidate_expr", "N/A"),
        "wall_clock_s": round(wall_clock, 1),
        "status": "SUCCESS" if best.get("success") else "NO_VALID_CANDIDATE",
    }


def main():
    args = parse_args()
    load_env_file()

    models = [m.strip() for m in args.models.split(",") if m.strip()]
    if len(models) < 2:
        print("[!] Warning: At least 2 models recommended for comparison. Specified:", models)

    benchmarks_to_run = filter_benchmarks(args)
    tag = args.output_tag or datetime.now().strftime("%Y%m%d_%H%M%S")

    repo_root = Path(__file__).resolve().parent
    results_dir = repo_root / "results"
    results_dir.mkdir(parents=True, exist_ok=True)

    csv_path = results_dir / f"model_comparison_{tag}.csv"
    md_path = results_dir / f"model_comparison_{tag}.md"

    print("=" * 105)
    print(" CHEHAB CROSS-MODEL COMPARATIVE EVALUATION (Haiku 5.5 vs Sonnet 5.5)")
    print("=" * 105)
    print(f" Models to compare: {models}")
    print(f" Provider:          {args.provider.upper()}")
    print(f" Iterations:        {args.iterations} per model per benchmark")
    print(f" Benchmarks ({len(benchmarks_to_run)}): {[b['name'] for b in benchmarks_to_run]}")
    print(f" Output Markdown:   {md_path.name}")
    print(f" Output CSV:        {csv_path.name}")
    print("=" * 105 + "\n")

    comparison_map: Dict[str, Dict[str, Any]] = {}
    total_start = time.perf_counter()

    for idx, bench_info in enumerate(benchmarks_to_run, start=1):
        bench_name = bench_info["name"]
        comparison_map[bench_name] = {"info": bench_info, "models": {}}

        print(f"\n=========================================================================================")
        print(f" [{idx}/{len(benchmarks_to_run)}] BENCHMARK: {bench_name} ({bench_info.get('scheme', 'CKKS')})")
        print(f"=========================================================================================")

        for model in models:
            print(f"\n---> Running Model: {model} on {bench_name} ({args.iterations} iterations)...")
            try:
                res = run_single_experiment(bench_info, model, args.provider, args.iterations, repo_root)
                comparison_map[bench_name]["models"][model] = res
            except Exception as e:
                print(f"[!] Error running {model} on {bench_name}: {e}")
                comparison_map[bench_name]["models"][model] = {
                    "model": model,
                    "llm_best_latency_ms": 0.0,
                    "speedup_vs_unoptimized": 0.0,
                    "multiplicative_depth": 0,
                    "status": f"FAILED: {e}",
                }

    total_duration = time.perf_counter() - total_start

    m1 = models[0]
    m2 = models[1] if len(models) > 1 else models[0]
    m1_short = "Haiku 5.5" if "haiku" in m1.lower() else m1
    m2_short = "Sonnet 5.5" if "sonnet" in m2.lower() else m2

    md_lines = [
        f"# Cross-Model Optimization Performance Comparison",
        f"- **Date**: {datetime.now().strftime('%Y-%m-%d %H:%M:%S')}",
        f"- **Models Evaluated**: `{m1}` vs `{m2}`",
        f"- **Provider**: `{args.provider}`",
        f"- **Iterations per benchmark**: {args.iterations}",
        f"- **Total evaluation runtime**: {total_duration/60:.1f} minutes",
        "",
        "## Comparative Benchmark Results",
        "",
        f"| Benchmark | Scheme | Baseline ($L$) | {m1_short} ($L$) | {m2_short} ($L$) | {m1_short} Speedup | {m2_short} Speedup | Depth ({m1_short} vs {m2_short}) | Latency Advantage |",
        "| :--- | :--- | :--- | :--- | :--- | :--- | :--- | :--- | :--- |",
    ]

    terminal_rows = []
    flat_csv_rows = []

    for bname, data in comparison_map.items():
        info = data["info"]
        res1 = data["models"].get(m1, {})
        res2 = data["models"].get(m2, {})

        scheme = info.get("scheme", "CKKS" if "ckks" in bname else "BFV")
        base_lat = res1.get("unoptimized_latency_ms") or res2.get("unoptimized_latency_ms", 0.0)

        l1 = res1.get("llm_best_latency_ms", 0.0)
        l2 = res2.get("llm_best_latency_ms", 0.0)
        s1 = res1.get("speedup_vs_unoptimized", 1.0)
        s2 = res2.get("speedup_vs_unoptimized", 1.0)
        d1 = res1.get("multiplicative_depth", 1)
        d2 = res2.get("multiplicative_depth", 1)

        if l1 > 0 and l2 > 0:
            if l2 < l1:
                diff_pct = ((l1 - l2) / l1) * 100
                winner = f"**{m2_short} (+{diff_pct:.1f}%)**"
            elif l1 < l2:
                diff_pct = ((l2 - l1) / l2) * 100
                winner = f"{m1_short} (+{diff_pct:.1f}%)"
            else:
                winner = "Tie"
        else:
            winner = "N/A"

        md_lines.append(
            f"| `{bname}` | {scheme} | {base_lat:.2f} ms | {l1:.2f} ms | {l2:.2f} ms | "
            f"**{s1:.2f}x** | **{s2:.2f}x** | {d1} vs {d2} | {winner} |"
        )

        terminal_rows.append((bname, scheme, f"{base_lat:.1f}", f"{l1:.1f}", f"{l2:.1f}", f"{s1:.2f}x", f"{s2:.2f}x", f"{d1}/{d2}", winner.replace("*", "")))

        flat_csv_rows.append({
            "benchmark": bname,
            "scheme": scheme,
            "category": info.get("category", ""),
            "baseline_latency_ms": base_lat,
            f"{m1}_latency_ms": l1,
            f"{m2}_latency_ms": l2,
            f"{m1}_speedup": s1,
            f"{m2}_speedup": s2,
            f"{m1}_depth": d1,
            f"{m2}_depth": d2,
            "winner": winner.replace("*", ""),
            f"{m1}_candidate": res1.get("best_candidate_expr", ""),
            f"{m2}_candidate": res2.get("best_candidate_expr", ""),
        })

    md_lines.append("\n## Candidate Circuit Analysis by Benchmark\n")
    for bname, data in comparison_map.items():
        res1 = data["models"].get(m1, {})
        res2 = data["models"].get(m2, {})
        md_lines.append(f"### `{bname}`")
        md_lines.append(f"**{m1_short} Candidate:**\n```lisp\n{res1.get('best_candidate_expr', 'N/A')}\n```")
        md_lines.append(f"**{m2_short} Candidate:**\n```lisp\n{res2.get('best_candidate_expr', 'N/A')}\n```\n")

    with open(md_path, "w") as f:
        f.write("\n".join(md_lines))

    # Write CSV
    if flat_csv_rows:
        with open(csv_path, "w", newline="") as f:
            writer = csv.DictWriter(f, fieldnames=list(flat_csv_rows[0].keys()))
            writer.writeheader()
            writer.writerows(flat_csv_rows)

    # Print Terminal Comparison Table
    print("\n" + "=" * 115)
    print(f" CROSS-MODEL PERFORMANCE COMPARISON: {m1_short.upper()} vs {m2_short.upper()}")
    print("=" * 115)
    print(f"{'Benchmark':<24} | {'Scheme':<6} | {'Base(ms)':<9} | {m1_short[:10]+'(ms)':<12} | {m2_short[:10]+'(ms)':<12} | {m1_short[:8]+' Spd':<10} | {m2_short[:8]+' Spd':<10} | {'Depth':<7} | {'Advantage':<15}")
    print("-" * 115)
    for r in terminal_rows:
        print(f"{r[0]:<24} | {r[1]:<6} | {r[2]:<9} | {r[3]:<12} | {r[4]:<12} | {r[5]:<10} | {r[6]:<10} | {r[7]:<7} | {r[8]:<15}")
    print("=" * 115)
    print(f"[✓] Comparison completed in {total_duration:.1f}s.")
    print(f"[✓] Report Markdown: {md_path}")
    print(f"[✓] Report CSV:      {csv_path}")
    print("=" * 115 + "\n")


if __name__ == "__main__":
    main()
