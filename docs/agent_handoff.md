# Agent Handoff: CKKS Operator Optimization Loop

Last verified locally: 2026-10-25

## 1. Goal and present scope

This repository combines CKKS operators that compile through CHEHAB with an LLM feedback loop that proposes replacement CHEHAB DSL kernels. Each proposal is compiled, run on encrypted inputs, decrypted, checked against independently calculated plaintext outputs, and fed back to the model with correctness and circuit metrics.

The current phase is **not** to implement bootstrapping or EIF. Those are paused. The selection foundation is now in place (section 5b). The immediate goals are to confirm it across repeated runs and on a teammate's machine (`docs/replication.md`), then extend the search with the user's approval.

## 2. Git state

- Work branch: `agent-v2-selection-feedback`, opened as a PR into `llm-feedback-loop` (which is at `7b200e6`). It commits everything that was previously uncommitted: the CLI proposer (the API-key and `.env` path is removed from the code), the v2 selection and feedback loop, the CHEHAB scale-mismatch message, all run evidence in `agent/runs/` (the 2026-10-13 Haiku/API run, Sonnet v1 and Sonnet v2 runs), and the aggregates in `agent/results/`.
- `agent/runs/*/*/programs/` (kept binaries), `.env` and `.DS_Store` are not committed.
- The root `.env` may still exist locally and is Git-ignored. Never display or commit its contents.

Run `git status --short` and `git log --oneline -5` before making any change; this section is a checkpoint, not a substitute for inspecting current state.

## 3. Environment that worked

- Host: macOS arm64.
- Python: 3.10 virtual environment at `agent/.venv`.
- Microsoft SEAL: 4.1.2.
- SEAL source: `/Users/abyan/Documents/Senior/Capstone/deps/SEAL`.
- SEAL install prefix: `/Users/abyan/Documents/Senior/Capstone/deps/seal-install`.
- Build directory: `build`.
- CMake generator: Unix Makefiles.
- Build type: Release.
- `FHECO_BUILD_CKKS_BENCHMARKS=ON`.

The system Python is 3.7 and cannot parse code used by the evaluator. Use `agent/.venv/bin/python` for all agent commands.

The working configure command was:

```bash
PATH="$PWD/agent/.venv/bin:$PATH" cmake -S . -B build -G "Unix Makefiles" \
  -DCMAKE_PREFIX_PATH=/Users/abyan/Documents/Senior/Capstone/deps/seal-install \
  -DCMAKE_BUILD_TYPE=Release \
  -DFHECO_BUILD_CKKS_BENCHMARKS=ON
```

For a full build, limit parallelism because a previous heavy build hung and crashed the laptop:

```bash
PATH="$PWD/agent/.venv/bin:$PATH" cmake --build build --parallel 2
```

The minimal SEAL build disabled Microsoft GSL, zlib, and zstd integrations to avoid CMake 4.4 compatibility errors in bundled dependencies. The current installed prefix already works; do not rebuild it unless necessary.

## 4. Feedback loop configuration

The implementation is under `agent/`:

- `loop.py`: orchestration, model calls through `claude -p`, conversational feedback, best-candidate selection, and run artifacts.
- `prompts.py`: system/task/feedback prompts and kernel extraction.
- `evaluate.py`: CHEHAB compilation, generated SEAL build/run, decryption, correctness checking, and metrics.
- `tasks.py` and `tasks/*.cpp`: task definitions and baseline kernels.
- `collect.py`: selects the latest run per task and writes aggregate Markdown/CSV.
- `runs/`: raw run evidence.
- `results/`: aggregate output.

Reproduced live configuration:

- Model: `claude-haiku-5-5`.
- Effort: `medium`.
- Attempts: 8 per task.
- Timing: median of 10 encrypted `fhe()` evaluations per candidate.
- Coverage: all 26 CKKS tasks.
- Correctness tolerance: `1e-6` normally; `1e-4` for extraction/table/embedding because accumulated rotation error is around `1e-5`.
- Credentials (that run only): an Anthropic API key. This path has since been removed from the code.

