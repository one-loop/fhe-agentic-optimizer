# Replicating the LLM feedback loop on another machine

Instructions for a teammate's coding agent (and the teammate). Goal: run the v2 loop (`agent/`) on all 26 CKKS tasks with Claude Sonnet 5.5 through the teammate's own Claude subscription, then compare with the reference results in `agent/results/cli_v2/` and `agent/results/comparison.md`.

Read `CLAUDE.md`, then `agent/README.md` (how the loop works). Do not commit `.env`, `.DS_Store` or `agent/runs/*/*/programs/`.

## 1. Claude Code CLI on the subscription (no API key)

The loop calls the model with `claude -p`, so calls count against the logged-in claude.ai subscription. No API key is used or needed.

1. Install the CLI if `claude --version` fails. Use one of: `brew install --cask claude-code` (macOS), `curl -fsSL https://claude.ai/install.sh | bash`, or `npm install -g @anthropic-ai/claude-code`.
2. **The human** runs `claude auth login` in a terminal and finishes the browser sign-in with their Claude account. An agent must not enter credentials.
3. Check: `claude auth status` must show `"loggedIn": true` and `"authMethod": "claude.ai"`.
4. Smoke test: `cd /tmp && claude -p "Say OK" --tools "" --model claude-sonnet-5-5 --output-format json`. The JSON must have `"is_error": false`.
   - `401 OAuth access token has been revoked` means the stored login is stale (this happens even when the Claude desktop app itself works). Run `claude auth login` again.
   - If `ANTHROPIC_API_KEY` is exported, the loop strips it before calling the CLI, but a manual `claude -p` test would use it. Unset it for the test.

## 2. Build dependencies (once)

Needed: CMake ≥ 3.22, a C++17 compiler, Python ≥ 3.10.

Microsoft SEAL 4.1.2, installed to a prefix outside the repository:

```bash
git clone --branch v4.1.2 --depth 1 https://github.com/microsoft/SEAL.git
cmake -S SEAL -B SEAL/build -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=$PWD/seal-install \
  -DSEAL_USE_MSGSL=OFF -DSEAL_USE_ZLIB=OFF -DSEAL_USE_ZSTD=OFF
cmake --build SEAL/build --parallel 2
cmake --install SEAL/build
export SEAL_PREFIX=$PWD/seal-install
```

CHEHAB and the agent targets, from the repository root:

```bash
python3 -m venv agent/.venv
agent/.venv/bin/pip install -r agent/requirements.txt
PATH="$PWD/agent/.venv/bin:$PATH" cmake -S . -B build -G "Unix Makefiles" \
  -DCMAKE_PREFIX_PATH=$SEAL_PREFIX -DCMAKE_BUILD_TYPE=Release -DFHECO_BUILD_CKKS_BENCHMARKS=ON
PATH="$PWD/agent/.venv/bin:$PATH" cmake --build build --parallel 2
```

Keep `--parallel` at 2 or so on a laptop; heavier builds have hung machines.

## 3. Checks without model calls

```bash
agent/.venv/bin/python -m unittest agent/test_selection.py          # 14 tests, all OK
PATH="$PWD/agent/.venv/bin:$PATH" agent/.venv/bin/python agent/loop.py --task chebyshev_sigmoid_d7 \
  --iterations 6 --proposer files:agent/examples/chebyshev_sigmoid_d7 --seal-prefix $SEAL_PREFIX
```

The offline replay must end with attempt 6 as `new_best` (confirmed in alternating batches). Then delete its run directory (`agent/runs/chebyshev_sigmoid_d7/<newest>`; its `results.json` says `"model": "files"`), so it does not mix with real runs.

## 4. One live task, then the full run

Ask the human before each live run; it uses their subscription limits.

```bash
PATH="$PWD/agent/.venv/bin:$PATH" agent/.venv/bin/python agent/loop.py --task extract_sparse --iterations 8 --seal-prefix $SEAL_PREFIX
```

Expected: about 1.4x, with ct-pt multiplications 3→2 and rotations 3→2. Then the full run (about 2 hours, roughly $11 of equivalent API usage on the subscription):

```bash
date +%Y%m%d-%H%M%S   # note the start timestamp, e.g. 20261101-1400
PATH="$PWD/agent/.venv/bin:$PATH" agent/.venv/bin/python agent/loop.py --task all --iterations 8 \
  --seal-prefix $SEAL_PREFIX 2>&1 | tee /tmp/loop_full.log
agent/.venv/bin/python agent/collect.py --out agent/results/replication_<name> --since <start timestamp>
```

Defaults equal the reference configuration: `claude-sonnet-5-5`, effort `medium`, 8 attempts, 10 timed runs. Run only one loop at a time, and keep the machine otherwise idle: concurrent work skews the timings.

## 5. Compare and report

Raw milliseconds differ between machines; compare speedups and circuits.

- Per task, compare the best circuit (levels, ct-ct, ct-pt, rotations) and speedup with `agent/results/cli_v2/summary.md`. Expect the 11 Chebyshev tasks and `extract_sparse` to change circuit and the other 14 to stay at x1.00. Individual Chebyshev results vary from run to run (for example x1.19 to x1.42 on `selu`), so compare the totals line and the geometric mean too.
- Any `speedup` above 1.00 must be marked `r` (remeasured) in the summary. A task listed under "Runs with no successful model call" points to a CLI or login problem.
- Report the summary, the totals line, wall time, and anything that failed or looked inconsistent. Commit `agent/runs/*/<your timestamps>/` and `agent/results/replication_<name>/` to a branch and open a PR.
