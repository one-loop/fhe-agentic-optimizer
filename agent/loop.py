#!/usr/bin/env python3
"""Closed-loop LLM optimization of CHEHAB CKKS kernels (proposal section 3.2, Figure 1).

  initial kernel -> [LLM proposes a kernel] -> [CHEHAB compiles, runs, measures] -> feedback -> ...

Each iteration the model sees the task, the baseline, and the result of every
earlier attempt (the conversation is append-only), and proposes a complete
kernel. A candidate counts only if it is correct on every test case; the best
correct candidate by fhe() evaluation time is kept.

  python agent/loop.py --task chebyshev_sigmoid_d7 --iterations 8
  python agent/loop.py --task all
  python agent/loop.py --task embedding --proposer files:agent/examples/embedding   # offline, no API calls

Results go to agent/runs/<task>/<timestamp>/ (summary.md, results.json, every
candidate kernel and model answer).
"""
import argparse
import json
import os
import shutil
import sys
import time
from datetime import datetime
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
from evaluate import REPO, evaluate  # noqa: E402
from prompts import SYSTEM_PROMPT, extract_kernel, feedback_turn, first_turn, no_code_turn  # noqa: E402
from tasks import TASKS  # noqa: E402

RUNS = Path(__file__).resolve().parent / "runs"
DEFAULT_MODEL = "claude-haiku-5-5"
# USD per million tokens (Claude Haiku 5.5, prompts up to 100K tokens); cache writes
# 1.25x and cache reads 0.1x the input price. Used for an approximate cost only.
PRICES = {"claude-haiku-5-5": (0.10, 0.50)}
IMPROVEMENT_MARGIN = 0.03  # timing noise: smaller gains are reported but not called improvements


class LLMProposer:
    """Asks the model for a kernel; keeps the whole conversation (append-only)."""

    def __init__(self, model: str, effort: str, max_tokens: int):
        import anthropic

        self.anthropic = anthropic
        self.client = anthropic.Anthropic()
        self.model, self.effort, self.max_tokens = model, effort, max_tokens
        self.messages = []
        self.usage = {"input_tokens": 0, "output_tokens": 0, "cache_creation_input_tokens": 0,
                      "cache_read_input_tokens": 0}

    def propose(self, user_turn: str):
        """Returns (answer text, stop reason)."""
        self.messages.append({"role": "user", "content": user_turn})
        response = self.client.messages.create(
            model=self.model,
            max_tokens=self.max_tokens,
            system=SYSTEM_PROMPT,
            messages=self.messages,
            output_config={"effort": self.effort},
            cache_control={"type": "ephemeral"},
        )
        # Keep every block (including thinking) so the history stays valid.
        self.messages.append({"role": "assistant", "content": response.content})
        for key in self.usage:
            self.usage[key] += getattr(response.usage, key, 0) or 0
        text = "".join(b.text for b in response.content if b.type == "text")
        return text, response.stop_reason

    def cost(self) -> float:
        inp, out = PRICES.get(self.model, (0.0, 0.0))
        u = self.usage
        return (u["input_tokens"] * inp + u["cache_creation_input_tokens"] * inp * 1.25
                + u["cache_read_input_tokens"] * inp * 0.1 + u["output_tokens"] * out) / 1e6


class FileProposer:
    """Offline: replays kernels from <dir>/attempt_<i>.cpp (for testing the loop without API calls)."""

    model = "files"
    usage = {}

    def __init__(self, directory: str):
        self.directory = Path(directory)
        self.i = 0

    def propose(self, user_turn: str):
        self.i += 1
        path = self.directory / f"attempt_{self.i}.cpp"
        if not path.exists():
            return "(no more files)", "end_turn"
        return f"Replayed {path.name}.\n```cpp\n{path.read_text()}```", "end_turn"

    def cost(self) -> float:
        return 0.0


def fmt_time(t):
    return f"{t:.2f}" if t is not None else "-"


