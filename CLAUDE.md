# CHEHAB Agent Project Context

Read [`docs/agent_handoff.md`](docs/agent_handoff.md) before changing or running anything. It is the detailed, evidence-backed handoff for the current work.

## Current objective

Improve and extend the Claude/CHEHAB feedback loop. Trustworthy candidate selection (circuit fingerprints plus interleaved remeasurement) and actionable feedback are implemented and evaluated (v2, `agent/results/comparison.md`). Current priorities: confirm v2 across repeated runs and on a teammate's machine (`docs/replication.md`), then a per-run reproducibility manifest. Broader search (cross-task lessons, a check tool, parallel candidates) needs the user's approval first.

Bootstrapping and Exact Indicator Function (EIF) work is intentionally paused. Do not restart either without the user's approval.

## Repository state

- Working directory: `/Users/abyan/Documents/Senior/Capstone/fhe-agentic-optimizer`
- Work branch: `agent-v2-selection-feedback`, PR into `llm-feedback-loop` (see `docs/agent_handoff.md` section 2).
- Run evidence lives in `agent/runs/` and aggregates in `agent/results/`; keep them. `agent/runs/*/*/programs/` holds kept binaries and is Git-ignored.
- `.DS_Store` is unrelated and untracked. Do not include it in a commit.

Always inspect `git status` before editing because the state may have changed since this handoff was written.

## Safety and operating constraints

- Never print, read into logs, commit, or otherwise expose the repository-root `.env` file or its API key.
- The loop calls the model only through the Claude Code CLI (`--proposer cli`, the logged-in claude.ai subscription). The API-key path was removed; never pass `ANTHROPIC_API_KEY`/`ANTHROPIC_AUTH_TOKEN` to the CLI subprocess.
- Ask before launching live model runs; they use the user's subscription limits. A complete 26-task run takes about two hours.
- Run one loop at a time with the machine otherwise idle: loops share `build/` and the CPU being timed.
- Avoid highly parallel full builds on this laptop. Use at most two build jobs for full builds.
- Use the Python 3.10 environment at `agent/.venv`; the macOS system Python 3.7 is too old for this code.
- SEAL 4.1: on the original laptop use `/Users/abyan/Documents/Senior/Capstone/deps/seal-install`; on another machine follow `docs/replication.md`.
- Do not treat a lower one-off median as an optimization when the circuit structure is unchanged; `agent/selection.py` encodes the rule and `agent/test_selection.py` tests it.

## Coding expectations

Before coding, state the interpretation and any consequential assumptions. Make the smallest surgical change that meets the request, match local style, and verify the narrowest observable outcome. For non-trivial work, report: Assumption, Changed, Verified, and Remaining risk.

Do not commit or push unless the user explicitly asks. Do not discard or overwrite existing uncommitted work.

