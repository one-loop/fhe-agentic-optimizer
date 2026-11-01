# llm_loop: LLM feedback loop for CHEHAB CKKS kernels

Umair's Phase 2 loop. Each teammate builds an independent loop over the same
CKKS operators, and the loops are then compared. This directory does not touch
the compiler, the benchmarks, the shared runner or any other loop (`agent/`).

Status: steps 1-3 are done (environment, baselines, reference kernels, the loop,
tested offline). Step 4 (runs with a model) has not started.

## Layout

| Path | What it is |
|---|---|
| `driver/` | Standalone CMake project: compiles one kernel through CHEHAB (`create_ckks_func`, `Compiler::compile` with the simplification ruleset, `gen_he_code`), with the benchmarks' settings and CKKS parameters (N = 32768, {60, 40 x 10, 60}, scale 2^40). Links `build/libfheco.a`. |
| `tasks.py` | The 26 tasks (every CKKS benchmark in `run_benchmarks.py`): generator commands for the fixed and dense cases, an independent NumPy oracle, a held-out input randomizer, tolerance, owner. |
| `baselines/` | One baseline kernel per task, a port of the benchmark's `fhe()`. Written by `gen_baselines.py`. |
| `evaluate.py` | The environment: kernel check, compile, static analysis, build and run on encrypted inputs, correctness. |
| `static_analysis.py` | Reads the generated SEAL code: operation counts (same fields as `run_benchmarks.py`), scale-alignment multiplies, per-output levels, an annotated level trace. |
| `score.py` | Neutral scorer: full correctness check, then interleaved timing rounds. Usable on kernels from any loop. |
| `check_baselines.py` | Step 1 check: baselines reproduce their benchmarks. |
| `reference/`, `reference_check.py` | Step 2: hand-derived kernels that show the headroom under the DSL rules. |
| `results/` | `baseline_check.json`, `reference_check.json`. |
| `loop.py` | The closed feedback loop (proposal Figure 1): strategies, feedback modes, run management, summaries. |
| `prompts.py` | System prompt, domain knowledge levels, task prompts, compiler feedback in C / L / D terms. |
| `proposers.py` | Where kernels come from: Claude through Claude Code's headless mode (subscription, no API key) or offline replay. |
| `tests/replay/` | Scripted answers that exercise every path of the loop without a model. |

Work files go to `build/llm_loop/` (ignored by git).

## Kernel format

The same as the other loop's, so kernels can be scored across loops: C++ that
defines `void fhe()` and may call `slots()` (the slot count), compiled after
`using namespace std; using namespace fheco;`. Inputs are declared by label
(`Ciphertext x("x");`, `Plaintext alpha("alpha");`), outputs with
`set_output`. CKKS rules: no numeric constants (real constants come from
plaintext inputs), no plaintext-plaintext operations.

## Correctness protocol

A kernel is correct when it passes, within the task's tolerance (1e-6, or 1e-4
for extract, table_mult and embedding, as their owners chose):

- **fixed** and **dense**: the benchmark's own generated inputs and expected outputs;
- **heldout101** to **heldout606**: fresh random inputs with the dense layout,
  expected outputs from the oracle in `tasks.py`. Ciphertext data, table
  columns and BatchNorm vectors are redrawn and polynomial coefficients are
  perturbed (x U(0.5, 1.5) for Chebyshev, x U(0.999, 1.001) for the composite
  ReLU, whose stages compound); operator structure (which terms exist, masks,
  the constants `one`, `half`, `alpha`) is kept. This catches kernels that
  exploit coincidences of the committed inputs: table columns that are multiples
  of each other, or c1 = 2 exactly in gelu/silu/softplus/relu (the offline replay
  test checks both). heldout505 and heldout606 also pin three of every four
  slots of the ciphertext input to the ends of its range and to 0 and, for
  Chebyshev, flip coefficient signs (Chebyshev, Quad and BatchNorm only).
  The first 864 model calls searched with heldout101 and heldout202 only;
  `experiments/recheck_correctness.py` re-checks their final kernels with all six.

