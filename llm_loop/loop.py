#!/usr/bin/env python3
"""Closed-loop LLM optimization of CHEHAB CKKS kernels (proposal section 3, Figure 1).

  baseline kernel -> LLM proposes a kernel -> CHEHAB compiles, verifies, measures -> feedback -> ...

  python3 -m llm_loop.loop --tasks chebyshev_gelu_d7 --iterations 8
  python3 -m llm_loop.loop --tasks all --runs 5                 # single-run and best-of-5, as in ComPilot
  python3 -m llm_loop.loop --tasks chebyshev_gelu_d7 --proposer replay:llm_loop/tests/replay   # offline

Strategies (same budget: --iterations model calls per task and run):
  linear   ComPilot-style dialogue, the proposal's design: the model sees every
           earlier attempt and its feedback and proposes the next kernel.
  archive  research-backed search: an archive of every evaluated kernel; the
           first quarter of the budget explores from the baseline with different
           hints, then each call refines a parent chosen by Thompson sampling
           (REx) and is shown one faster kernel for inspiration (FunSearch
           best-shot prompting); one repair turn per illegal kernel; duplicates
           are not re-evaluated; a static cost-model pre-screen skips the
           encrypted run of kernels predicted to be clearly slower.

Feedback (the proposal's open design question, section 1):
  execution  measured fhe() latency (default);
  static     latency from the static cost model only (the encrypted run still
             checks correctness, and measured times are recorded for analysis).

Results: <out>/<task>/run<k>/ (prompts, answers, kernels, attempts.jsonl,
result.json) and <out>/summary.md, summary.csv. The best kernel of every run
is re-timed against the baseline with the neutral scorer (score.py).
"""
import argparse
import csv
import hashlib
import json
import re
import shutil
import statistics
import subprocess
import sys
import time
from dataclasses import dataclass
from datetime import datetime
from pathlib import Path
from typing import List, Optional

import numpy as np

from llm_loop import prompts
from llm_loop.evaluate import WORK, Evaluation, Evaluator
from llm_loop.proposers import ClaudeCodeProposer, ProposerUnavailable, ReplayProposer, UsageLimitReached
from llm_loop.score import score
from llm_loop.tasks import REPO, TASKS, Task

RUNS = Path(__file__).resolve().parent / "runs"
REPAIRABLE = {"rejected", "compile_error", "chehab_error", "build_error", "run_error"}
THOMPSON_C = 20.0  # REx's recommended exploration constant


@dataclass
class Entry:
    id: int
    parent: Optional[int]
    source: str
    idea: str
    ev: Optional[Evaluation]
    outcome: str
    refinements: int = 0
    repaired: bool = False
    is_repair: bool = False

    @property
    def correct(self) -> bool:
        return self.ev is not None and self.ev.correct


def normalize(source: str) -> str:
    code = re.sub(r"//[^\n]*", "", re.sub(r"/\*.*?\*/", " ", source, flags=re.S))
    return re.sub(r"\s+", "", code)