def run_task(task, args, proposer) -> dict:
    stamp = datetime.now().strftime("%Y%m%d-%H%M%S")
    out = RUNS / task.name / stamp
    out.mkdir(parents=True)
    start = time.time()
    print(f"== {task.name}: evaluating the baseline", flush=True)
    baseline_src = task.baseline()
    baseline = evaluate(task, baseline_src, args.seal_prefix, args.eval_repeats)
    (out / "baseline.cpp").write_text(baseline_src)
    if not baseline.correct:
        raise RuntimeError(f"baseline of {task.name} is not correct:\n{baseline.summary()}")
    print(baseline.summary(), flush=True)

    best_ev, best_src, best_it = baseline, baseline_src, 0
    records = [{"iteration": 0, "kind": "baseline", "evaluation": baseline.to_dict(), "seconds": time.time() - start}]
    turn = first_turn(task, baseline_src, baseline.summary(), args.iterations)

    for it in range(1, args.iterations + 1):
        t0 = time.time()
        try:
            text, stop = proposer.propose(turn)
        except Exception as e:  # API errors: record and stop this task
            print(f"   attempt {it}: model call failed: {e}", flush=True)
            records.append({"iteration": it, "kind": "api_error", "error": str(e)})
            break
        (out / f"attempt_{it:02d}_answer.md").write_text(text)
        record = {"iteration": it, "stop_reason": stop}
        source = extract_kernel(text) if stop != "refusal" else None
        if source is None:
            record["kind"] = "no_code" if stop != "refusal" else "refusal"
            turn = no_code_turn(it, args.iterations)
            print(f"   attempt {it}: {record['kind']} (stop_reason {stop})", flush=True)
        else:
            (out / f"attempt_{it:02d}.cpp").write_text(source)
            ev = evaluate(task, source, args.seal_prefix, args.eval_repeats)
            record.update(kind="candidate", evaluation=ev.to_dict())
            improved = ev.correct and ev.eval_time_ms < best_ev.eval_time_ms
            significant = improved and ev.eval_time_ms < best_ev.eval_time_ms * (1 - IMPROVEMENT_MARGIN)
            if improved:
                best_ev, best_src, best_it = ev, source, it
            record["new_best"] = improved
            print(f"   attempt {it}: {ev.status:<13} time {fmt_time(ev.eval_time_ms)} ms, levels {ev.levels}, "
                  f"ops {ev.ops}{'  <- new best' if significant else ''}", flush=True)
            turn = feedback_turn(it, args.iterations, ev.summary(), baseline.eval_time_ms,
                                 best_ev.eval_time_ms, best_it)
        record["seconds"] = time.time() - t0
        records.append(record)

    (out / "best.cpp").write_text(best_src)
    result = {
        "task": task.name, "model": proposer.model, "effort": getattr(proposer, "effort", None),
        "iterations": args.iterations, "timestamp": stamp, "wall_time_s": time.time() - start,
        "baseline": baseline.to_dict(), "best": best_ev.to_dict(), "best_iteration": best_it,
        "speedup": baseline.eval_time_ms / best_ev.eval_time_ms,
        "usage": proposer.usage, "approx_cost_usd": proposer.cost(), "attempts": records,
    }
    if isinstance(proposer, LLMProposer):
        (out / "transcript.json").write_text(json.dumps(
            [{"role": m["role"], "content": m["content"] if isinstance(m["content"], str)
              else [b.to_dict() for b in m["content"]]} for m in proposer.messages], indent=1))
    (out / "results.json").write_text(json.dumps(result, indent=1))
    (out / "summary.md").write_text(summary_md(task, result, records))
    print(f"== {task.name}: baseline {fmt_time(baseline.eval_time_ms)} ms -> best {fmt_time(best_ev.eval_time_ms)} ms "
          f"(x{result['speedup']:.2f}, attempt {best_it}); results in {out.relative_to(REPO)}", flush=True)
    return result


def _note(message: str) -> str:
    """Most informative line of a failure message, for the summary table."""
    lines = [l.strip() for l in message.splitlines() if l.strip()]
    for marker in ("exception of type", "error:", "max abs error", "may not contain", "must define"):
        for line in lines:
            if marker in line:
                return line.split("exception of type std::logic_error: ")[-1][:110]
    return lines[0][:110] if lines else ""