Each oracle is validated against the generators' expected outputs (max
relative difference 1e-15).

## Usage

From the repository root, after building the repository with the CKKS
benchmarks (`cmake -S . -B build -DFHECO_BUILD_CKKS_BENCHMARKS=ON ...`, which
`run_benchmarks.py --scheme ckks` also does):

```bash
python3 -m llm_loop.gen_baselines
python3 -m llm_loop.check_baselines --jobs 4 --records <run_benchmarks CSV with sigmoid_d5>
python3 -m llm_loop.reference_check --rounds 5
python3 -m llm_loop.score --task chebyshev_gelu_d7 --kernel mine=path/to/kernel.cpp
```

`chebyshev_sigmoid_d5` has no record in the committed `results/*.csv`; pass a
`run_benchmarks.py --scheme ckks` CSV with `--records` to check it.

## Results so far (2026-10-24, Apple M-series, SEAL 4.1, commit 8831440)

Step 1: 26/26 baselines reproduce their benchmark (levels, (depth, xdepth) and
every operation count equal the Phase 1 records) and pass every case,
held-out cases included.

Step 2: reference kernels, timed against the baseline with `score.py`
(5 interleaved rounds, median of 10 fhe() runs each; dense case):

| Tasks | Kernel | Levels | ct x ct | ct x pt | align | mod_switch | Speedup |
|---|---|---|---|---|---|---|---|
| gelu, silu, softplus, relu d7 | baseline | 7 | 5 | 5 | 7 | 10 | 1.00 (174 ms) |
| | `even_d7--cheb_u` | 4 | 3 | 4 | 2 | 1 | **1.41** |
| sigmoid, hardshrink d7 | baseline | 8 | 6 | 5 | 8 | 13 | 1.00 (185 ms) |
| | `odd_d7*--cheb_u` | 5 | 4 | 4 | 1 | 3 | **1.29** |
| | `odd_d7*--power_ps` | 4 | 5 | 5 | 3 | 0 | 0.97 |
| elu, selu, mish d7 | baseline | 8 | 6 | 8 | 11 | 10 | 1.00 (206 ms) |
| | `full_d7--cheb_u` | 5 | 5 | 6 | 3 | 3 | **1.09** |
| | `full_d7--mixed_ps` | rejected by CHEHAB (equal-level scale mismatch) | | | | | |
| sigmoid d5 | baseline | 6 | 4 | 4 | 5 | 8 | 1.00 (147 ms) |
| | `odd_d5--power` | 4 | 3 | 4 | 2 | 2 | **1.21** |
| extract_sparse | baseline | 1 | 0 | 3 (3 rot) | 0 | 0 | 1.00 (101 ms) |
| | `extract_sparse--shared_rotation` | 1 | 0 | 3 (2 rot) | 0 | 0 | **1.35** |

"align" counts the multiply_plain-by-1 + rescale pairs CHEHAB inserts to line up
scales. Fewer levels alone do not pay: the 4-level power-basis kernels are
slower than the 5-level Chebyshev ones because they need more ct x ct
multiplications. Time follows ct x ct, ct x pt and alignment operations.

## The loop (step 3)

```bash
python3 -m llm_loop.loop --tasks all --iterations 8                     # archive search, measured feedback
python3 -m llm_loop.loop --tasks all --runs 5                           # single-run and best-of-5 (ComPilot)
python3 -m llm_loop.loop --tasks family:chebyshev --strategy linear     # ComPilot-style dialogue
python3 -m llm_loop.loop --tasks chebyshev_gelu_d7 table_mult --proposer replay:llm_loop/tests/replay  # offline
```

Every call to the model is one iteration; `--iterations` is the budget per task and run.

- **Strategies.** `linear`: the proposal's ComPilot-style dialogue, where the model sees
  every earlier attempt and its feedback. `archive`: research-backed search (archive of
  every kernel, the first quarter of the budget explores from the baseline with
  different hints, then Thompson-sampling parent choice as in REx, one faster
  kernel shown for inspiration as in FunSearch, one repair turn per illegal kernel,
  duplicates not re-evaluated, static pre-screen of clearly slower kernels).