The model receives the baseline kernel, task contract, CKKS/CHEHAB guidance, prior attempts, compile/runtime errors, correctness failures, timing, levels, depth, term count, and operation counts. It only emits CHEHAB DSL kernel code; unsafe/non-kernel constructs are rejected.

## 5. Independently reproduced evidence

The full live run completed successfully for all 26 tasks:

- 208 model proposals across 26 tasks.
- 202 proposals passed correctness.
- 4 proposals produced CHEHAB errors.
- 2 proposals produced wrong outputs; both were rejected.
- Total wall time: 5,296.88 seconds (88.28 minutes).
- Recorded approximate API cost: `$0.21270396`.
- Raw run directories: 27 total (`26` live plus `1` offline replay).
- Live transcripts: 26.

The aggregate collector currently reports 19 tasks with at least a 3% lower measured median, but that is not the same as 19 real optimizations. The credible structural improvements are the 11 Chebyshev tasks and `extract_sparse`.

Representative verified changes:

| Task | Measured speedup | Structural change |
|---|---:|---|
| `chebyshev_gelu_d7` | 1.31x | levels 7→5; ciphertext multiplications 5→3 |
| `chebyshev_silu_d7` | 1.42x | levels 7→4; ciphertext multiplications 5→3; plaintext multiplications 5→4 |
| `chebyshev_softplus_d7` | 1.39x | levels 7→4; ciphertext multiplications 5→3; plaintext multiplications 5→4 |
| `chebyshev_relu_d7` | 1.33x | levels 7→5; ciphertext multiplications 5→3 |
| `chebyshev_relu_d2` | 1.44x | plaintext multiplications 3→2 |
| `extract_sparse` | 1.39x | plaintext multiplications 3→2; rotations 3→2 |

The strongest extraction evidence is in:

```text
agent/runs/extract_sparse/20261013-163327/
```

Its dense-case median changed from `177.954 ms` to `128.098 ms`, while all correctness cases passed. The candidate retained one consumed level and reduced both rotations and ciphertext-plaintext multiplications from three to two.

Examples that look faster in the aggregate but did **not** materially change circuit shape:

- `batchnorm2d`: about 1.07x, unchanged operations.
- `elementwise_add_scalar`: about 1.31x, unchanged operations.
- `relu_composite`: about 1.05x, identical operations.
- `table_mult`: about 1.08x, identical operations.
- `embedding`: about 1.03x, still 6 plaintext multiplications and 26 rotations; its alternative packing slightly worsened reported depth from 21 to 22.

These should be treated as timing noise or alternative-but-equivalent implementations until repeated interleaved measurements establish otherwise. `quad` also exposes an operation-quantifier artifact: `square(x)` can be reported as reducing ciphertext multiplication count even though the encrypted work and runtime are equivalent.

## 5a. CLI rerun with Sonnet 5.5 (2026-10-24)

All 26 tasks were rerun with `--proposer cli --model claude-sonnet-5-5`, effort medium, 8 attempts, on the claude.ai subscription (no API key; about $7.18 equivalent API cost, $0 billed). Aggregate: `agent/results/cli/summary.md`; raw runs are the `20261024-*` directories. 188/208 proposals were correct (API run: 202). 12 tasks changed circuit structure: the 11 Chebyshev tasks except `chebyshev_elu_d7` (whose best was structurally identical), plus `extract_sparse` and `relu_composite` (16→15 ct-ct, 12→11 ct-pt, 1.01x). Notes:

- `chebyshev_softplus_d7` reports ct-ct 5→1 because `square()` is not counted as a ct-ct multiplication; relinearizations 5→3 give the true count of 3.
- `chebyshev_selu_d7` (1.41x) keeps 8 levels but uses a Clenshaw recurrence: ct-pt 8→2, rescale 14→8, match_scale 11→5.
- `elementwise_add_scalar` again shows a large (1.43x) gain with an identical circuit: timing noise.
- The CLI adds one small context block (account email) to each prompt; no CLAUDE.md, tools or MCP servers.

## 5b. Selection and feedback improvements, Sonnet v2 run (2026-10-24)

