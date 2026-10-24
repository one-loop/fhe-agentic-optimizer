# llm_loop: LLM feedback loop for CHEHAB CKKS kernels

Umair's Phase 2 loop. Each teammate builds an independent loop over the same
CKKS operators, and the loops are then compared. This directory does not touch
the compiler, the benchmarks, the shared runner or any other loop (`agent/`).

Status: steps 1 and 2 (environment, baselines, reference kernels) are done. The
search loop and the model proposer (steps 3 and 4) are not written yet.

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
- **heldout101**, **heldout202**: fresh random inputs with the dense layout,
  expected outputs from the oracle in `tasks.py`. Ciphertext data, table
  columns and BatchNorm vectors are redrawn; operator structure (coefficients,
  masks) is kept. This catches kernels that exploit coincidences of the
  committed inputs (for example the table columns are multiples of each other).

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

## Caveats

- Not a sandbox: kernels are C++ compiled and run on this machine. The token
  blocklist in `evaluate.py` only stops accidental or obvious misuse.
- Timings depend on the machine; compare kernels only through `score.py` runs
  on the same machine.