def summary_md(task, result, records) -> str:
    b, best = result["baseline"], result["best"]
    lines = [
        f"# {task.name}: LLM optimization run {result['timestamp']}", "",
        f"Model `{result['model']}` (effort {result['effort']}), {result['iterations']} attempts, "
        f"wall time {result['wall_time_s']:.0f} s, approximate API cost ${result['approx_cost_usd']:.4f}.", "",
        "| | fhe() time (ms) | levels | (depth, mult. depth) | ct-ct mul | ct-pt mul | rotations | max error |",
        "|---|---:|---:|---|---:|---:|---:|---:|",
    ]

    def row(name, e):
        ops = e.get("ops", {})
        return (f"| {name} | {fmt_time(e.get('eval_time_ms'))} | {e.get('levels', '-')} | "
                f"({e.get('depth')}, {e.get('mult_depth')}) | {ops.get('ct_ct_mul', '-')} | "
                f"{ops.get('ct_pt_mul', '-')} | {ops.get('rotate', '-')} | "
                f"{e['max_abs_error']:.2g} |" if e.get("max_abs_error") is not None else f"| {name} | - |")

    lines += [row("baseline", b), row(f"best (attempt {result['best_iteration']})", best), "",
              f"Speedup: x{result['speedup']:.2f}.", "", "| attempt | outcome | fhe() time (ms) | note |",
              "|---:|---|---:|---|"]
    for r in records[1:]:
        e = r.get("evaluation", {})
        note = "new best" if r.get("new_best") else _note(e.get("message", "") or r.get("error", ""))
        lines.append(f"| {r['iteration']} | {e.get('status', r['kind'])} | {fmt_time(e.get('eval_time_ms'))} | "
                     f"{note.replace('|', '/')} |")
    return "\n".join(lines) + "\n"


def main():
    p = argparse.ArgumentParser(description=__doc__.split("\n\n")[0],
                                formatter_class=argparse.RawDescriptionHelpFormatter, epilog=__doc__)
    p.add_argument("--task", required=True, help=f"one of {sorted(TASKS)} or 'all'")
    p.add_argument("--iterations", type=int, default=8)
    p.add_argument("--model", default=DEFAULT_MODEL)
    p.add_argument("--effort", default="medium", choices=["low", "medium", "high", "xhigh", "max"])
    p.add_argument("--max-tokens", type=int, default=16000)
    p.add_argument("--eval-repeats", type=int, default=10, help="timed fhe() runs per evaluation")
    p.add_argument("--proposer", default="llm", help="'llm' (default) or 'files:<dir>' to replay kernels offline")
    p.add_argument("--seal-prefix", default=os.environ.get("CMAKE_PREFIX_PATH", str(Path.home() / ".local")))
    args = p.parse_args()

    names = sorted(TASKS) if args.task == "all" else [args.task]
    if any(n not in TASKS for n in names):
        p.error(f"unknown task {args.task}; known: {sorted(TASKS)}")
    if not shutil.which("cmake"):
        p.error("cmake is not on PATH")
    if not (REPO / "build" / "agent").exists():
        p.error("build/ has no agent target: configure with -DFHECO_BUILD_CKKS_BENCHMARKS=ON and build first "
                "(see agent/README.md)")

    if args.proposer == "llm" and not (os.environ.get("ANTHROPIC_API_KEY") or os.environ.get("ANTHROPIC_AUTH_TOKEN")):
        p.error("no Anthropic credentials: export ANTHROPIC_API_KEY=... (see agent/README.md), "
                "or use --proposer files:<dir> to test offline")

    results = []
    for name in names:
        if args.proposer == "llm":
            proposer = LLMProposer(args.model, args.effort, args.max_tokens)
        elif args.proposer.startswith("files:"):
            proposer = FileProposer(args.proposer.split(":", 1)[1])
        else:
            p.error("--proposer must be 'llm' or 'files:<dir>'")
        results.append(run_task(TASKS[name], args, proposer))

    print("\n| task | baseline (ms) | best (ms) | speedup | best attempt | cost (USD) |")
    print("|---|---:|---:|---:|---:|---:|")
    for r in results:
        print(f"| {r['task']} | {fmt_time(r['baseline'].get('eval_time_ms'))} | {fmt_time(r['best'].get('eval_time_ms'))} "
              f"| x{r['speedup']:.2f} | {r['best_iteration']} | {r['approx_cost_usd']:.4f} |")


if __name__ == "__main__":
    main()
