"""Command-line interface driver for the LLM FHE optimization agent.

Supports both interactive multi-turn closed-loop optimization runs and single-shot
compiler invocation from within CHEHAB.
"""

import argparse
import sys
from pathlib import Path

from .agent import FHEOptimizationAgent
from .client import create_llm_client
from .grammar import extract_code_from_response, extract_vec_lengths, parse_sexpr
from .prompts import SYSTEM_PROMPT


def main():
    parser = argparse.ArgumentParser(
        description="Run LLM-Guided FHE Compiler Feedback Optimization on CHEHAB benchmarks."
    )
    parser.add_argument(
        "--benchmark",
        "-b",
        type=str,
        default="dot_product",
        help="Benchmark name to optimize (e.g. dot_product, nn_linear, nn_conv2d, nn_avgpool2d).",
    )
    parser.add_argument(
        "--slot-count",
        "-s",
        type=int,
        default=4,
        help="SIMD slot count (vector width).",
    )
    parser.add_argument(
        "--iterations",
        "-i",
        type=int,
        default=5,
        help="Number of feedback loop optimization iterations.",
    )
    parser.add_argument(
        "--provider",
        "-p",
        type=str,
        default="mock",
        choices=["mock", "gemini", "openai", "anthropic"],
        help="LLM provider backend.",
    )
    parser.add_argument(
        "--model",
        "-m",
        type=str,
        default=None,
        help="Specific LLM model identifier (e.g. gemini-2.5-flash, gpt-4o-mini).",
    )
    parser.add_argument(
        "--api-key",
        type=str,
        default=None,
        help="Optional API key override (defaults to environment variables).",
    )

    # Arguments for single-shot compiler hook (called by C++ call_llm_vectorizer)
    parser.add_argument(
        "--single-shot",
        action="store_true",
        help="Run in single-shot mode as a direct vectorizer hook for the C++ compiler.",
    )
    parser.add_argument(
        "--expression-file",
        type=str,
        help="Input scalar expression file (single-shot mode).",
    )
    parser.add_argument(
        "--output-file",
        type=str,
        help="Output vectorized code file (single-shot mode).",
    )
    parser.add_argument(
        "--vector-width",
        type=int,
        default=4,
        help="Vector width for single-shot mode.",
    )

    args = parser.parse_args()

    # Mode 1: Single-shot compiler hook invocation
    if args.single_shot:
        if not args.expression_file or not args.output_file:
            print("Error: --single-shot requires --expression-file and --output-file.", file=sys.stderr)
            sys.exit(1)

        expr_path = Path(args.expression_file)
        if not expr_path.exists():
            print(f"Error: Expression file '{args.expression_file}' not found.", file=sys.stderr)
            sys.exit(1)

        unopt_expr = expr_path.read_text().strip()
        print(f"[LLM Vectorizer] Received scalar IR expression:\n{unopt_expr}")

        client = create_llm_client(
            provider=args.provider,
            model=args.model,
            api_key=args.api_key,
            benchmark_name="generic",
            slot_count=args.vector_width,
        )

        user_prompt = (
            f"Please vectorize this CHEHAB scalar expression for vector_width={args.vector_width}:\n\n"
            f"```\n{unopt_expr}\n```\n\n"
            "PROPOSED_EXPRESSION:\n"
            "<your S-expression>"
        )

        response = client.generate(
            messages=[{"role": "user", "content": user_prompt}],
            system_prompt=SYSTEM_PROMPT,
        )

        cand_expr = extract_code_from_response(response)
        if not cand_expr:
            print(f"[LLM Vectorizer] Error: Could not extract expression from reply:\n{response}", file=sys.stderr)
            sys.exit(1)

        print(f"[LLM Vectorizer] Generated candidate:\n{cand_expr}")

        out_path = Path(args.output_file)
        with open(out_path, "w") as f:
            f.write(cand_expr + "\n")
            f.write(f"{args.vector_width} {args.vector_width} 1 1\n")

        print(f"[LLM Vectorizer] Successfully written to {args.output_file}")
        sys.exit(0)

    # Mode 2: Multi-turn closed-loop iterative optimization agent
    client = create_llm_client(
        provider=args.provider,
        model=args.model,
        api_key=args.api_key,
        benchmark_name=args.benchmark,
        slot_count=args.slot_count,
    )

    agent = FHEOptimizationAgent(
        benchmark_name=args.benchmark,
        slot_count=args.slot_count,
        client=client,
        max_iterations=args.iterations,
        verbose=True,
    )

    summary = agent.run()
    sys.exit(0)


if __name__ == "__main__":
    main()
