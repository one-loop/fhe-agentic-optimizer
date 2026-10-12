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

| Task | Baseline | Why it is a target |
|---|---|---|
| `chebyshev_sigmoid_d7` | `benchmarks/chebyshev_ckks`, sigmoid, degree 7 | linear-depth recurrence: 6 ciphertext multiplications, 8 levels |
| `chebyshev_gelu_d7` | the same benchmark, GELU | same structure, even polynomial |
| `relu_composite` | `benchmarks/relu_ckks` | 16 ciphertext multiplications, all 10 levels |
| `embedding` | `benchmarks/extract_embedding_ckks`, embedding | 26 rotations, the proposal's main bottleneck |

Each baseline in `tasks/*.cpp` reproduces its benchmark exactly: the same depth, levels and operation counts. A candidate must be correct on every case of the task: few values ("fixed"), many values ("dense", also used for timing), and, for embedding, random inputs ("random"). Add a task by writing its baseline kernel in `tasks/` and an entry in `tasks.py`.

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

```bash
agent/.venv/bin/python agent/loop.py --task chebyshev_sigmoid_d7 --iterations 8
agent/.venv/bin/python agent/loop.py --task all
```

Options: `--model` (default `claude-haiku-5-5`), `--effort` (default `medium`), `--iterations` (default 8) and `--eval-repeats` (timed `fhe()` runs per evaluation, default 10).

An evaluation takes about 10 to 30 seconds, so a task of 8 attempts takes a few minutes. With Haiku 5.5 one run costs about a cent; the exact cost is in the results.

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
- **Coverage.** The four tasks cover polynomial activations and a rotation-heavy operator. Linear, Conv2d and AvgPool are BFV-only benchmarks and are not tasks yet.