class TaskRun:
    def __init__(self, task: Task, args, proposer, evaluator: Evaluator, baseline: Evaluation, run: int,
                 out: Path):
        self.task, self.args, self.proposer, self.evaluator = task, args, proposer, evaluator
        self.out, self.run = out, run
        self.rng = np.random.default_rng(args.seed + 1000 * run)
        self.system = prompts.system_prompt(args.hints, task.scheme)
        self.hints = prompts.HINTS[prompts.FAMILY_HINTS[task.family]]
        self.entries: List[Entry] = [Entry(0, None, task.baseline(), "baseline", baseline, "baseline")]
        self.seen = {normalize(task.baseline()): 0}
        self.calls, self.model_seconds, self.cost_usd = 0, 0.0, 0.0
        self.models_used = set()
        self.tokens = {"input": 0, "output": 0, "thinking": 0}
        self.dialogue: List[tuple] = []  # linear strategy: (role, text)

    # ---------------------------------------------------------------- helpers
    def latency(self, ev: Evaluation) -> Optional[float]:
        return ev.estimated_ms if self.args.feedback == "static" else ev.eval_time_ms

    @property
    def baseline(self) -> Entry:
        return self.entries[0]

    @property
    def best(self) -> Entry:
        """Fastest correct kernel; a later kernel replaces the current best only when it is faster by
        more than the noise margin, so timing noise does not produce phantom improvements."""
        best = self.entries[0]
        for e in self.entries[1:]:
            if e.correct and self.latency(e.ev) < self.latency(best.ev) * (1 - self.args.margin):
                best = e
        return best

    def describe(self, e: Entry) -> str:
        if e.correct:
            return prompts.metrics(e.ev, self.baseline.ev, self.best.ev, self.args.feedback_label, self.task.scheme)
        return prompts.failure(e.ev) if e.ev is not None else e.outcome

    def history(self, limit: int = 10) -> str:
        rows = []
        for e in self.entries[-limit:]:
            rows.append({"id": e.id, "outcome": e.outcome, "idea": e.idea,
                         "latency": self.latency(e.ev) if e.correct else None,
                         "xdepth": e.ev.xdepth if e.correct else "-", "static": e.ev.static if e.correct else {}})
        return prompts.history_table(rows)

    def evaluate(self, source: str) -> tuple:
        """(Evaluation or None, outcome) with the cascade: dedup, static pre-screen, full run."""
        key = normalize(source)
        if key in self.seen:
            return None, f"duplicate of #{self.seen[key]}"
        best = self.best.ev
        if self.args.prescreen > 0 and self.args.feedback == "execution" and best.estimated_ms:
            pre = self.evaluator.evaluate(self.task, source, static_only=True, tag="candidate")
            if pre.status != "ok":
                return pre, pre.status
            if pre.estimated_ms > best.estimated_ms * (1 + self.args.prescreen):
                pre.status = "predicted_slower"
                pre.message = (f"Not run on encrypted inputs: the static cost model predicts "
                               f"{pre.estimated_ms:.1f} ms, more than the best kernel's {best.estimated_ms:.1f} ms "
                               f"(operations: {pre.static}).")
                return pre, "predicted_slower"
        ev = self.evaluator.evaluate(self.task, source, heldout=True, tag="candidate")
        if ev.status == "wrong_output" and "heldout" in ev.message.split("\n")[0]:
            ev.status = "heldout_fail"
            ev.message = ("Correct on the visible test inputs but wrong on hidden random inputs: the kernel relies "
                          "on properties of the test values instead of computing the task's function.")
        return ev, ev.status

    def ask(self, prompt: str, it: int) -> str:
        (self.out / f"prompt_{it:02d}.md").write_text(prompt)
        reply = self.proposer.propose(self.system, prompt)
        self.calls += 1
        self.model_seconds += reply.seconds
        self.cost_usd += reply.cost_usd or 0.0
        self.models_used.update(reply.usage.get("models", []))
        u = reply.usage
        self.tokens["input"] += (u.get("input_tokens") or 0) + (u.get("cache_creation_input_tokens") or 0) \
            + (u.get("cache_read_input_tokens") or 0)
        self.tokens["output"] += u.get("output_tokens") or 0
        self.tokens["thinking"] += (u.get("output_tokens_details") or {}).get("thinking_tokens") or 0
        with open(self.out / "calls.jsonl", "a") as f:
            f.write(json.dumps({"iteration": it, "seconds": reply.seconds, "usage": reply.usage,
                                "estimated_cost_usd": reply.cost_usd, "error": reply.error}) + "\n")
        (self.out / f"answer_{it:02d}.md").write_text(reply.text if not reply.error else f"ERROR: {reply.error}")
        return reply.text if not reply.error else ""

    def record(self, it: int, parent: Optional[int], text: str, is_repair: bool = False) -> Entry:
        source = prompts.extract_kernel(text)
        if source is None:
            entry = Entry(len(self.entries), parent, "", prompts.idea_of(text) or "(no answer)", None, "no_code")
        else:
            (self.out / f"kernel_{it:02d}.cpp").write_text(source)
            ev, outcome = self.evaluate(source)
            entry = Entry(len(self.entries), parent, source, prompts.idea_of(text), ev, outcome, is_repair=is_repair)
            if source and normalize(source) not in self.seen:
                self.seen[normalize(source)] = entry.id
        self.entries.append(entry)
        with open(self.out / "attempts.jsonl", "a") as f:
            f.write(json.dumps({"iteration": it, "id": entry.id, "parent": parent, "outcome": entry.outcome,
                                "repair": is_repair, "idea": entry.idea,
                                "evaluation": entry.ev.to_dict() if entry.ev else None}) + "\n")
        lat = f"{self.latency(entry.ev):.1f} ms" if entry.correct else ""
        print(f"   [{self.task.name} run {self.run}] call {it}: #{entry.id} from #{parent} {entry.outcome} {lat}",
              flush=True)
        return entry

    # -------------------------------------------------------------- strategies
    def run_linear(self):
        b = self.baseline
        first = (f"{prompts.task_header(self.task)}\n\n# Baseline kernel\n{prompts.code_block(b.source)}\n\n"
                 f"# Baseline result\n{self.describe(b)}\n\nYou have {self.args.iterations} attempts. "
                 "Propose a faster kernel that computes the same outputs.")
        self.dialogue = [("user", first)]
        for it in range(1, self.args.iterations + 1):
            prompt = "\n\n".join(f"## {'Compiler' if role == 'user' else 'You (earlier answer)'}\n{text}"
                                 for role, text in self.dialogue) if len(self.dialogue) > 1 else first
            text = self.ask(prompt, it)
            self.dialogue.append(("assistant", text or "(no answer)"))
            entry = self.record(it, self.best.id, text)
            left = self.args.iterations - it
            best = self.best
            feedback = (f"# Result of attempt {it}\n{self.describe(entry)}\n\nBest so far: #{best.id} "
                        f"({'the baseline' if best.id == 0 else f'attempt {best.id}'}).")
            if left:
                feedback += (f"\n{left} attempt(s) left. Propose an improved kernel (start from the best one if "
                             "this attempt was worse or failed).")
            self.dialogue.append(("user", feedback))

    def thompson_parent(self) -> Entry:
        base = self.latency(self.baseline.ev)
        draws = []
        for e in (e for e in self.entries if e.correct):
            h = float(np.clip((base / self.latency(e.ev) - 0.9) / 0.6, 0.0, 1.0))
            draws.append((self.rng.beta(1 + THOMPSON_C * h, 1 + THOMPSON_C * (1 - h) + e.refinements), e.id))
        return self.entries[max(draws)[1]]

    def archive_prompt(self, parent: Entry, hint: str) -> str:
        parts = [prompts.task_header(self.task),
                 f"# Kernel to improve (#{parent.id}{', the baseline' if parent.id == 0 else ''})\n"
                 f"{prompts.code_block(parent.source)}\n\n## Its result\n{self.describe(parent)}"]
        better = [e for e in self.entries if e.correct and e.id != parent.id
                  and self.latency(e.ev) < self.latency(parent.ev)]
        if better:
            insp = min(better, key=lambda e: self.latency(e.ev))
            parts.append(f"# A faster kernel found earlier (#{insp.id}), for inspiration\n"
                         f"{prompts.code_block(insp.source)}\n\n## Its result\n{self.describe(insp)}")
        if len(self.entries) > 1:
            parts.append("# Attempts so far (most recent last)\n" + self.history())
        parts.append(f"# This attempt\n{hint}\nPropose one kernel that improves on #{parent.id}. "
                     "Return the idea and the complete kernel.")
        return "\n\n".join(parts)

    def repair_prompt(self, failed: Entry) -> str:
        parent = self.entries[failed.parent]
        return "\n\n".join([
            prompts.task_header(self.task),
            f"# Your kernel #{failed.id} (derived from #{parent.id}) was not accepted\n"
            f"{prompts.code_block(failed.source)}\n\n## Compiler verdict\n{prompts.failure(failed.ev)}",
            f"# The kernel it started from (#{parent.id})\n{prompts.code_block(parent.source)}\n\n"
            f"## Its result\n{self.describe(parent)}",
            "# This attempt\nFix the problem, keeping the idea if it is sound, or propose another improvement of "
            f"#{parent.id}. Return the idea and the complete kernel."])

    def run_archive(self):
        iters = self.args.iterations
        breadth = min(iters, max(2, round(0.25 * iters)))
        pending: Optional[Entry] = None
        for it in range(1, iters + 1):
            if pending is not None:
                prompt, parent_id, is_repair = self.repair_prompt(pending), pending.parent, True
                pending.repaired = True
                pending = None
            else:
                if it <= breadth:
                    parent, hint = self.baseline, self.hints[(it - 1) % len(self.hints)]
                else:
                    parent, hint = self.thompson_parent(), self.hints[int(self.rng.integers(len(self.hints)))]
                parent.refinements += 1
                prompt, parent_id, is_repair = self.archive_prompt(parent, hint), parent.id, False
            entry = self.record(it, parent_id, self.ask(prompt, it), is_repair)
            if entry.outcome in REPAIRABLE and not is_repair and entry.source:
                pending = entry

    # -------------------------------------------------------------------- run
    def execute(self) -> dict:
        started_at = datetime.now().isoformat(timespec="seconds")
        start = time.time()
        (self.run_linear if self.args.strategy == "linear" else self.run_archive)()
        search_seconds = time.time() - start
        if self.calls != self.args.iterations:  # every invocation counts; no free calls
            raise RuntimeError(f"{self.task.name} run {self.run}: {self.calls} calls, budget {self.args.iterations}")
        best = self.best  # the strategy's own selection; re-scoring measures it, never re-selects
        (self.out / "best.cpp").write_text(best.source)
        rescored = None
        if best.id != 0 and self.args.final_rounds > 0:
            rescored = score(self.task, {"best": best.source}, self.evaluator, rounds=self.args.final_rounds)
        outcomes = [e.outcome for e in self.entries[1:]]
        failures = {k: outcomes.count(k) for k in ("rejected", "compile_error", "chehab_error", "build_error",
                                                   "run_error", "wrong_output", "heldout_fail", "no_code")}
        return {
            "task": self.task.name, "owner": self.task.owner, "run": self.run, "strategy": self.args.strategy,
            "started_at": started_at,
            "feedback": self.args.feedback, "hints": self.args.hints, "iterations": self.args.iterations,
            "proposer": self.proposer.describe(), "models_used": sorted(self.models_used), "calls": self.calls, "model_seconds": self.model_seconds,
            "estimated_cost_usd": self.cost_usd, "tokens": self.tokens, "search_seconds": search_seconds,
            "wall_seconds": time.time() - start, "git": self.args.git,
            "best_id": best.id, "best_is_repair": best.is_repair,
            "baseline": self.baseline.ev.to_dict(), "best": best.ev.to_dict(),
            "speedup_in_loop": self.baseline.ev.eval_time_ms / best.ev.eval_time_ms,
            "rescored": rescored,
            "outcomes": {o: outcomes.count(o) for o in sorted(set(outcomes))},
            "duplicates": sum(o.startswith("duplicate") for o in outcomes),
            "predicted_slower": outcomes.count("predicted_slower"), "failures": failures,
            "best_source_sha1": hashlib.sha1(best.source.encode()).hexdigest(),
            "correct_attempts": sum(e.correct for e in self.entries[1:]),
        }