- **Feedback** (`--feedback`), the proposal's open design question: `execution`
  (measured latency) or `static` (cost-model latency; correctness is still checked
  on encrypted inputs and measured times are recorded).
- **Domain knowledge** (`--hints`): `none`, `identities`, `full` (identities plus
  worked examples that are not benchmark tasks), to measure the effect of
  structured grounding.
- Each attempt's feedback reports compilation time C, latency L and multiplicative
  depth D with the baseline and best-so-far, operation counts and the largest
  estimated cost; illegal code gets CHEHAB's message verbatim. A kernel replaces
  the best only when it is faster by more than `--margin` (2%), and each run's best
  kernel is re-timed against the baseline with `score.py` (`--final-rounds`).
- Results: `<out>/<task>/run<k>/` (every prompt, answer and kernel, `attempts.jsonl`,
  `result.json`) and `<out>/summary.md`, `summary.csv`. Interrupted runs resume
  with the same `--out`.

### Model access without an API key

The default proposer runs `claude -p` (Claude Code headless mode) with the model
pinned to `claude-haiku-5-5`, a replaced system prompt, no tools and safe mode, from
an empty directory. Signed in with a Claude subscription, calls count against the
plan's usage limits instead of being billed per token. `ANTHROPIC_API_KEY` and
`ANTHROPIC_AUTH_TOKEN` are removed from each call's environment, because headless
mode always uses (and bills) a key when one is present. Haiku 5.5 needs Claude
Code 2.1.293 or newer (`claude update`). If a usage limit is reached, the loop
stops and the same command resumes it.

## Phase-2 pilot protocol

`experiments/phase2_pilot.sh` runs the approved pilot: `archive` vs `linear`, both
with `--hints none` and measured feedback, on `chebyshev_gelu_d7`,
`chebyshev_sigmoid_d7` and `chebyshev_elu_d7`; Claude Haiku 5.5 (effort medium)
through Claude Code headless mode for both strategies; 8 model calls per run;
3 independent runs per task and strategy (144 calls).

- **Budget.** Every model invocation (proposal, repair, duplicate, invalid or
  empty answer) uses one of the 8 calls; a run that does not make exactly 8 calls
  is an error. Nothing is called after the budget.
- **Order.** Task order rotates across runs (run 1: GELU, Sigmoid, ELU; run 2:
  Sigmoid, ELU, GELU; run 3: ELU, GELU, Sigmoid) and strategy order alternates
  (archive first in runs 1 and 3, linear first in run 2).
- **Final kernel.** Each run's final kernel is the one its strategy selected
  within the budget (`best.cpp`). Neutral re-scoring measures it and never picks
  another attempt.
- **Neutral re-scoring** (`rescore.py`), per task in one session: correctness on
  fixed, dense and held-out cases; 3 compiles (median compile time, reported
  only); 1 warmup + 10 measured processes of `./main --repeats 10`, interleaved
  across the baseline and every final kernel. Speedup = baseline eval-only median
  / kernel eval-only median from that session. Identical kernels are measured
  once (a final equal to the baseline scores x1.000).
- **Reporting.** Every run is kept and reported; per task and strategy the
  median, mean, min, max and standard deviation; across tasks only the geometric
  mean of per-task means. Approximation error (polynomial vs activation) is a task
  property reported apart from FHE error.
- **Outputs.** Runs: `runs/phase2-pilot/<strategy>/<task>/run<k>/` (git-ignored).
  Results: `results/phase2_pilot/` (`phase2_pilot.md`, `runs.csv`, `rescore.csv`,
  `rescore.meta.json`).

## Caveats

- Not a sandbox: kernels are C++ compiled and run on this machine. The token
  blocklist in `evaluate.py` only stops accidental or obvious misuse.
- Timings depend on the machine; compare kernels only through `score.py` runs
  on the same machine, with nothing else running.
- Concurrent loop runs need different `--work` directories.