Implemented the section 7 selection items (1–3, 5) plus feedback changes, then reran all 26 tasks (runs `20261024-1[5-9]*`, summary `agent/results/cli_v2/`). Full three-way comparison: `agent/results/comparison.md`.

- `agent/selection.py` (+ `test_selection.py`, 14 tests): circuit fingerprint (depth, mult. depth, all op counts); a candidate becomes best only if its circuit is new and it wins an interleaved remeasurement (5 batches, ≥3% median, ≥80% batch wins). Final speedup: baseline vs best in 7 alternating batches.
- `evaluate.py`: `square()` counted in `ct_ct_mul`; already-seen circuits are not re-timed; non-timing cases run once; CHEHAB errors are trimmed and get a reading hint; `snapshot`/`time_program` keep and re-time built programs (`programs/`, Git-ignored).
- `src/fheco/passes/align_ckks_operands.cpp`: the scale-mismatch error now prints each operand's level, symbolic scale and expression over the input labels.
- `prompts.py`: judging rule in the first turn; each feedback turn has a verdict note and the baseline's and best's circuit shapes.
- `agent/remeasure_runs.py`: re-times older runs' best kernels the same way (no model calls).

Result: geometric-mean remeasured speedup 1.091 (API Haiku) → 1.127 (Sonnet v1) → 1.141 (Sonnet v2); Chebyshev-only 1.193 → 1.290 → 1.326. v2 found 73 distinct new circuits (v1: 35), repeated an earlier circuit 22 times (v1: 55), and its next attempt was correct after 16/21 failures (v1: 9/20). One run per configuration. Not done yet: manifest of the run environment (section 7 item 4).

## 6. How to inspect or reproduce without spending money

Inspect the aggregate report:

```bash
sed -n '1,220p' agent/results/summary.md
```

Rebuild the aggregate table from existing raw runs:

```bash
agent/.venv/bin/python agent/collect.py
```

Exercise the feedback machinery without an API call:

```bash
agent/.venv/bin/python agent/loop.py \
  --task chebyshev_sigmoid_d7 \
  --iterations 6 \
  --proposer files:agent/examples/chebyshev_sigmoid_d7
```

A paid single-task run, only after user approval, is:

```bash
agent/.venv/bin/python agent/loop.py --task extract_sparse --iterations 8
```

A full paid run, also requiring approval, is:

```bash
agent/.venv/bin/python agent/loop.py --task all --iterations 8
agent/.venv/bin/python agent/collect.py
```

## 7. Next recommended work

Done (section 5b): circuit fingerprint, no credit for unchanged circuits, interleaved remeasurement before replacing the best, final interleaved baseline-versus-best speedup, unit tests.

Next, in order:

1. Repeat runs: three independent v2 runs with identical settings, compared with the first v2 run for consistency and bugs (in progress at the time of writing; results go to `agent/results/`).
2. Teammate replication on another machine (`docs/replication.md`).
3. Reproducibility manifest per run: Git commit and dirty state, host/CPU, Python/CMake/SEAL versions, model configuration, measurement configuration.
4. Only with the user's approval (they consume more subscription usage): lessons carried across tasks, a compile-only "check" tool for multi-step reasoning inside an attempt (the Agent SDK suits this better than the bare CLI), wider search (several candidates per attempt, independent branches, MCTS-style).

## 8. Paused and unsupported work

- **Bootstrapping:** paused even though the professor's CKKS-capable fork was identified. It is not present as a working task in this branch and was not part of the 26-task run.
- **EIF:** paused. Existing embedding/table tasks consume encrypted indicator-style inputs, but they do not homomorphically generate an exact equality indicator. CKKS is approximate, so exact equality remains a separate algorithmic requirement.
- Do not describe the existing embedding task as a complete EIF implementation.

## 9. Verification checklist for the next agent

Before reporting a change complete:

- Confirm no secret or `.env` content appears in diffs or logs.
- Preserve all current uncommitted files unless the user explicitly asks otherwise.
- Run the narrow unit/offline test first.
- Confirm every selected best kernel decrypts within its task tolerance.
- Report both latency and circuit metrics; never report speedup alone.
- State clearly whether API calls were made and what they cost.
- Ask before committing, pushing, or running the full paid suite.

