#!/usr/bin/env python3
"""Offline checks of the Phase 0 protocol changes (no model, no FHE build).

  python3 -m llm_loop.tests.test_phase0

1. Sized chain: the system prompt changes only the two statements about fixed parameters; the fixed-chain
   prompt and cost model are unchanged.
2. Task sets: `main` and `trivial` partition the tasks; selection accepts names, families and sets.
3. New tasks: the generator is deterministic and the oracles reproduce its expected outputs.
4. Rescore: a final kernel that fails a repeated correctness check scores x1.000 (strict speedup).
"""
import csv
import subprocess
import sys
import tempfile
from pathlib import Path

from llm_loop import iofile, prompts, rescore
from llm_loop.static_analysis import TOTAL_PRIMES, StaticInfo
from llm_loop.tasks import GEN_IO, TASK_SETS, TASKS, TRIVIAL, select


def test_prompts_and_cost_model():
    fixed, sized = prompts.system_prompt("none", "ckks", "fixed"), prompts.system_prompt("none", "ckks", "sized")
    assert fixed == prompts.SYSTEM_PROMPT and sized != fixed
    assert sized.replace(prompts._SIZED_LEVELS, prompts._FIXED_LEVELS).replace(
        prompts._SIZED_DEPTH, prompts._FIXED_DEPTH) == fixed, "only the two chain statements may differ"
    assert prompts.system_prompt("none", "bfv", "sized") == prompts.system_prompt("none", "bfv", "fixed")
    info = StaticInfo(output_levels={"y": 4}, cost_items=[("multiply", 0), ("relinearize", 3)])
    linear = sum(c * (TOTAL_PRIMES - l) / TOTAL_PRIMES for c, l in ((4.0, 0), (24.0, 3)))
    assert abs(info.estimated_ms - linear) < 1e-9, "the fixed-chain cost model must be unchanged"
    info.sized_chain = True
    assert info.primes == 6 and info.estimated_ms < linear * 6 / TOTAL_PRIMES
    print("prompts and cost model: fixed chain unchanged, sized chain differs only where it should  OK")


def test_task_sets():
    assert len(TASKS) == 34
    assert set(TASK_SETS["main"]) | set(TRIVIAL) == set(TASKS) and not set(TASK_SETS["main"]) & set(TRIVIAL)
    assert len(TASK_SETS["main"]) == 25 and len(TASK_SETS["new"]) == 5
    assert select(["quad", "family:matmul", "set:new", "quad"]) == ["quad", "matmul_2x2", "matmul_3x3",
                                                                   *[n for n in TASK_SETS["new"] if "tree" in n]]
    for name in TASKS:  # every family has hints
        assert prompts.FAMILY_HINTS[TASKS[name].family] in prompts.HINTS
    print("task sets: main (25) and trivial (9) partition the 34 tasks  OK")


def test_new_tasks():
    for name in TASK_SETS["new"]:
        task = TASKS[name]
        for case in task.cases:
            outs = []
            for _ in range(2):
                with tempfile.TemporaryDirectory() as d:
                    stdout = subprocess.run(case.command, cwd=d, capture_output=True, text=True, check=True).stdout
                    assert int(stdout.split()[0]) == case.slot_count
                    outs.append((Path(d) / "fhe_io_example.txt").read_text())
            assert outs[0] == outs[1], f"{name}/{case.name}: generator is not deterministic"
            with tempfile.TemporaryDirectory() as d:
                (Path(d) / "f.txt").write_text(outs[0])
                io = iofile.read(Path(d) / "f.txt")
            expected = task.oracle(io)
            assert set(expected) == set(io.outputs) and all((expected[k] == io.outputs[k]).all() for k in expected)
    # held-out inputs: results stay exact (below 2^18, far under the 20-bit plaintext modulus / 2)
    for name in TASK_SETS["new"]:
        task = TASKS[name]
        with tempfile.TemporaryDirectory() as d:
            subprocess.run(task.cases[1].command, cwd=d, capture_output=True, check=True)
            dense = iofile.read(Path(d) / "fhe_io_example.txt")
        for seed in (101, 505):
            io = task.heldout(dense, seed)
            assert all(abs(v) < 2 ** 18 for vals in io.outputs.values() for v in vals), name
    assert Path(GEN_IO).exists()
    print("new tasks: deterministic inputs, oracles match the generator, held-out results stay exact  OK")


def test_strict_speedup():
    base = {"task": "t", "sha1": "b", "kernel": "baseline", "speedup": 1.0, "eval_median_ms": 10.0,
            "e2e_median_ms": 11.0, "correct": True, "fhe_max_abs_error": 1e-7, "depth": 1, "xdepth": 1,
            "levels_run": 1, "ct_ct_mul": 1, "ct_pt_mul": 0, "align_mul": 0, "rescale": 1, "mod_switch": 0,
            "compile_median_ms": 1.0, "runs": "", "repeat_checks": 16, "repeat_passes": 16,
            "repeat_worst_error": 1e-7, "reliably_correct": True}
    good = dict(base, sha1="g", kernel="k1", speedup=1.5, runs="archive/run1")
    flaky = dict(base, sha1="f", kernel="k2", speedup=2.0, repeat_passes=15, reliably_correct=False,
                 runs="archive/run2")
    runs = []
    for k, (sha, src) in enumerate((("g", "good"), ("f", "flaky")), 1):
        runs.append({"task": "t", "strategy": "archive", "run": k, "best_id": 3, "best_source": src, "calls": 8,
                     "correct_attempts": 5})
    orig = rescore.sha1
    rescore.sha1 = lambda text: {"good": "g", "flaky": "f"}[text]  # normalize() keeps these strings
    try:
        with tempfile.TemporaryDirectory() as d:
            meta = {"git": {"commit": "0" * 40, "dirty": False}, "machine": "m", "compiler": "c", "seal": "4.1",
                    "finished": "now", "ckks_chain": "sized", "protocol": {"correctness_repeats": 2}}
            rescore.report(Path(d), [base, good, flaky], runs, meta)
            rows = {int(r["run"]): r for r in csv.DictReader(open(Path(d) / "runs.csv"))}
    finally:
        rescore.sha1 = orig
    assert float(rows[1]["speedup_strict"]) == 1.5 and float(rows[2]["speedup_strict"]) == 1.0
    assert rows[2]["repeat_passes"] == "15/16"
    print("rescore: an unreliable final kernel scores x1.000 in the strict speedup  OK")


if __name__ == "__main__":
    test_prompts_and_cost_model()
    test_task_sets()
    test_new_tasks()
    test_strict_speedup()
    print("all Phase 0 checks passed")
    sys.exit(0)
