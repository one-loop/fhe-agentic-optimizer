#!/usr/bin/env python3
"""Offline checks of the no-feedback ablation (no model, no FHE build: a stub evaluator).

  python3 -m llm_loop.tests.test_feedback_ablation

1. `independent` sends the same prompt on every call, identical to `linear`'s first prompt apart from the
   attempt budget, and the prompt never contains anything about the model's own attempts.
2. `linear` does feed results back (so the two strategies really differ only in feedback).
3. `independent` still keeps the fastest correct kernel and spends exactly the call budget.
4. compare_feedback's statistics: exact sign-flip p-value and the report on synthetic runs.
"""
import json
import math
import random
import tempfile
from argparse import Namespace
from pathlib import Path

from llm_loop import loop
from llm_loop.evaluate import CaseResult, Evaluation
from llm_loop.experiments import compare_feedback
from llm_loop.proposers import Reply
from llm_loop.tasks import TASKS

STATIC = {k: 1 for k in ("levels", "ct_ct_mul", "ct_pt_mul", "align_mul", "mod_switch", "rotate", "add", "add_plain")}
MARKERS = ["# Result of attempt", "Best so far", "Your kernel #", "Compiler verdict", "Attempts so far",
           "earlier answer", "attempt(s) left"]


def ev(ms: float, correct: bool = True) -> Evaluation:
    return Evaluation(status="ok" if correct else "wrong_output", correct=correct,
                      message="" if correct else "max error 1.0 on case dense", compile_time_ms=1.0, depth=3,
                      xdepth=3, static=dict(STATIC), estimated_ms=ms,
                      cases=[CaseResult("dense", 256, passed=correct, eval_time_ms=ms)])


class StubEvaluator:
    """Latency is read from a `"ms=<value>"` string in the kernel (comments are stripped by the duplicate
    check); `wrong` in that string marks a wrong kernel."""

    def evaluate(self, task, source, static_only=False, heldout=False, tag=""):
        tag = source.split('"ms=')[1].split('"')[0]
        return ev(float(tag.split()[0]), correct="wrong" not in tag)


class RecordingProposer:
    def __init__(self, latencies):
        self.latencies, self.prompts = list(latencies), []

    def describe(self):
        return {"proposer": "recording"}

    def propose(self, system, prompt):
        self.prompts.append(prompt)
        ms = self.latencies[len(self.prompts) - 1]
        tag = f"{ms or 1.0} wrong" if ms is None else f"{ms}"
        return Reply(f"Idea {len(self.prompts)}.\n```cpp\nvoid fhe() {{ const char *m = \"ms={tag}\"; }}\n```")


def run(strategy, latencies, out):
    task = TASKS["chebyshev_gelu_d7"]
    args = Namespace(strategy=strategy, iterations=len(latencies), seed=0, hints="none", feedback="execution",
                     feedback_label="measured", prescreen=0.0, margin=0.02, final_rounds=0, git={})
    proposer = RecordingProposer(latencies)
    out.mkdir(parents=True)
    result = loop.TaskRun(task, args, proposer, StubEvaluator(), ev(100.0), 1, out).execute()
    return result, proposer.prompts


def test_loop():
    lat = [90.0, None, 60.0, 80.0, 59.5, 70.0]  # None = wrong kernel; 59.5 is within the 2% margin of 60
    with tempfile.TemporaryDirectory() as d:
        ind, ind_prompts = run("independent", lat, Path(d) / "independent")
        lin, lin_prompts = run("linear", lat, Path(d) / "linear")
    assert len(set(ind_prompts)) == 1, "independent must send one fixed prompt"
    assert ind["calls"] == len(lat) and lin["calls"] == len(lat)
    for m in MARKERS:
        assert m not in ind_prompts[0], f"feedback marker {m!r} leaked into the no-feedback prompt"
    budget = f"You have {len(lat)} attempts. "
    assert lin_prompts[0] == ind_prompts[0].replace("Propose a faster kernel", budget + "Propose a faster kernel"), \
        "call-1 prompts must match up to the budget"
    assert any("# Result of attempt" in p for p in lin_prompts[1:]), "linear must feed results back"
    assert ind["best_id"] == 3 and lin["best_id"] == 3, "the fastest correct kernel (60 ms) must be kept"
    assert ind["failures"]["wrong_output"] == 1
    print("loop: independent sends no feedback; linear does; same budget and selection  OK")


def test_stats():
    assert compare_feedback.sign_flip_p([1.0, 1.0, 1.0]) == 0.25  # 2 of 8 sign patterns as extreme
    assert compare_feedback.sign_flip_p([0.5, -0.5]) == 1.0
    random.seed(1)
    with tempfile.TemporaryDirectory() as d:
        root = Path(d) / "runs"
        for strategy, gain in (("independent", 1.0), ("linear", 1.3)):
            for task in ("t_a", "t_b"):
                for k in (1, 2, 3):
                    run_dir = root / strategy / task / f"run{k}"
                    run_dir.mkdir(parents=True)
                    best = 100.0 / (gain * random.uniform(1.1, 1.2))
                    attempts = [{"iteration": i, "evaluation": {"correct": True, "eval_time_ms": 100.0 if i < 3
                                                                else best}} for i in range(1, 5)]
                    (run_dir / "attempts.jsonl").write_text("\n".join(json.dumps(a) for a in attempts) + "\n")
                    (run_dir / "result.json").write_text(json.dumps({
                        "task": task, "strategy": strategy, "run": k, "calls": 4, "correct_attempts": 4,
                        "duplicates": 0, "best_id": 3, "failures": {}, "predicted_slower": 0,
                        "baseline": {"eval_time_ms": 100.0}, "speedup_in_loop": 100.0 / best,
                        "models_used": ["stub"]}))
        out = Path(d) / "report"
        import sys
        sys.argv = ["x", "--runs", str(root), "--out", str(out)]
        compare_feedback.main()
        text = (out / "feedback_ablation.md").read_text()
        effect = (out / "feedback_effect.csv").read_text().splitlines()[1].split(",")
        ratio, lo, hi, p = float(effect[3]), float(effect[4]), float(effect[5]), float(effect[7])
        assert 1.15 < ratio < 1.45 and lo > 1.0 and math.isclose(p, 2 / 64), (ratio, lo, hi, p)
        assert "IN-LOOP" in text and "Sanity check: call 1" in text
    print(f"compare_feedback: ratio x{ratio:.3f} [{lo:.3f}, {hi:.3f}], p = {p:.4f}  OK")


if __name__ == "__main__":
    test_loop()
    test_stats()
    print("all feedback-ablation checks passed")