# ---------------------------------------------------------------------------
def summarize(out: Path, results: List[dict]) -> None:
    rows = []
    for r in results:
        b, best = r["baseline"], r["best"]
        k = (r.get("rescored") or {}).get("kernels", {})
        speedup = k.get("best", {}).get("speedup") if k else r["speedup_in_loop"]
        rows.append({
            "task": r["task"], "owner": r["owner"], "run": r["run"], "strategy": r["strategy"],
            "feedback": r["feedback"], "hints": r["hints"], "calls": r["calls"],
            "correct_attempts": r["correct_attempts"], "best_attempt": r["best_id"],
            "L_baseline_ms": k.get("baseline", {}).get("median_ms", b["eval_time_ms"]) if k else b["eval_time_ms"],
            "L_best_ms": k.get("best", {}).get("median_ms", best["eval_time_ms"]) if k else best["eval_time_ms"],
            "speedup": speedup, "speedup_source": "re-timed" if k else "in-loop (not neutral)",
            "D_baseline": b["xdepth"], "D_best": best["xdepth"],
            "levels_baseline": b["static"]["levels"], "levels_best": best["static"]["levels"],
            "C_baseline_ms": b["compile_time_ms"], "C_best_ms": best["compile_time_ms"],
            "ctct_baseline": b["static"]["ct_ct_mul"], "ctct_best": best["static"]["ct_ct_mul"],
            "rot_baseline": b["static"]["rotate"], "rot_best": best["static"]["rotate"],
            "model_seconds": round(r["model_seconds"], 1), "wall_seconds": round(r["wall_seconds"], 1),
        })
    with open(out / "summary.csv", "w", newline="") as f:
        w = csv.DictWriter(f, fieldnames=list(rows[0]))
        w.writeheader()
        w.writerows(rows)
    lines = ["# LLM loop results", "",
             "Speedups are " + ("in-loop measurements, NOT neutral re-timing (run rescore.py)"
                                if rows[0]["speedup_source"] != "re-timed" else "neutral re-timing") + ".",
             "L = fhe() latency (median), "
             "D = multiplicative depth (Quantifier xdepth), C = CHEHAB compilation time.", "",
             "| task | owner | run | calls | correct | best | L base (ms) | L best (ms) | speedup | D | levels | "
             "C (ms) | ct x ct | rot |", "|---|---|---:|---:|---:|---:|---:|---:|---:|---|---|---|---|---|"]
    for r in rows:
        lines.append(f"| {r['task']} | {r['owner']} | {r['run']} | {r['calls']} | {r['correct_attempts']} | "
                     f"#{r['best_attempt']} | {r['L_baseline_ms']:.1f} | {r['L_best_ms']:.1f} | x{r['speedup']:.2f} | "
                     f"{r['D_baseline']}->{r['D_best']} | {r['levels_baseline']}->{r['levels_best']} | "
                     f"{r['C_baseline_ms']:.2f}->{r['C_best_ms']:.2f} | {r['ctct_baseline']}->{r['ctct_best']} | "
                     f"{r['rot_baseline']}->{r['rot_best']} |")
    by_task = {}
    for r in rows:
        by_task.setdefault(r["task"], []).append(r["speedup"])
    if any(len(v) > 1 for v in by_task.values()):
        lines += ["", "| task | runs | mean single-run speedup | best of runs |", "|---|---:|---:|---:|"]
        for t, v in by_task.items():
            lines.append(f"| {t} | {len(v)} | x{statistics.mean(v):.2f} | x{max(v):.2f} |")
    (out / "summary.md").write_text("\n".join(lines) + "\n")


