# Benchmark result records

`run_benchmarks.py` runs BFV and CKKS benchmarks through the same CHEHAB
pipeline and writes one record per run configuration (benchmark, case) to a
CSV file (default `results/results_<scheme>.csv`), with run metadata (commit,
platform, compiler, command, run counts) in `<csv>.meta.json`. `--markdown`
also renders the records as a table with separate BFV and CKKS sections.

```bash
python run_benchmarks.py                       # BFV sweep (vectorized, RL), as before
python run_benchmarks.py --scheme ckks         # every CKKS benchmark
python run_benchmarks.py --scheme all --vectorize 0 --slot-counts 4 \
    --benchmarks nn_linear nn_conv2d nn_avgpool2d quad_ckks --markdown out.md
python run_benchmarks.py --help                # all options and benchmark names
```

CKKS benchmarks are built with `-DFHECO_BUILD_CKKS_BENCHMARKS=ON`, which the
runner passes when a CKKS benchmark is selected; SEAL is found through
`--seal-prefix` (default `$CMAKE_PREFIX_PATH`, else `~/.local`).

## Pipeline, the same for both schemes

1. Prepare inputs: the benchmark's generator writes `fhe_io_example.txt`
   (inputs and expected outputs).
2. Compile phase, `--compile-runs` times (default 3): the benchmark binary runs
   the CHEHAB compiler and writes `he/_gen_he_fhe.{hpp,cpp}` and `he/main.cpp`.
3. Build the generated program.
4. Execution phase: `--warmup` unmeasured runs (default 1), then `--repeats`
   measured runs (default 10) of the generated program, each a fresh process.

BFV benchmarks take the standard command line
`<vectorize_code> <slot_count> <opt_method> <window> <quantifier> <cse> <const_folding>`.
CKKS benchmarks always use the scalar path (CKKS functions reject
vectorization) and their own generators and arguments, which the runner's
registry knows.

## Fields

| Field | Meaning |
|---|---|
| `record_id` | `<benchmark>_<case>`; for BFV the case is the slot count, so ids match the old CSV names (e.g. `box_blur_3`) |
| `benchmark`, `case`, `scheme` | `scheme` is `BFV` or `CKKS` |
| `status` | `PASS`, `FAIL` (wrong result), `NOT_CHECKED` (program has no correctness report) or `ERROR` (a phase failed; see `error`) |
| `poly_modulus_degree`, `coeff_modulus` | read from the generated `he/main.cpp` |
| `plain_modulus_bits` | BFV only |
| `log2_scale` | CKKS only |
| `slot_count` | slots of the CHEHAB function (number of test values) |
| `compiler_path`, `cse`, `const_folding` | how the program was compiled (scalar `simplification_ruleset`, or vectorized with e-graph/RL) |
| `compile_time_ms`, `compile_runs` | median of the compile-phase times printed by the benchmark (`create_func` through `gen_he_code`; on the vectorized path this includes the vectorizer subprocess) |
| `exec_time_ms`, `exec_time_min_ms`, `exec_time_max_ms` | median, min and max over the measured runs of CHEHAB's end-to-end `execution_time_(ms)`: parse inputs + encode + encrypt + evaluate |
| `exec_warmup_runs`, `exec_measured_runs` | warmup policy: warmup runs are discarded |
| `eval_time_ms`, `eval_runs_per_process` | CKKS only: evaluation-only time (`fhe()` alone). Each process reports the median of `eval_runs_per_process` timed runs after one warmup run; the field is the median over the measured processes. Empty for BFV: the generated BFV program does not report it, and BFV code generation is kept unchanged |
| `depth`, `xdepth` | CHEHAB Quantifier `max: (depth, xdepth)`: depth counts operations on the longest path (not relin/rescale/mod_switch/match_scale); xdepth counts multiplications on it, including ciphertext-plaintext ones |
| `op_add`, `op_add_plain`, `op_sub`, `op_sub_plain`, `op_multiply`, `op_multiply_plain`, `op_square`, `op_relinearize`, `op_rescale`, `op_mod_switch`, `op_rotate`, `op_negate` | number of `evaluator.<method>(` calls in the generated `fhe()`, matched by exact method name (`rescale_to_next` -> `op_rescale`, `mod_switch_to_next` -> `op_mod_switch`, `rotate_rows` -> `op_rotate`). The generated code is straight-line, so these are also the executed counts |
| `bfv_outputs_match` | BFV only: 1 when every decrypted output equals the expected value in every measured run (`outputs_match:` line printed by `benchmarks/utils.cpp`) |
| `bfv_noise_budget_bits` | BFV only: smallest remaining invariant noise budget over outputs and measured runs |
| `ckks_fhe_max_abs_error`, `ckks_tolerance` | CKKS only: largest absolute error between decrypted outputs and the same computation in plaintext, over outputs and measured runs; PASS requires every run within the tolerance (default 1e-6) |
| `ckks_chain_index_start`, `ckks_chain_index_end`, `ckks_levels_consumed`, `ckks_expected_levels` | CKKS only: chain index of the fresh inputs and of the output; PASS also requires the expected level count when one is known |
| `ckks_output_log2_scale` | CKKS only: log2 of the output ciphertext's scale |
| `ckks_approx_error_test_points`, `ckks_max_fit_error` | CKKS Chebyshev only: polynomial approximation error, kept separate from FHE error. The first is the plaintext polynomial vs the true activation on the case's test points; the second is the committed fitting-grid error of the coefficient set |

## Comparing results

- Compare a metric only between records with the same `scheme`,
  `poly_modulus_degree` and `coeff_modulus`. BFV and CKKS benchmarks use
  different encryption parameters (BFV N = 16384 by default, CKKS N = 32768),
  so their latencies are not comparable.
- Compare timings only between runs on the same machine with the same
  `exec_warmup_runs` / `exec_measured_runs`.
- FHE error and approximation error answer different questions; never add or
  mix them.
