# Fixed vs sized CKKS modulus chain

Written by `experiments/compare_sized_chain.py`. My loop's CKKS final kernels (archive strategy, hints none,
the same kernels as `../model_comparison/`), re-timed with the coefficient modulus sized to each kernel: one
40-bit prime per level it consumes, plus the 60-bit first and special primes, instead of always 10 levels.
The baseline gets its own sized chain. The kernels were not re-generated: the loop itself ran on the fixed chain.

- **Why.** With the fixed chain, levels a kernel does not use are carried by every operation, so saving
  levels earned little, and consuming extra levels made later operations cheaper. The sized chain removes
  both effects. It is `CkksParams::trim_to_levels_used` from the teammates' `sized-chain-and-history` branch.
- **How.** One re-scoring session, all models' kernels of a task interleaved with one baseline, the same
  protocol as `rescore.py` (8 correctness cases, 1 warmup + 10 measured processes). Run in a separate,
  git-ignored worktree with the compiler change applied; this branch's compiler is unchanged. The worktree's
  changes are in `worktree.patch` (compiler, `trim_to_levels_used = true` in the driver, runs labelled by
  model folder).
- **Correctness.** 117 of 117 kernels pass all 8 cases under the sized chain.
- **Extra levels.** 2 kernels consume more levels than their baseline: opus chebyshev_gelu_d7 run 1 (10 vs 7, x2.63 -> x1.44); opus chebyshev_sigmoid_d7 run 1 (10 vs 8, x2.55 -> x1.72).
- **Caveats.** One session on one laptop, on battery. 1 run per task outside the Chebyshev tasks.
  The BFV tasks are not affected (CHEHAB selects BFV parameters per program) and are not re-timed.

## Across tasks (geometric mean of per-task mean speedups, fixed -> sized)

| tasks | n | Haiku | Sonnet | Opus |
|---|---:|---|---|---|
| 9 Chebyshev activations | 9 | x1.272 -> **x2.272** | x1.250 -> **x2.140** | - |
| GELU, Sigmoid d7, ELU | 3 | x1.277 -> **x2.351** | x1.225 -> **x2.099** | x1.661 -> **x2.184** |
| Quad, BatchNorm1d/2d | 3 | x1.003 -> **x1.002** | x1.000 -> **x1.000** | - |
| The other 14 operators | 14 | x1.067 -> **x1.126** | x1.068 -> **x1.127** | x1.069 -> **x1.128** |

## Every CKKS operator (mean over runs, fixed -> sized; L = levels of the final kernels)

| operator | owner | baseline levels | Haiku | Sonnet | Opus |
|---|---|---:|---|---|---|
| chebyshev_gelu_d7 | Umair | 7 | x1.48 -> x2.62 (L 4,5) | x1.37 -> x2.40 (L 4,5) | x1.93 -> x2.19 (L 10,5) |
| chebyshev_sigmoid_d7 | Umair | 8 | x1.29 -> x2.44 (L 5,6) | x1.25 -> x2.08 (L 5,6) | x1.83 -> x2.36 (L 10,5,6) |
| chebyshev_elu_d7 | Umair | 8 | x1.09 -> x2.03 (L 5) | x1.07 -> x1.85 (L 5,6) | x1.29 -> x2.01 (L 6) |
| chebyshev_silu_d7 | Umair | 7 | x1.46 -> x2.77 (L 4,5) | x1.42 -> x2.95 (L 4) | not run |
| chebyshev_selu_d7 | Umair | 8 | x1.08 -> x1.99 (L 5) | x1.06 -> x1.67 (L 5,8) | not run |
| chebyshev_softplus_d7 | Umair | 7 | x1.44 -> x2.74 (L 4,5) | x1.43 -> x2.73 (L 4,5) | not run |
| chebyshev_mish_d7 | Umair | 8 | x1.09 -> x2.00 (L 5) | x1.06 -> x1.68 (L 5,8) | not run |
| chebyshev_hardshrink_d7 | Umair | 8 | x1.28 -> x2.35 (L 5,6) | x1.25 -> x2.24 (L 5,6) | not run |
| chebyshev_sigmoid_d5 | Umair | 6 | x1.34 -> x1.74 (L 5) | x1.42 -> x2.01 (L 4,5) | not run |
| quad | Umair | 1 | x1.00 -> x1.00 (L 1) | x1.00 -> x1.00 (L 1) | not run |
| batchnorm1d | Umair | 1 | x1.00 -> x1.00 (L 1) | x1.00 -> x1.00 (L 1) | not run |
| batchnorm2d | Umair | 1 | x1.01 -> x1.00 (L 1) | x1.00 -> x1.00 (L 1) | not run |
| chebyshev_relu_d7 | Moonis | 7 | x1.41 -> x2.91 (L 4) | x1.40 -> x2.92 (L 4) | x1.44 -> x2.99 (L 4) |
| chebyshev_relu_d2 | Moonis | 3 | x1.28 -> x1.40 (L 3) | x1.28 -> x1.40 (L 3) | x1.28 -> x1.40 (L 3) |
| relu_composite | Moonis | 10 | x1.00 -> x1.00 (L 10) | x1.01 -> x1.01 (L 10) | x1.00 -> x1.00 (L 10) |
| elementwise_add | Moonis | 0 | x1.00 -> x1.00 (L 0) | x1.00 -> x1.00 (L 0) | x1.00 -> x1.00 (L 0) |
| elementwise_add_plain | Moonis | 0 | x1.00 -> x1.00 (L 0) | x1.00 -> x1.00 (L 0) | x1.00 -> x1.00 (L 0) |
| elementwise_add_scalar | Moonis | 0 | x1.00 -> x1.00 (L 0) | x1.00 -> x1.00 (L 0) | x1.00 -> x1.00 (L 0) |
| elementwise_mul | Moonis | 1 | x1.00 -> x1.00 (L 1) | x1.00 -> x1.00 (L 1) | x1.00 -> x1.00 (L 1) |
| elementwise_mul_plain | Moonis | 1 | x1.00 -> x1.00 (L 1) | x1.00 -> x1.00 (L 1) | x1.00 -> x1.00 (L 1) |
| elementwise_mul_scalar | Moonis | 1 | x1.00 -> x1.00 (L 1) | x1.00 -> x1.00 (L 1) | x1.00 -> x1.00 (L 1) |
| extract | mabyan22 | 1 | x1.00 -> x1.00 (L 1) | x1.00 -> x1.00 (L 1) | x1.00 -> x1.00 (L 1) |
| extract_linear | mabyan22 | 1 | x1.00 -> x1.00 (L 1) | x1.00 -> x1.00 (L 1) | x1.00 -> x1.00 (L 1) |
| extract_sparse | mabyan22 | 1 | x1.39 -> x1.28 (L 1) | x1.38 -> x1.28 (L 1) | x1.37 -> x1.28 (L 1) |
| table_mult | mabyan22 | 1 | x1.00 -> x1.00 (L 1) | x1.00 -> x1.00 (L 1) | x1.00 -> x1.00 (L 1) |
| embedding | mabyan22 | 2 | x1.00 -> x1.00 (L 2) | x1.00 -> x1.00 (L 2) | x1.00 -> x1.00 (L 2) |
