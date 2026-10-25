#!/usr/bin/env python3
"""Closed-loop LLM optimization of CHEHAB CKKS kernels (proposal section 3.2, Figure 1).

  initial kernel -> [LLM proposes a kernel] -> [CHEHAB compiles, runs, measures] -> feedback -> ...

Each iteration the model sees the task, the baseline, and the result of every
earlier attempt (the conversation is append-only), and proposes a complete
kernel. A candidate counts only if it is correct on every test case; the best
correct candidate by fhe() evaluation time is kept.

  python agent/loop.py --task chebyshev_sigmoid_d7 --iterations 8
  python agent/loop.py --task all
  python agent/loop.py --task embedding --proposer files:agent/examples/embedding   # offline, no model calls

The model is called through the Claude Code CLI on the logged-in claude.ai
subscription; no API key is used.

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
from evaluate import REPO, evaluate, snapshot, time_program  # noqa: E402
from prompts import SYSTEM_PROMPT, extract_kernel, feedback_turn, first_turn, no_code_turn  # noqa: E402
from selection import BATCHES, FINAL_BATCHES, classify, confirm, fingerprint  # noqa: E402
from tasks import TASKS  # noqa: E402

RUNS = Path(__file__).resolve().parent / "runs"
DEFAULT_MODEL = "claude-sonnet-5-5"


class ClaudeCLIProposer:
    """Asks the model through the Claude Code CLI (`claude -p`), so calls use the logged-in
    claude.ai subscription instead of an API key. The conversation lives in a CLI session that
    each turn resumes. No tools are enabled; the CLI's own system prompt is replaced by ours."""

    # Never pass API credentials, a proxy, or a parent Claude Code session's settings to the child.
    STRIP_ENV = ("ANTHROPIC_API_KEY", "ANTHROPIC_AUTH_TOKEN", "ANTHROPIC_BASE_URL", "CLAUDECODE")

    def __init__(self, model: str, effort: str, max_tokens: int):
        import tempfile

        self.model, self.effort = model, effort
        self.cli = shutil.which("claude")
        if not self.cli:
            raise RuntimeError("the `claude` CLI is not on PATH")
        # Run outside the repository so no project CLAUDE.md is picked up.
        self.cwd = tempfile.mkdtemp(prefix="chehab-cli-")
        self.env = {k: v for k, v in os.environ.items()
                    if k not in self.STRIP_ENV and not k.startswith(("CLAUDE_CODE_", "CLAUDE_AGENT_SDK"))}
        self.env["CLAUDE_CODE_MAX_OUTPUT_TOKENS"] = str(max_tokens)
        self.session_id = None
        self.messages = []
        self.usage = {"input_tokens": 0, "output_tokens": 0, "cache_creation_input_tokens": 0,
                      "cache_read_input_tokens": 0}
        self.equivalent_cost = 0.0  # what the CLI reports the calls would cost at API prices; not billed

    def propose(self, user_turn: str):
        """Returns (answer text, stop reason)."""
        import subprocess

        cmd = [self.cli, "-p", "--output-format", "json", "--model", self.model, "--effort", self.effort,
               "--system-prompt", SYSTEM_PROMPT, "--tools", "", "--strict-mcp-config", "--setting-sources", ""]
        if self.session_id:
            cmd += ["--resume", self.session_id]
        proc = subprocess.run(cmd, input=user_turn, capture_output=True, text=True, cwd=self.cwd,
                              env=self.env, timeout=900)
        try:
            out = json.loads(proc.stdout)
        except json.JSONDecodeError:
            raise RuntimeError(f"claude CLI exit {proc.returncode}: {(proc.stderr or proc.stdout)[-500:]}")
        if out.get("is_error"):
            raise RuntimeError(f"claude CLI error: {out.get('result')}")
        self.session_id = out["session_id"]
        for key in self.usage:
            self.usage[key] += out.get("usage", {}).get(key, 0) or 0
        self.equivalent_cost += out.get("total_cost_usd", 0) or 0
        text = out.get("result", "")
        self.messages += [{"role": "user", "content": user_turn}, {"role": "assistant", "content": text}]
        return text, out.get("stop_reason")

    def cost(self) -> float:
        return 0.0  # subscription usage; see equivalent_cost


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


