# LLM optimization loop (basic setup)

This is the closed feedback loop from section 3.2 and Figure 1 of the proposal, in its first, basic form:

```
baseline kernel ──> LLM proposes a kernel ──> CHEHAB compiles it, runs it encrypted, measures it
                         ^                                          │
                         └──────────── result + metrics ────────────┘
```

- **Agent:** Claude Sonnet 5.5 (`claude-sonnet-5-5`) through the Claude Code CLI (`claude -p`), on the logged-in claude.ai subscription; no API key is used. It gets a system prompt that explains CHEHAB's DSL, the CKKS rules and measured operation costs, then the task and the baseline kernel.
- **Environment:** CHEHAB itself. Each proposed kernel is compiled by `Compiler::compile` and `Compiler::gen_he_code`, with the same settings as the benchmarks, except that the CKKS coefficient modulus is sized to the kernel (`CkksParams::trim_to_levels_used`): one 40-bit prime per level the kernel consumes, plus the 60-bit first and special primes, instead of always 10 levels. With a fixed 10-level chain, levels a kernel does not use are wasted, so reducing depth earned nothing and deliberately consuming levels (`x * one`) made kernels faster; see `results/experiments/combine_and_level_floor/`. The baseline is measured the same way. The generated SEAL program is built and run on encrypted inputs, and the decrypted outputs are checked against independently computed expected outputs.
- **Feedback:** compile errors, CHEHAB errors, wrong results, or, for a correct kernel, its `fhe()` time, levels used, depth and operation counts. A CKKS scale mismatch names both operands with their level, symbolic scale and expression over the kernel's inputs. Every turn also gets a verdict (identical to the baseline, the best or attempt *k*; new but slower; not confirmed; new best), the baseline's and best's circuit shapes, a one-line log of every attempt so far, and the model's three fastest distinct correct kernels in full. The history is shown for reference only; the prompt never asks the model to combine kernels.
- The model keeps the whole conversation, so it sees every earlier attempt. Only correct kernels count.
- **Selection** (`selection.py`): a correct kernel replaces the best only if its circuit (CHEHAB depth plus all operation counts) is new, and it is then confirmed faster than the best in 5 alternating timing batches (at least 3% in the median ratio, winning at least 4 of 5). The reported speedup comes from baseline and best timed in 7 alternating batches at the end; it is 1.00 when nothing was confirmed. A circuit seen before is not timed again.

The model only writes CHEHAB DSL code. Kernels that contain preprocessor lines, I/O or compiler internals are rejected before compiling.

## Tasks

There are 29 tasks: every CKKS benchmark in the repository, plus the three BFV neural-network operators (Linear, Conv2d, AvgPool2d) on CHEHAB's scalar path. Bootstrap does not exist yet.

| Group | Tasks | Benchmark |
|---|---|---|
| Chebyshev activations | `chebyshev_{gelu,silu,sigmoid,elu,selu,softplus,mish,hardshrink,relu}_d7`, `chebyshev_sigmoid_d5`, `chebyshev_relu_d2` | `chebyshev_ckks` |
| Composite ReLU | `relu_composite` | `relu_ckks` |
| Quad | `quad` | `quad_ckks` |
| BatchNorm | `batchnorm1d`, `batchnorm2d` | `batchnorm_ckks` |
| Add | `elementwise_add`, `elementwise_add_plain`, `elementwise_add_scalar` | `elementwise_ckks` |
| Multiply | `elementwise_mul`, `elementwise_mul_plain`, `elementwise_mul_scalar` | `elementwise_ckks` |
| Extraction | `extract`, `extract_linear`, `extract_sparse` | `extract_embedding_ckks` |
| Table and embedding | `table_mult`, `embedding` | `extract_embedding_ckks` |
| Linear, Conv2d, AvgPool2d (BFV) | `nn_linear`, `nn_conv2d`, `nn_avgpool2d` | `nn_linear`, `nn_conv2d`, `nn_avgpool2d` |

Each baseline in `tasks/*.cpp` reproduces its benchmark, with the same depth, levels and operation counts. `tasks/gen_baselines.py` writes the Chebyshev, Quad, BatchNorm, Add, Multiply, extraction and table baselines. `relu_composite.cpp` and `embedding.cpp` are written by hand.

A candidate must be correct on every case of its task:

- **fixed:** few values.
- **dense:** many values, also used for timing.
- **random:** embedding only, with random inputs.

The extraction, table and embedding tasks use the 1e-4 tolerance their author chose, because rotations add about 1e-5 of error. Every other task uses 1e-6.

The three BFV tasks are exact: a proposal is correct only if every decrypted output equals the expected integer (`agent/bfv_main.cpp`, which reuses the repository's `benchmarks/utils.cpp` and the generated main's parameters, BFVDefault(16384) with a 20-bit plaintext modulus). Their baselines are the scalar kernels of `benchmarks/nn_*` (one value per ciphertext), as `run_benchmarks.py` records them with `--vectorize 0`; the fixed case is width 4 (the recorded baseline) and the dense, timed case is width 6. CHEHAB's own vectorizer is not used, so a packed rewrite is the model's to find.

