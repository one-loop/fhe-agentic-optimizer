#!/usr/bin/env python3
"""Unattended Phase-2 campaign: the follow-up experiments of the pilot, run one
after another without supervision, each under the approved pilot protocol
(8 model calls per run, every invocation counts; task order rotated across runs;
strategy order alternating; final kernel = the strategy's own selection; neutral
re-scoring of every final kernel at the end of each experiment).

Experiments, in priority order:
  1. chebyshev_rest   the other 6 Chebyshev activations, archive vs linear, hints none, 3 runs
  2. pilot_runs_4_5   2 more runs of the 3 pilot tasks (5 per strategy in total), re-scored together
  3. hints_full       the 3 pilot tasks with --hints full (identities + worked examples)
  4. static_feedback  the 3 pilot tasks with --feedback static (cost-model latency only)
  5. quad_batchnorm   Quad, BatchNorm1d/2d, 1 run each: does the agent leave optimal kernels intact?
  6. sonnet_archive   the 9 Chebyshev tasks with Claude Sonnet 5.5, archive only, hints none, 3 runs
  7. nn_haiku, nn_sonnet, sonnet_quad_batchnorm  the BFV nn_* tasks (3 runs) and Quad, BatchNorm (1 run), archive only
  8. opus_chebyshev, opus_quad_batchnorm, opus_nn  the same tasks with Claude Opus 5.5

Unattended behaviour: if Claude's usage limit is reached, or the model cannot be
reached, the campaign waits and retries (an interrupted run is redone from
scratch; completed runs are kept). Progress: <runs>/campaign/status.md and
campaign.log. Rerunning the campaign resumes where it stopped.

  python3 -m llm_loop.experiments.campaign                   # the campaign
  python3 -m llm_loop.experiments.campaign --proposer replay:llm_loop/tests/replay --only chebyshev_rest \
      --runs-root <scratch> --results-root <scratch> --skip-rescore      # offline check
"""
import argparse
import json
import subprocess
import sys
import time
from datetime import datetime
from pathlib import Path

REPO = Path(__file__).resolve().parents[2]
NN = ["nn_linear", "nn_avgpool2d", "nn_conv2d"]
QUAD_BN = ["quad", "batchnorm1d", "batchnorm2d"]
PILOT = ["chebyshev_gelu_d7", "chebyshev_sigmoid_d7", "chebyshev_elu_d7"]
REST = ["chebyshev_silu_d7", "chebyshev_selu_d7", "chebyshev_softplus_d7", "chebyshev_mish_d7",
        "chebyshev_hardshrink_d7", "chebyshev_sigmoid_d5"]
