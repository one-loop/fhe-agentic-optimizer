# LLM optimization loop (basic setup)

This is the closed feedback loop from section 3.2 and Figure 1 of the proposal, in its first, basic form:

```
baseline kernel ──> LLM proposes a kernel ──> CHEHAB compiles it, runs it encrypted, measures it
                         ^                                          │
                         └──────────── result + metrics ────────────┘
```

- **Agent:** Claude Haiku 5.5 (`claude-haiku-5-5`) through the Anthropic API. It gets a system prompt that explains CHEHAB's DSL, the CKKS rules and measured operation costs, then the task and the baseline kernel.
- **Environment:** CHEHAB itself. Each proposed kernel is compiled by `Compiler::compile` and `Compiler::gen_he_code`, with the same settings as the benchmarks. The generated SEAL program is built and run on encrypted inputs, and the decrypted outputs are checked against independently computed expected outputs.
- **Feedback:** compile errors, CHEHAB errors (such as a CKKS scale mismatch), wrong results, or, for a correct kernel, its `fhe()` time, levels used, depth and operation counts.
- The model keeps the whole conversation, so it sees every earlier attempt. Only correct kernels count. The fastest correct one is kept.

The model only writes CHEHAB DSL code. Kernels that contain preprocessor lines, I/O or compiler internals are rejected before compiling.

## Tasks

There are 26 tasks: every CKKS benchmark in the repository. Bootstrap does not exist yet. Linear, Conv2d and AvgPool are BFV-only benchmarks, so they are not tasks yet.

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

Each baseline in `tasks/*.cpp` reproduces its benchmark, with the same depth, levels and operation counts. `tasks/gen_baselines.py` writes the Chebyshev, Quad, BatchNorm, Add, Multiply, extraction and table baselines. `relu_composite.cpp` and `embedding.cpp` are written by hand.

A candidate must be correct on every case of its task:

- **fixed:** few values.
- **dense:** many values, also used for timing.
- **random:** embedding only, with random inputs.

The extraction, table and embedding tasks use the 1e-4 tolerance their author chose, because rotations add about 1e-5 of error. Every other task uses 1e-6.

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
export ANTHROPIC_API_KEY=sk-ant-...
```

`requirements.txt` pins the SDK below 1.0 because the 1.x SDK needs Python 3.10 or newer.

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

- `--model`: default `claude-haiku-5-5`.
- `--effort`: default `medium`.
- `--iterations`: default 8.
- `--eval-repeats`: timed `fhe()` runs per evaluation, default 10.

An evaluation takes about 6 to 20 seconds, so a task takes a few minutes and all 26 take one to two hours. With Haiku 5.5, a task costs about one to two cents, so the whole set costs roughly half a dollar. If a baseline is not correct, the loop reports the task as SKIPPED and goes on.

`collect.py` takes the latest run of each task and writes `agent/results/summary.md` and `summary.csv`. A run whose model calls all failed, for example with an invalid API key, is listed separately and not counted.

Offline check, with no API calls, which replays hand-written kernels covering every kind of feedback:

```bash
agent/.venv/bin/python agent/loop.py --task chebyshev_sigmoid_d7 --iterations 6 --proposer files:agent/examples/chebyshev_sigmoid_d7
```

Attempt 6 there is a hand-optimized kernel. It uses 4 instead of 6 ciphertext multiplications and 5 instead of 8 levels, and runs about 7% faster, so the loop's "new best" path and the headroom are both confirmed.

## Results

Each run writes to `agent/runs/<task>/<timestamp>/`:

- `summary.md`: a baseline and best comparison table, plus one line per attempt.
- `results.json`: every evaluation, token usage, approximate cost, wall time and best attempt. It is the record to compare across team members.
- `baseline.cpp`, `attempt_NN.cpp`, `attempt_NN_answer.md`, `best.cpp`, and `transcript.json` holding the full conversation.

The metrics follow section 4.1 of the proposal:

- **Execution latency:** `fhe()` evaluation time on the dense case, median of 10 runs.
- **Rotation count**, plus the other operation counts and the levels used.
- **Time to converge:** wall time, and the attempt that produced the best kernel.

Timing noise is about 2 to 3%. Gains smaller than 3% are kept, but the console does not mark them as a new best.

## Limits of this basic version

- **One task at a time.** There is no search tree and no parallel candidates; the proposal's MCTS-style exploration is a later step.
- **CKKS only, on the scalar path.** CHEHAB's CKKS functions reject its vectorizer, so the agent rewrites kernels at the DSL level.
- **Coverage.** The tasks cover every CKKS benchmark. Linear, Conv2d and AvgPool are BFV-only benchmarks, and Bootstrap does not exist yet, so none of them are tasks.
- **Timing.** Results are from one machine. Compare across team members only on the same machine, or compare speedups rather than raw times.
