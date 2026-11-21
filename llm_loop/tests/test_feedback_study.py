#!/usr/bin/env python3
"""Offline checks of the feedback-study arms (no model, no FHE build: the stub evaluator of
test_feedback_ablation).

  python3 -m llm_loop.tests.test_feedback_study

1. linear-minimal: the same first prompt as linear; afterwards the model learns correctness only (no latency,
   operation counts or ranking), and the pre-screen is off.
2. linear-explore / archive-explore: Technique line required, ledger of approaches with repeat marks, the
   "new approach" instruction, and the last call may refine. Selection and budget are unchanged.
3. Diversity metric, Holm correction and the Monte Carlo sign-flip test.
"""
import math
import tempfile
from argparse import Namespace
from pathlib import Path

from llm_loop import loop, prompts
from llm_loop.experiments import compare_feedback
from llm_loop.proposers import Reply
from llm_loop.tasks import TASKS
from llm_loop.tests.test_feedback_ablation import STATIC, StubEvaluator, ev

PERFORMANCE = ["ms", "Best so far", "Operations", "latency", "x1.", "x0.", "ct x ct", "Largest estimated cost"]


class Proposer:
    def __init__(self, latencies, technique=True):
        self.latencies, self.prompts, self.systems, self.technique = list(latencies), [], [], technique

    def describe(self):
        return {"proposer": "stub"}

    def propose(self, system, prompt):
        self.prompts.append(prompt)
        self.systems.append(system)
        i = len(self.prompts)
        ms = self.latencies[i - 1]
        tag = f"{ms or 1.0} wrong" if ms is None else f"{ms}"
        head = f"Technique: approach {i % 3}\n" if self.technique else ""
        return Reply(f"{head}Idea {i}.\n```cpp\nvoid fhe() {{ const char *m = \"ms={tag}\"; }}\n```")


def run(strategy, latencies, out, prescreen=0.15):
    args = Namespace(strategy=strategy, iterations=len(latencies), seed=0, hints="none", feedback="execution",
                     feedback_label="measured", prescreen=prescreen, margin=0.02, final_rounds=0, git={},
                     ckks_chain="sized")
    proposer = Proposer(latencies)
    out.mkdir(parents=True)
    task_run = loop.TaskRun(TASKS["chebyshev_gelu_d7"], args, proposer, StubEvaluator(), ev(100.0), 1, out)
    return task_run, task_run.execute(), proposer


def feedback_part(prompt: str) -> str:
    """The compiler messages of a linear prompt after the first one (the model's own answers left out)."""
    sections = prompt.split("## ")[1:]
    compiler = [x for x in sections if x.startswith("Compiler")]
    return "\n".join(compiler[1:])


def test_minimal():
    lat = [90.0, None, 60.0, 80.0]
    with tempfile.TemporaryDirectory() as d:
        full_run, full, full_p = run("linear", lat, Path(d) / "linear")
        min_run, minimal, min_p = run("linear-minimal", lat, Path(d) / "minimal")
    assert min_run.prescreen == 0.0 and full_run.prescreen == 0.15
    assert min_p.prompts[0] == full_p.prompts[0] and min_p.systems[0] == full_p.systems[0]
    for prompt in min_p.prompts[1:]:
        fb = feedback_part(prompt)
        assert prompts.MINIMAL_CORRECT in fb or "Wrong result" in fb
        for word in PERFORMANCE:
            assert word not in fb.replace("max error", ""), f"performance information {word!r} in minimal feedback"
    assert "Best so far" in feedback_part(full_p.prompts[1])
    assert minimal["best_id"] == full["best_id"] == 3 and minimal["calls"] == 4  # selection unchanged
    print("linear-minimal: same first prompt, correctness-only feedback, no pre-screen, same selection  OK")


def test_explore():
    lat = [90.0, 90.0, 60.0, None, 80.0]
    with tempfile.TemporaryDirectory() as d:
        _, lin, lin_p = run("linear-explore", lat, Path(d) / "le")
        _, arc, arc_p = run("archive-explore", lat, Path(d) / "ae")
        _, plain, plain_p = run("archive", lat, Path(d) / "a")
    for p in (lin_p, arc_p):
        assert all(s.endswith(prompts.EXPLORE_FORMAT) for s in p.systems)
        assert prompts.EXPLORE_LAST in p.prompts[-1] and prompts.EXPLORE_INSTRUCTION not in p.prompts[-1].split(
            "# Approaches tried so far")[-1]
    assert prompts.EXPLORE_INSTRUCTION in lin_p.prompts[1] and "# Approaches tried so far" in lin_p.prompts[1]
    assert "same operation counts as #0, the baseline" in lin_p.prompts[1]  # the stub's static counts never change
    assert "approach 1" in lin_p.prompts[1]  # the technique line is shown in the ledger
    assert "approach" not in plain_p.systems[0] and "Approaches tried" not in "".join(plain_p.prompts)
    assert lin["best_id"] == 3 and lin["calls"] == len(lat) and arc["calls"] == len(lat)
    assert lin["diversity"]["techniques"][:2] == ["approach 1", "approach 2"]
    print("explore arms: technique line, ledger with repeat marks, new-approach instruction, last call free  OK")


def test_diversity_and_stats():
    s = lambda m: dict(STATIC, ct_ct_mul=m)
    attempts = [
        {"iteration": 1, "outcome": "ok", "evaluation": {"static": s(1)}},       # baseline's structure: a repeat
        {"iteration": 2, "outcome": "ok", "evaluation": {"static": s(2)}},       # new
        {"iteration": 3, "outcome": "duplicate of #2", "evaluation": None},
        {"iteration": 4, "outcome": "wrong_output", "evaluation": {"static": s(2)}},  # repeat
        {"iteration": 5, "outcome": "no_code", "evaluation": None},
        {"iteration": 6, "outcome": "ok", "evaluation": {"static": s(3)}, "technique": "BSGS"},
    ]
    d = loop.diversity(attempts, s(1))
    assert (d["distinct_kernels"], d["duplicates"], d["distinct_structures"], d["structure_repeats"],
            d["distinct_techniques"]) == (4, 1, 2, 2, 1), d
    assert compare_feedback.holm([0.01, 0.04, 0.03]) == [0.03, 0.06, 0.06]
    p = compare_feedback.sign_flip_p([0.05] * 30)
    assert p < 1e-4, p
    q = compare_feedback.sign_flip_p([0.05, -0.05] * 15)
    assert q > 0.9, q
    print("diversity, Holm correction, Monte Carlo sign-flip test  OK")


if __name__ == "__main__":
    test_minimal()
    test_explore()
    test_diversity_and_stats()
    print("all feedback-study checks passed")