EXPERIMENTS = [
    {"name": "chebyshev_rest", "tasks": REST, "runs": [1, 2, 3], "hints": "none", "feedback": "execution",
     "out": "phase2-chebyshev-rest", "results": "phase2_chebyshev_rest"},
    {"name": "pilot_runs_4_5", "tasks": PILOT, "runs": [4, 5], "hints": "none", "feedback": "execution",
     "out": "phase2-pilot", "results": "phase2_pilot_5runs"},
    {"name": "hints_full", "tasks": PILOT, "runs": [1, 2, 3], "hints": "full", "feedback": "execution",
     "out": "phase2-hints-full", "results": "phase2_hints_full"},
    {"name": "static_feedback", "tasks": PILOT, "runs": [1, 2, 3], "hints": "none", "feedback": "static",
     "out": "phase2-static-feedback", "results": "phase2_static_feedback"},
    {"name": "quad_batchnorm", "tasks": ["quad", "batchnorm1d", "batchnorm2d"], "runs": [1], "hints": "none",
     "feedback": "execution", "out": "phase2-quad-batchnorm", "results": "phase2_quad_batchnorm"},
    {"name": "sonnet_archive", "tasks": PILOT + REST, "runs": [1, 2, 3], "hints": "none", "feedback": "execution",
     "out": "phase2-sonnet", "results": "phase2_sonnet", "model": "claude-sonnet-5-5", "strategies": ["archive"]},
    {"name": "nn_haiku", "tasks": NN, "runs": [1, 2, 3], "hints": "none", "feedback": "execution",
     "out": "phase2-nn-haiku", "results": "phase2_nn_haiku", "strategies": ["archive"]},
    {"name": "nn_sonnet", "tasks": NN, "runs": [1, 2, 3], "hints": "none", "feedback": "execution",
     "out": "phase2-nn-sonnet", "results": "phase2_nn_sonnet", "model": "claude-sonnet-5-5", "strategies": ["archive"]},
    {"name": "sonnet_quad_batchnorm", "tasks": QUAD_BN, "runs": [1], "hints": "none", "feedback": "execution",
     "out": "phase2-sonnet-quad-batchnorm", "results": "phase2_sonnet_quad_batchnorm", "model": "claude-sonnet-5-5",
     "strategies": ["archive"]},
    {"name": "opus_chebyshev", "tasks": PILOT + REST, "runs": [1, 2, 3], "hints": "none", "feedback": "execution",
     "out": "phase2-opus", "results": "phase2_opus", "model": "claude-opus-5-5", "strategies": ["archive"]},
    {"name": "opus_quad_batchnorm", "tasks": QUAD_BN, "runs": [1], "hints": "none", "feedback": "execution",
     "out": "phase2-opus-quad-batchnorm", "results": "phase2_opus_quad_batchnorm", "model": "claude-opus-5-5",
     "strategies": ["archive"]},
    {"name": "opus_nn", "tasks": NN, "runs": [1, 2, 3], "hints": "none", "feedback": "execution",
     "out": "phase2-nn-opus", "results": "phase2_nn_opus", "model": "claude-opus-5-5", "strategies": ["archive"]},
]
WAIT_LIMIT_S = 30 * 60      # after a usage limit
WAIT_ERROR_S = 10 * 60      # after the model could not be reached or a crash
MAX_WAIT_S = 12 * 3600      # per step; then the campaign stops (likely a weekly limit or a sign-in problem)