def interleaved(task, a_dir: Path, b_dir: Path, batches: int, repeats: int):
    """fhe() medians of two kept programs, timed in alternating batches (ABBA... order)."""
    a, b = [], []
    for i in range(batches):
        pairs = [(a_dir, a), (b_dir, b)]
        for directory, acc in (pairs if i % 2 == 0 else pairs[::-1]):
            acc.append(time_program(directory, task.tolerance, repeats))
    return a, b


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
    programs = out / "programs"
    snapshot(task, programs / "baseline")

    best_ev, best_src, best_it, best_dir = baseline, baseline_src, 0, programs / "baseline"
    baseline_fp = fingerprint(baseline.to_dict())
    seen = {baseline_fp: 0}  # circuit fingerprint -> first attempt that produced it
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
            ev = evaluate(task, source, args.seal_prefix, args.eval_repeats, skip_timing=frozenset(seen))
            evd = ev.to_dict()
            record.update(kind="candidate", evaluation=evd)
            verdict, earlier = classify(evd, baseline_fp, fingerprint(best_ev.to_dict()), best_ev.eval_time_ms, seen)
            remeasure = None
            if verdict == "remeasure":
                cand_dir = programs / f"attempt_{it:02d}"
                snapshot(task, cand_dir)
                remeasure = confirm(*interleaved(task, cand_dir, best_dir, BATCHES, args.eval_repeats))
                remeasure["against"] = best_it
                if remeasure["confirmed"]:
                    verdict = "new_best"
                    best_ev, best_src, best_it, best_dir = ev, source, it, cand_dir
                else:
                    verdict = "not_confirmed"
                    shutil.rmtree(cand_dir)
            if ev.correct:
                seen.setdefault(fingerprint(evd), it)
            record.update(verdict=verdict, same_as=earlier, remeasure=remeasure, new_best=verdict == "new_best")
            extra = (f", remeasured x{remeasure['speedup']:.2f} vs attempt {remeasure['against']} "
                     f"({remeasure['wins']}/{remeasure['batches']} wins)" if remeasure else "")
            print(f"   attempt {it}: {ev.status:<13} time {fmt_time(ev.eval_time_ms)} ms, levels {ev.levels}, "
                  f"ops {ev.ops}: {verdict}{'' if earlier is None else f' (attempt {earlier})'}{extra}", flush=True)
            turn = feedback_turn(it, args.iterations, ev.summary(), verdict, earlier, remeasure,
                                 baseline, best_ev, best_it)
        record["seconds"] = time.time() - t0
        records.append(record)

    (out / "best.cpp").write_text(best_src)
    # The reported speedup: baseline and best timed in alternating batches.
    final = confirm(*interleaved(task, best_dir, programs / "baseline", FINAL_BATCHES, args.eval_repeats)) \
        if best_it else None
    result = {
        "task": task.name, "model": proposer.model, "effort": getattr(proposer, "effort", None),
        "iterations": args.iterations, "timestamp": stamp, "wall_time_s": time.time() - start,
        "baseline": baseline.to_dict(), "best": best_ev.to_dict(), "best_iteration": best_it,
        "speedup": final["speedup"] if final else 1.0, "final_remeasure": final,
        "single_run_speedup": baseline.eval_time_ms / best_ev.eval_time_ms,
        "structural_change": fingerprint(best_ev.to_dict()) != baseline_fp,
        "usage": proposer.usage, "approx_cost_usd": proposer.cost(), "attempts": records,
        "proposer": args.proposer,
    }
    if isinstance(proposer, ClaudeCLIProposer):
        result["equivalent_api_cost_usd"] = proposer.equivalent_cost
        result["cli_session_id"] = proposer.session_id
        (out / "transcript.json").write_text(json.dumps(proposer.messages, indent=1))
    (out / "results.json").write_text(json.dumps(result, indent=1))
    (out / "summary.md").write_text(summary_md(task, result, records))
    print(f"== {task.name}: baseline {fmt_time(baseline.eval_time_ms)} ms -> best {fmt_time(best_ev.eval_time_ms)} ms "
          f"(x{result['speedup']:.2f}, attempt {best_it}); results in {out.relative_to(REPO)}", flush=True)
    return result


def _note(message: str) -> str:
    """Most informative line of a failure message, for the summary table."""
    lines = [l.strip() for l in message.splitlines() if l.strip()]
    for marker in ("exception of type", "scale mismatch", "does not support", "error:", "max abs error",
                   "may not contain", "must define"):
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
              f"Speedup: x{result['speedup']:.2f} (baseline and best timed in "
              f"{FINAL_BATCHES} alternating batches; 1.00 when no candidate was confirmed faster).", "",
              "| attempt | outcome | fhe() time (ms) | verdict | note |", "|---:|---|---:|---|---|"]
    for r in records[1:]:
        e = r.get("evaluation", {})
        verdict = r.get("verdict", "") + (f" (attempt {r['same_as']})" if r.get("same_as") is not None else "")
        if r.get("remeasure"):
            verdict += f", x{r['remeasure']['speedup']:.2f} vs attempt {r['remeasure']['against']}"
        note = "" if e.get("correct") else _note(e.get("message", "") or r.get("error", ""))
        lines.append(f"| {r['iteration']} | {e.get('status', r['kind'])} | {fmt_time(e.get('eval_time_ms'))} | "
                     f"{verdict} | {note.replace('|', '/')} |")
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
    p.add_argument("--proposer", default="cli",
                   help="'cli' (default, Claude Code CLI on the logged-in subscription) "
                        "or 'files:<dir>' to replay kernels offline")
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

    if args.proposer == "cli" and not shutil.which("claude"):
        p.error("the `claude` CLI is not on PATH (log in with `claude auth login`), "
                "or use --proposer files:<dir> to test offline")

    results, failed = [], []
    for name in names:
        if args.proposer == "cli":
            proposer = ClaudeCLIProposer(args.model, args.effort, args.max_tokens)
        elif args.proposer.startswith("files:"):
            proposer = FileProposer(args.proposer.split(":", 1)[1])
        else:
            p.error("--proposer must be 'cli' or 'files:<dir>'")
        try:
            results.append(run_task(TASKS[name], args, proposer))
        except RuntimeError as e:  # a baseline that is not correct: record it and go on
            print(f"== {name}: SKIPPED. {str(e).splitlines()[0]}", flush=True)
            failed.append((name, str(e)))

    print("\n| task | baseline (ms) | best (ms) | speedup | best attempt | cost (USD) |")
    print("|---|---:|---:|---:|---:|---:|")
    for r in results:
        print(f"| {r['task']} | {fmt_time(r['baseline'].get('eval_time_ms'))} | {fmt_time(r['best'].get('eval_time_ms'))} "
              f"| x{r['speedup']:.2f} | {r['best_iteration']} | {r['approx_cost_usd']:.4f} |")
    for name, message in failed:
        print(f"SKIPPED {name}: {message.splitlines()[0]}")


if __name__ == "__main__":
    main()
