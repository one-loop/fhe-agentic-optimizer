"""Closed-loop iterative optimization controller for FHE expressions.

Implements the multi-turn feedback dialogue between the LLM agent and the CHEHAB
compiler. Only candidates whose decrypted outputs were verified correct can
become the best result; every latency is measured, never estimated.
"""

import json
import platform
import time
from datetime import datetime
from pathlib import Path
from typing import Any, Dict, List, Optional

from .client import LLMClient, create_llm_client
from .compiler_interface import CompilerInterface, EvaluationResult, HarnessError
from .grammar import extract_code_from_response
from .prompts import build_feedback_prompt, build_initial_user_prompt, system_prompt_for


def _speedup(base: Optional[EvaluationResult], best: Optional[EvaluationResult]) -> Optional[float]:
    if base is None or best is None or not base.success or not best.success:
        return None
    if base.execution_latency_ms <= 0 or best.execution_latency_ms <= 0:
        return None
    return base.execution_latency_ms / best.execution_latency_ms


class FHEOptimizationAgent:
    """Manages the iterative closed-feedback loop optimization."""

    def __init__(
        self,
        benchmark_name: str = "dot_product",
        slot_count: int = 4,
        client: Optional[LLMClient] = None,
        max_iterations: int = 5,
        repo_root: Optional[str] = None,
        verbose: bool = True,
        compiler_kwargs: Optional[Dict[str, Any]] = None,
        results_tag: Optional[str] = None,
    ):
        self.benchmark_name = benchmark_name
        self.slot_count = slot_count
        self.max_iterations = max_iterations
        self.verbose = verbose
        self.results_tag = results_tag
        self.compiler = CompilerInterface(
            repo_root=repo_root, benchmark_name=benchmark_name, slot_count=slot_count, **(compiler_kwargs or {})
        )
        self.client = client or create_llm_client(provider="mock", benchmark_name=benchmark_name, slot_count=slot_count)
        self.messages: List[Dict[str, str]] = []
        self.history: List[Dict[str, Any]] = []
        self.best_result: Optional[EvaluationResult] = None
        self.result_file: Optional[Path] = None

    def log(self, msg: str):
        if self.verbose:
            print(msg, flush=True)

    def run(self) -> Dict[str, Any]:
        started = datetime.now()
        self.log("=" * 80)
        self.log(f"FHE LLM optimization loop: '{self.benchmark_name}' (slots={self.slot_count}, "
                 f"model={getattr(self.client, 'model', type(self.client).__name__)})")
        self.log("=" * 80)

        # Step 1: inputs, baselines and reference. Any failure here aborts the benchmark.
        try:
            t0 = time.perf_counter()
            self.compiler.setup()
            self.log(f"[*] Setup (generator + baselines) took {time.perf_counter() - t0:.1f}s")
        except HarnessError as e:
            self.log(f"[!] Benchmark setup failed, no LLM iterations run: {e}")
            summary = self._summary(started, harness_error=str(e))
            self._save(summary)
            return summary

        c = self.compiler
        unopt, egraph = c.get_unoptimized_baseline(), c.get_egraph_baseline()
        self.log(f"[*] Original expression:\n    {c.unoptimized_expression}")
        self.log(f"[*] Unoptimized baseline: {unopt.execution_latency_ms:.2f} ms  ({unopt.correctness_detail})")
        if egraph.success:
            self.log(f"[*] E-graph baseline:     {egraph.execution_latency_ms:.2f} ms  ({egraph.correctness_detail})")
        else:
            self.log(f"[*] E-graph baseline unavailable: {egraph.stage_failed}: "
                     f"{egraph.error_message or egraph.correctness_detail}")
        if c.reference_result is not None:
            self.log(f"[*] Reference program via candidate path: {c.reference_result.execution_latency_ms:.2f} ms")

        system_prompt = system_prompt_for(c.scheme)
        self.messages.append({
            "role": "user",
            "content": build_initial_user_prompt(
                benchmark_name=self.benchmark_name,
                scheme=c.scheme,
                unoptimized_expr=c.unoptimized_expression,
                input_types=c.input_types,
                unopt_metrics=unopt.to_dict(),
                egraph_metrics=egraph.to_dict() if egraph.success else None,
                n_outputs=len(c.reference_slots) if c.scheme == "BFV" else None,
            ),
        })

        # Step 2: feedback loop.
        for iteration in range(1, self.max_iterations + 1):
            self.log("-" * 80)
            self.log(f"=== Iteration {iteration} of {self.max_iterations} ===")
            t0 = time.perf_counter()
            try:
                reply = self.client.generate(self.messages, system_prompt=system_prompt)
            except Exception as e:  # API failure: record it, do not invent a result
                self.log(f"[!] LLM call failed: {e}")
                self.history.append({"iteration": iteration, "llm_error": str(e)})
                break
            llm_s = time.perf_counter() - t0
            self.messages.append({"role": "assistant", "content": reply})
            self.log(f"[LLM reply ({llm_s:.1f}s)]:\n{reply.strip()}\n")

            stop_reason = getattr(self.client, "last_stop_reason", None)
            cand = extract_code_from_response(reply)
            if stop_reason == "max_tokens":
                res = EvaluationResult(success=False, candidate_expr=cand or "", scheme=c.scheme,
                                       stage_failed="response_truncated",
                                       error_message="Your reply hit the output token limit before it finished. "
                                                     "Keep the rationale short and give the complete expression.")
            elif stop_reason == "refusal":
                res = EvaluationResult(success=False, candidate_expr="", scheme=c.scheme,
                                       stage_failed="response_refused", error_message="The model declined to answer.")
            elif not cand:
                res = EvaluationResult(success=False, candidate_expr="", scheme=c.scheme,
                                       stage_failed="response_parsing",
                                       error_message="No S-expression found after 'PROPOSED_EXPRESSION:'.")
            else:
                self.log(f"[*] Candidate: {cand}")
                t0 = time.perf_counter()
                res = c.evaluate_candidate(cand, iteration=iteration)
                self.log(f"[*] Evaluation took {time.perf_counter() - t0:.1f}s")
            feedback = res.feedback_string()
            self.log(f"[Compiler feedback]:\n{feedback.strip()}")

            is_best = res.success and (
                self.best_result is None or res.execution_latency_ms < self.best_result.execution_latency_ms
            )
            if is_best:
                self.best_result = res
                self.log(f"[+] New best verified candidate: {res.execution_latency_ms:.2f} ms")
            self.history.append({
                "iteration": iteration,
                "candidate": cand,
                "llm_response": reply,
                "llm_seconds": llm_s,
                "llm_stop_reason": stop_reason,
                "llm_usage": getattr(self.client, "last_usage", None),
                "evaluation": res.to_dict(),
                "is_best": is_best,
            })
            if iteration < self.max_iterations:
                best_lat = self.best_result.execution_latency_ms if self.best_result else None
                self.messages.append({"role": "user", "content": build_feedback_prompt(iteration, feedback, best_lat)})

        summary = self._summary(started)
        self._save(summary)
        return summary

    def _summary(self, started: datetime, harness_error: Optional[str] = None) -> Dict[str, Any]:
        c = self.compiler
        unopt = c._unoptimized
        egraph = c._egraph
        best = self.best_result
        summary = {
            "benchmark": self.benchmark_name,
            "scheme": c.scheme,
            "slot_count": self.slot_count,
            "iterations": self.max_iterations,
            "model": getattr(self.client, "model", type(self.client).__name__),
            "started": started.isoformat(timespec="seconds"),
            "finished": datetime.now().isoformat(timespec="seconds"),
            "harness_error": harness_error,
            "methodology": {
                "bfv_latency": f"median end-to-end time of the generated SEAL main (parse+encode+encrypt+evaluate), "
                               f"{c.exec_warmup} warmup + {c.exec_repeats} measured runs",
                "ckks_latency": f"median over {c.ckks_processes} processes of the eval-only median "
                                f"(1 warmup + {c.ckks_eval_repeats} runs of fhe())",
                "ckks_tolerance": c.ckks_tolerance,
                "host": f"{platform.system()} {platform.machine()} {platform.mac_ver()[0] or platform.release()}",
            },
            "original_expression": c.unoptimized_expression,
            "unoptimized_baseline": unopt.to_dict() if unopt else None,
            "egraph_baseline": egraph.to_dict() if egraph else None,
            "reference_via_candidate_path": c.reference_result.to_dict() if c.reference_result else None,
            "best_result": best.to_dict() if best else None,
            "speedup_vs_unoptimized": _speedup(unopt, best),
            "speedup_vs_egraph": _speedup(egraph, best),
            "egraph_speedup_vs_unoptimized": _speedup(unopt, egraph),
            "valid_candidates": sum(1 for h in self.history if h.get("evaluation", {}).get("success")),
            "history": self.history,
        }
        self.log("\n" + "=" * 80)
        self.log(f"SUMMARY {self.benchmark_name}")
        if harness_error:
            self.log(f"  setup failed: {harness_error}")
        else:
            self.log(f"  unoptimized : {unopt.execution_latency_ms:.2f} ms")
            self.log(f"  e-graph     : {f'{egraph.execution_latency_ms:.2f} ms' if egraph.success else 'n/a'}")
            self.log(f"  best LLM    : {f'{best.execution_latency_ms:.2f} ms' if best else 'no verified candidate'}")
            self.log(f"  valid candidates: {summary['valid_candidates']}/{self.max_iterations}")
        self.log("=" * 80)
        return summary

    def _save(self, summary: Dict[str, Any]):
        results_dir = Path(self.compiler.repo_root) / "results"
        if self.results_tag:
            results_dir = results_dir / self.results_tag
        results_dir.mkdir(parents=True, exist_ok=True)
        ts = datetime.now().strftime("%Y%m%d_%H%M%S")
        self.result_file = results_dir / f"llm_opt_{self.benchmark_name}_slot{self.slot_count}_{ts}.json"
        self.result_file.write_text(json.dumps(summary, indent=2))
        self.log(f"[✓] Trajectory saved to {self.result_file.relative_to(self.compiler.repo_root)}")