The small tasks are already optimal, or nearly so: Add, Multiply, Quad, BatchNorm and the extraction tasks. They show whether the model breaks working code, not whether it speeds it up.

To add a task, write its baseline kernel in `tasks/` and an entry in `tasks.py`.

## Setup (once)

From the repository root, with `cmake` on `PATH` and SEAL 4.1 installed:

```bash
export CMAKE_PREFIX_PATH=/path/to/seal-install
cmake -S . -B build -DCMAKE_PREFIX_PATH=$CMAKE_PREFIX_PATH -DCMAKE_BUILD_TYPE=Release -DFHECO_BUILD_CKKS_BENCHMARKS=ON
cmake --build build -j8
python3 -m venv agent/.venv
agent/.venv/bin/pip install -r agent/requirements.txt
claude auth login    # once; `claude auth status` should show authMethod "claude.ai"
```

The loop runs `claude -p` with no tools, our system prompt in place of the CLI's, no settings or MCP servers, and a working directory outside the repository, so no `CLAUDE.md` is read. It never passes `ANTHROPIC_API_KEY`, `ANTHROPIC_AUTH_TOKEN` or `ANTHROPIC_BASE_URL` to the CLI, so calls always count against the subscription even if a key is exported. Each task's conversation is one CLI session that every attempt resumes.

## Run

Every task, then one results table:

```bash
agent/.venv/bin/python agent/loop.py --task all --iterations 8
agent/.venv/bin/python agent/collect.py
```

One task, or a few at a time:

```bash
agent/.venv/bin/python agent/loop.py --task chebyshev_sigmoid_d7 --iterations 8
for t in extract extract_linear extract_sparse table_mult embedding; do agent/.venv/bin/python agent/loop.py --task $t --iterations 8; done
```

Options:

- `--model`: default `claude-sonnet-5-5`.
- `--effort`: default `medium`.
- `--iterations`: default 8.
- `--eval-repeats`: timed `fhe()` runs per evaluation, default 10.

An evaluation takes about 6 to 20 seconds, so a task takes a few minutes and all 29 take about two and a half hours (about 73% of it CHEHAB compiling, building and timing; the BFV tasks are the slowest to evaluate). Run one loop at a time: concurrent runs share the CPU and the `build/` directory, which corrupts both timings and builds. Calls use the subscription's limits, so a run's `approx_cost_usd` is 0; `equivalent_api_cost_usd` is what the CLI reports the calls would have cost at API prices (about $11 for all 26 tasks with Sonnet 5.5). If a baseline is not correct, the loop reports the task as SKIPPED and goes on.

`collect.py` takes the latest run of each task and writes `agent/results/summary.md` and `summary.csv`, or another directory with `--out`; `--since`/`--until` restrict it to a window of run timestamps (for example one full run). `remeasure_runs.py` re-times several runs' best kernels against the baseline in one session, so runs made at different times can be compared on equal terms (no model calls). Unit tests of the selection rule: `agent/.venv/bin/python -m unittest agent/test_selection.py`. `compare_models.py` puts full runs made with different models side by side under the strict rule (`--run name:since..until` for runs that recorded it, `--run name:<set>` for sets in `results/remeasured.csv`; `--notes file.md` appends remarks). The five-run table is `results/model_comparison.md`. A run whose model calls all failed, for example because the CLI is not logged in, is listed separately and not counted.

Offline check, with no model calls, which replays hand-written kernels covering every kind of feedback:

```bash
agent/.venv/bin/python agent/loop.py --task chebyshev_sigmoid_d7 --iterations 6 --proposer files:agent/examples/chebyshev_sigmoid_d7
```

Attempt 6 there is a hand-optimized kernel. It uses 4 instead of 6 ciphertext multiplications and 5 instead of 8 levels, and runs about 7% faster, so the loop's "new best" path and the headroom are both confirmed.

## Results

Each run writes to `agent/runs/<task>/<timestamp>/`:

- `summary.md`: a baseline and best comparison table, plus one line per attempt.
- `results.json`: every evaluation, token usage, approximate cost, wall time and best attempt. It is the record to compare across team members.
- `baseline.cpp`, `attempt_NN.cpp`, `attempt_NN_answer.md`, `best.cpp`, and `transcript.json` holding the full conversation.
- `programs/`: the built baseline and best programs, kept for re-timing (Git-ignored).

The metrics follow section 4.1 of the proposal:

- **Execution latency:** `fhe()` evaluation time on the dense case, median of 10 runs; the speedup from alternating batches as above.
- **Rotation count**, plus the other operation counts and the levels used.
- **Time to converge:** wall time, and the attempt that produced the best kernel.

Timing noise of one median is several percent, and up to ~40% on sub-millisecond tasks, which is why a single faster median never counts on its own.

## Limits of this basic version

- **One task at a time.** There is no search tree and no parallel candidates; the proposal's MCTS-style exploration is a later step.
- **CKKS only, on the scalar path.** CHEHAB's CKKS functions reject its vectorizer, so the agent rewrites kernels at the DSL level.
- **Coverage.** The tasks cover every CKKS benchmark and the three BFV operators. Bootstrap does not exist yet, so it is not a task.
- **Timing.** Results are from one machine. Compare across team members only on the same machine, or compare speedups rather than raw times.