def git_state() -> dict:
    def git(*a):
        return subprocess.run(["git", *a], cwd=REPO, capture_output=True, text=True).stdout.strip()
    return {"commit": git("rev-parse", "HEAD"), "branch": git("rev-parse", "--abbrev-ref", "HEAD"),
            "dirty": bool(git("status", "--porcelain", "--untracked-files=no"))}


def make_proposer(args, task: Task):
    if args.proposer.startswith("replay:"):
        root = Path(args.proposer.split(":", 1)[1])
        return ReplayProposer(root / task.name if (root / task.name).is_dir() else root)
    if args.proposer == "claude-code":
        return ClaudeCodeProposer(args.model, args.effort)
    raise SystemExit(f"unknown proposer {args.proposer}")


def main():
    p = argparse.ArgumentParser(description=__doc__.split("\n\n")[0], epilog=__doc__,
                                formatter_class=argparse.RawDescriptionHelpFormatter)
    p.add_argument("--tasks", nargs="+", default=["all"], help="task names, 'all', or family:<family>")
    p.add_argument("--strategy", choices=["archive", "linear"], default="archive")
    p.add_argument("--iterations", type=int, default=8, help="model calls per task and run")
    p.add_argument("--runs", type=int, default=1, help="independent runs per task (best-of-N)")
    p.add_argument("--proposer", default="claude-code", help="'claude-code' or 'replay:<dir>'")
    p.add_argument("--model", default="claude-haiku-5-5")
    p.add_argument("--effort", default="medium", choices=["low", "medium", "high", "xhigh", "max"])
    p.add_argument("--feedback", choices=["execution", "static"], default="execution")
    p.add_argument("--hints", choices=["full", "identities", "none"], default="full")
    p.add_argument("--prescreen", type=float, default=0.15,
                   help="skip the encrypted run when the cost model predicts > (1 + x) * best; 0 disables")
    p.add_argument("--margin", type=float, default=0.02,
                   help="a kernel must be this much faster than the best to replace it (timing noise)")
    p.add_argument("--final-rounds", type=int, default=5, help="neutral re-timing rounds of each run's best kernel")
    p.add_argument("--seed", type=int, default=0)
    p.add_argument("--out", type=Path, help="results directory (default llm_loop/runs/<timestamp>)")
    p.add_argument("--work", type=Path, default=WORK / "loop",
                   help="build directory; give concurrent runs different ones")
    args = p.parse_args()
    args.feedback_label = "measured" if args.feedback == "execution" else "static"

    names = []
    for t in args.tasks:
        if t == "all":
            names += list(TASKS)
        elif t.startswith("family:"):
            names += [n for n, task in TASKS.items() if task.family == t.split(":", 1)[1]]
        elif t in TASKS:
            names.append(t)
        else:
            p.error(f"unknown task {t}; known: {sorted(TASKS)}")
    out = args.out or RUNS / datetime.now().strftime("%Y%m%d-%H%M%S")
    out.mkdir(parents=True, exist_ok=True)
    args.git = git_state()
    with open(out / "invocations.jsonl", "a") as f:  # one line per invocation (resumed runs append)
        f.write(json.dumps({"time": datetime.now().isoformat(timespec="seconds"), "argv": sys.argv[1:],
                            **{k: str(v) for k, v in vars(args).items()}}) + "\n")

    evaluator = Evaluator(work=args.work)
    results = []
    try:
        for name in dict.fromkeys(names):
            task = TASKS[name]
            baseline = None
            for run in range(1, args.runs + 1):
                run_dir = out / name / f"run{run}"
                if (run_dir / "result.json").exists():  # resume: completed runs are kept
                    results.append(json.loads((run_dir / "result.json").read_text()))
                    continue
                if run_dir.exists():
                    shutil.rmtree(run_dir)  # an interrupted run restarts from scratch
                run_dir.mkdir(parents=True)
                if baseline is None:
                    print(f"== {name}: evaluating the baseline", flush=True)
                    baseline = evaluator.evaluate(task, task.baseline(), heldout=True, tag="baseline")
                    if not baseline.correct:
                        print(f"== {name}: SKIPPED, baseline not correct: {baseline.message[:200]}")
                        break
                result = TaskRun(task, args, make_proposer(args, task), evaluator, baseline, run, run_dir).execute()
                (run_dir / "result.json").write_text(json.dumps(result, indent=1) + "\n")
                results.append(result)
                re_timed = (result.get("rescored") or {}).get("kernels", {}).get("best", {}).get("speedup")
                sp = f"x{re_timed:.2f} (re-timed)" if re_timed else f"x{result['speedup_in_loop']:.2f} (in-loop, not neutral)"
                print(f"== {name} run {run}: best #{result['best_id']}, {sp}, "
                      f"{result['correct_attempts']}/{result['calls']} correct", flush=True)
    except UsageLimitReached as e:
        print(f"\nSTOPPED_USAGE_LIMIT: Claude usage limit reached ({str(e)[:200]}). Completed runs are saved; rerun "
              f"the same command with --out {out} to resume.", file=sys.stderr)
    except ProposerUnavailable as e:
        print(f"\nSTOPPED_PROPOSER_UNAVAILABLE: the model could not be reached ({str(e)[:300]}). Completed runs "
              f"are saved; rerun the same command with --out {out} to resume.", file=sys.stderr)
    if results:
        summarize(out, results)
        print(f"\nsummary: {out / 'summary.md'}")


if __name__ == "__main__":
    main()
