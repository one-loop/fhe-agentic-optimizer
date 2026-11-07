# Model comparison: Haiku, Sonnet and Opus 5.5 in my loop

Written by `experiments/compare_models.py` from the neutral re-scoring of each experiment. Archive
strategy, hints none, measured feedback, 8 model calls per run, all through `claude -p` on the
subscription. Speedup = baseline eval-only median / final-kernel eval-only median, both from the same
re-scoring session. The final kernel is the strategy's own selection.

- **Runs differ per task.** 3 runs per model for the Chebyshev tasks and nn_conv2d (Haiku: 5 for GELU,
  Sigmoid d7, ELU), 1 run for the rest. A single run says nothing about run-to-run spread.
- **Sessions differ.** Each model's runs were re-scored in their own session on the same machine. Speedups
  are ratios to the baseline of the same session, for which the same kernel moved by about 0.1% (median) across sessions earlier.
- **Correctness.** Every final kernel is checked on the fixed, dense and 6 held-out cases; 102 of 102 finals pass.
- **Pending.** Sonnet and Opus on the other 14 operators ran but are not re-timed yet. Sonnet's
  relu_composite run was skipped because the baseline failed a held-out case (error 1.35e-06,
  tolerance 1e-06).
- **Not confirmed.** Opus run 1 on GELU (x2.63) and Sigmoid d7 (x2.55) use more levels than the
  baseline; a repeated correctness check is pending (see `../phase2_opus_focus/README.md`).
- These are my loop's results only. Teammates' kernels are compared in `../phase2_teammates_rescore/`.

## Across tasks (geometric mean of per-task mean speedups, same tasks for every model in a row)

| tasks | n | Haiku | Sonnet | Opus |
|---|---:|---:|---:|---:|
| 9 Chebyshev activations | 9 | x1.272 | x1.250 | - |
| GELU, Sigmoid d7, ELU | 3 | x1.277 | x1.225 | x1.661 |
| nn_conv2d (BFV) | 1 | x1.430 | x1.622 | x2.799 |
| Quad, BatchNorm1d/2d, nn_linear, nn_avgpool2d | 5 | x1.002 | x1.002 | - |
| The other 14 operators | 14 | pending | pending | pending |

## Every operator (median, min-max over runs)

| operator | owner | scheme | Haiku | Sonnet | Opus |
|---|---|---|---|---|---|
| chebyshev_gelu_d7 | Umair | CKKS | x1.47 (1.41-1.55, 5 runs) | x1.41 (1.23-1.47, 3 runs) | x1.60 (1.58-2.63, 3 runs) |
| chebyshev_sigmoid_d7 | Umair | CKKS | x1.30 (1.18-1.44, 5 runs) | x1.26 (1.18-1.31, 3 runs) | x1.58 (1.38-2.55, 3 runs) |
| chebyshev_elu_d7 | Umair | CKKS | x1.09 (1.08-1.09, 5 runs) | x1.07 (1.05-1.09, 3 runs) | x1.27 (1.25-1.36, 3 runs) |
| chebyshev_silu_d7 | Umair | CKKS | x1.41 (1.40-1.55, 3 runs) | x1.42 (1.41-1.43, 3 runs) | not run |
| chebyshev_selu_d7 | Umair | CKKS | x1.08 (1.08-1.08, 3 runs) | x1.07 (1.00-1.10, 3 runs) | not run |
| chebyshev_softplus_d7 | Umair | CKKS | x1.43 (1.41-1.47, 3 runs) | x1.41 (1.41-1.47, 3 runs) | not run |
| chebyshev_mish_d7 | Umair | CKKS | x1.09 (1.09-1.09, 3 runs) | x1.08 (1.00-1.09, 3 runs) | not run |
| chebyshev_hardshrink_d7 | Umair | CKKS | x1.23 (1.23-1.38, 3 runs) | x1.21 (1.19-1.37, 3 runs) | not run |
| chebyshev_sigmoid_d5 | Umair | CKKS | x1.36 (1.13-1.52, 3 runs) | x1.52 (1.21-1.52, 3 runs) | not run |
| quad | Umair | CKKS | x1.00 | x1.00 | not run |
| batchnorm1d | Umair | CKKS | x1.00 | x1.00 | not run |
| batchnorm2d | Umair | CKKS | x1.01 | x1.00 | not run |
| nn_linear | Umair | BFV | x1.00 | x1.00 | not run |
| nn_avgpool2d | Umair | BFV | x1.00 | x1.01 | not run |
| nn_conv2d | Umair | BFV | x1.43 (1.43-1.43, 3 runs) | x1.35 (1.30-2.22, 3 runs) | x2.35 (2.31-3.74, 3 runs) |
| chebyshev_relu_d7 | Moonis | CKKS | x1.41 | pending re-timing | pending re-timing |
| chebyshev_relu_d2 | Moonis | CKKS | x1.28 | pending re-timing | pending re-timing |
| relu_composite | Moonis | CKKS | x1.00 | pending re-timing | pending re-timing |
| elementwise_add | Moonis | CKKS | x1.00 | pending re-timing | pending re-timing |
| elementwise_add_plain | Moonis | CKKS | x1.00 | pending re-timing | pending re-timing |
| elementwise_add_scalar | Moonis | CKKS | x1.00 | pending re-timing | pending re-timing |
| elementwise_mul | Moonis | CKKS | x1.00 | pending re-timing | pending re-timing |
| elementwise_mul_plain | Moonis | CKKS | x1.00 | pending re-timing | pending re-timing |
| elementwise_mul_scalar | Moonis | CKKS | x1.00 | pending re-timing | pending re-timing |
| extract | mabyan22 | CKKS | x1.00 | pending re-timing | pending re-timing |
| extract_linear | mabyan22 | CKKS | x1.00 | pending re-timing | pending re-timing |
| extract_sparse | mabyan22 | CKKS | x1.39 | pending re-timing | pending re-timing |
| table_mult | mabyan22 | CKKS | x1.00 | pending re-timing | pending re-timing |
| embedding | mabyan22 | CKKS | x1.00 | pending re-timing | pending re-timing |