def task_order(tasks, run):
    """Rotate the task order across runs (the pilot's rotation, generalised to 6 tasks)."""
    shift = ((run - 1) % 3) * max(1, len(tasks) // 3)
    return tasks[shift:] + tasks[:shift]


def strategy_order(run):
    return ["archive", "linear"] if run % 2 else ["linear", "archive"]


def strategies(e, run):
    return [s for s in strategy_order(run) if s in e.get("strategies", ("archive", "linear"))]


class Campaign:
    def __init__(self, args):
        self.args = args
        self.runs_root, self.results_root = Path(args.runs_root), Path(args.results_root)
        self.dir = self.runs_root / "campaign"
        self.dir.mkdir(parents=True, exist_ok=True)
        self.log_path = self.dir / "campaign.log"
        self.events = []

    def log(self, msg):
        line = f"{datetime.now().isoformat(timespec='seconds')} {msg}"
        print(line, flush=True)
        with open(self.log_path, "a") as f:
            f.write(line + "\n")
        self.events.append(line)

    def status(self, current):
        done, total = 0, 0
        lines = ["# Phase-2 campaign status", "", f"Updated {datetime.now().isoformat(timespec='seconds')}. "
                 f"Now: {current}", "", "| experiment | completed runs | planned runs | results |",
                 "|---|---:|---:|---|"]
        for e in self.selected():
            planned = len(e["tasks"]) * len(e["runs"]) * len(e.get("strategies", ("archive", "linear")))
            completed = sum((self.runs_root / e["out"] / s / t / f"run{r}" / "result.json").exists()
                            for t in e["tasks"] for r in e["runs"] for s in e.get("strategies", ("archive", "linear")))
            res = self.results_root / e["results"] / "phase2_pilot.md"
            lines.append(f"| {e['name']} | {completed} | {planned} | {res if res.exists() else '-'} |")
            done, total = done + completed, total + planned
        lines += ["", f"Total: {done}/{total} runs ({8 * done}/{8 * total} model calls).", "",
                  "Last events:", "", *[f"    {l}" for l in self.events[-12:]]]
        (self.dir / "status.md").write_text("\n".join(lines) + "\n")

    def selected(self):
        """Without --only: the original campaign, not the later single-model experiments."""
        return [e for e in EXPERIMENTS if (e["name"] in self.args.only if self.args.only else "model" not in e and "strategies" not in e)]

    def step(self, e, run, strategy):
        tasks = task_order(e["tasks"], run)
        out = self.runs_root / e["out"] / strategy
        expected = [out / t / f"run{run}" / "result.json" for t in tasks]
        cmd = [sys.executable, "-m", "llm_loop.loop", "--tasks", *tasks, "--strategy", strategy,
               "--hints", e["hints"], "--feedback", e["feedback"], "--iterations", "8", "--runs", str(run),
               "--final-rounds", "0", "--proposer", self.args.proposer, "--model", e.get("model", "claude-haiku-5-5"), "--out", str(out),
               "--work", str(REPO / "build" / "llm_loop" / "campaign")]
        waited = 0
        while not all(p.exists() for p in expected):
            self.log(f"{e['name']}: run {run} {strategy} on {' '.join(tasks)}")
            self.status(f"{e['name']} run {run} {strategy}")
            proc = subprocess.run(cmd, cwd=REPO, capture_output=True, text=True)
            with open(self.dir / f"{e['name']}.loop.log", "a") as f:
                f.write(proc.stdout + proc.stderr)
            if all(p.exists() for p in expected):
                break
            text = proc.stdout + proc.stderr
            wait = WAIT_LIMIT_S if "STOPPED_USAGE_LIMIT" in text else WAIT_ERROR_S
            reason = ("usage limit" if "STOPPED_USAGE_LIMIT" in text else "model unreachable"
                      if "STOPPED_PROPOSER_UNAVAILABLE" in text else f"loop exited {proc.returncode}")
            if waited + wait > MAX_WAIT_S:
                self.log(f"STOPPING: {reason} persisted for {waited / 3600:.1f} h; rerun the campaign to resume. "
                         f"Last output: {text.strip()[-300:]}")
                self.status("stopped")
                raise SystemExit(2)
            self.log(f"{reason}; waiting {wait // 60} min before retrying (waited {waited / 60:.0f} min so far)")
            self.status(f"waiting ({reason})")
            time.sleep(wait)
            waited += wait

    def rescore(self, e):
        res = self.results_root / e["results"]
        if self.args.skip_rescore or (res / "phase2_pilot.md").exists():
            return
        self.log(f"{e['name']}: neutral re-scoring")
        self.status(f"{e['name']} re-scoring")
        proc = subprocess.run([sys.executable, "-m", "llm_loop.rescore", "--runs", str(self.runs_root / e["out"]),
                               "--tasks", *e["tasks"], "--out", str(res),
                               "--work", str(REPO / "build" / "llm_loop" / "campaign-rescore")],
                              cwd=REPO, capture_output=True, text=True)
        with open(self.dir / f"{e['name']}.rescore.log", "a") as f:
            f.write(proc.stdout + proc.stderr)
        self.log(f"{e['name']}: re-scoring {'done' if proc.returncode == 0 else f'FAILED ({proc.returncode})'}")

    def run(self):
        self.log(f"campaign start: {[e['name'] for e in self.selected()]}, proposer {self.args.proposer}")
        for e in self.selected():
            for run in e["runs"]:
                for strategy in strategies(e, run):
                    self.step(e, run, strategy)
            self.rescore(e)
        self.log("campaign finished")
        self.status("finished")


def main():
    p = argparse.ArgumentParser(description=__doc__.split("\n\n")[0])
    p.add_argument("--proposer", default="claude-code")
    p.add_argument("--only", nargs="*", help="experiment names to run (default: all, in order)")
    p.add_argument("--runs-root", default=str(REPO / "llm_loop" / "runs"))
    p.add_argument("--results-root", default=str(REPO / "llm_loop" / "results"))
    p.add_argument("--skip-rescore", action="store_true")
    Campaign(p.parse_args()).run()


if __name__ == "__main__":
    main()
