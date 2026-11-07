# Phase-2 pilot: neutral re-scoring

Commit 29347817aa (clean), Darwin arm64 arm, Apple clang version 17.0.0 (clang-1700.3.19.1), SEAL 4.1. Re-scored 2026-11-07T21:46:53.

Speedup = baseline eval-only median / kernel eval-only median, both from the same neutral session (1 warmup + 10 measured processes of ./main --repeats 10, interleaved). The final kernel of each run is the strategy's own selection; re-scoring never re-selects. E2E = CHEHAB execution_time_(ms) (parse + encode + encrypt + fhe()). Compile time: median of 3, reported only.

## Every run

| task | strategy | run | final | speedup | eval (ms) | e2e (ms) | FHE err | depth | xdepth | levels | ct x ct | ct x pt | align | rescale | mod sw | C (ms) | correct / 8 | dup | failures | out tok | think tok | search (s) |
|---|---|---:|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---|---:|---:|---:|
| chebyshev_relu_d2 | archive | 1 | #1 (k1) | x1.276 | 47.1 | 71.6 | 1.56e-07 | 7 | 3 | 3 | 1 | 2 | 0 | 3 | 1 | 0.34 | 3 / 8 | 3 | - | 15438 | 12864 | 234.3 |
| chebyshev_relu_d7 | archive | 1 | #3 (k1) | x1.404 | 126.1 | 154.6 | 2.69e-07 | 9 | 4 | 4 | 3 | 4 | 2 | 9 | 0 | 0.58 | 5 / 8 | 2 | chehab 1 | 16412 | 12182 | 302.6 |
| elementwise_add | archive | 1 | #0 (baseline) | x1.000 | 0.5 | 42.0 | 4.90e-08 | 1 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0.35 | 0 / 8 | 8 | - | 1235 | 44 | 48.8 |
| elementwise_add_plain | archive | 1 | #0 (baseline) | x1.000 | 2.8 | 24.8 | 3.07e-08 | 1 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0.33 | 0 / 8 | 8 | - | 2509 | 1099 | 56.6 |
| elementwise_add_scalar | archive | 1 | #0 (baseline) | x1.000 | 0.3 | 22.0 | 2.84e-08 | 1 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0.25 | 0 / 8 | 8 | - | 2526 | 1216 | 56.4 |
| elementwise_mul | archive | 1 | #0 (baseline) | x1.000 | 36.9 | 82.1 | 6.05e-08 | 1 | 1 | 1 | 1 | 0 | 0 | 1 | 0 | 0.39 | 0 / 8 | 8 | - | 1671 | 202 | 54.0 |
| elementwise_mul_plain | archive | 1 | #0 (baseline) | x1.000 | 7.7 | 29.4 | 4.96e-08 | 1 | 1 | 1 | 0 | 1 | 0 | 1 | 0 | 0.41 | 0 / 8 | 8 | - | 1431 | 212 | 47.1 |
| elementwise_mul_scalar | archive | 1 | #0 (baseline) | x1.000 | 5.2 | 26.9 | 5.72e-08 | 1 | 1 | 1 | 0 | 1 | 0 | 1 | 0 | 0.31 | 0 / 8 | 8 | - | 2122 | 726 | 54.7 |
| embedding | archive | 1 | #0 (baseline) | x1.000 | 759.4 | 794.2 | 4.29e-05 | 21 | 2 | 2 | 0 | 6 | 0 | 6 | 0 | 0.53 | 1 / 8 | 6 | wrong 1 | 33893 | 30425 | 431.1 |
| extract | archive | 1 | #0 (baseline) | x1.000 | 33.8 | 58.3 | 6.14e-06 | 2 | 1 | 1 | 0 | 1 | 0 | 1 | 0 | 0.31 | 0 / 8 | 8 | - | 5335 | 3665 | 80.4 |
| extract_linear | archive | 1 | #0 (baseline) | x1.000 | 34.4 | 59.7 | 1.11e-05 | 2 | 1 | 1 | 0 | 1 | 0 | 1 | 0 | 0.32 | 0 / 8 | 8 | - | 4323 | 2669 | 68.9 |
| extract_sparse | archive | 1 | #1 (k1) | x1.379 | 73.7 | 119.8 | 7.46e-06 | 4 | 1 | 1 | 0 | 2 | 1 | 3 | 0 | 0.38 | 7 / 8 | 1 | - | 11285 | 8383 | 277.1 |
| relu_composite | archive | 1 | #8 (k1) | x1.014 | 365.1 | 402.8 | 8.04e-07 | 18 | 10 | 10 | 15 | 11 | 8 | 34 | 16 | 0.69 | 2 / 8 | 4 | chehab 2 | 28515 | 22155 | 364.4 |
| table_mult | archive | 1 | #0 (baseline) | x1.000 | 663.7 | 691.2 | 6.82e-05 | 17 | 1 | 1 | 0 | 3 | 0 | 3 | 0 | 0.53 | 3 / 8 | 5 | - | 10253 | 7412 | 354.4 |

## Per task and strategy (3 runs each)

| task | strategy | runs | speedup |
|---|---|---:|---|
| chebyshev_relu_d2 | archive | 1 | median x1.276, mean x1.276, min x1.276, max x1.276, sd 0.000 |
| chebyshev_relu_d7 | archive | 1 | median x1.404, mean x1.404, min x1.404, max x1.404, sd 0.000 |
| elementwise_add | archive | 1 | median x1.000, mean x1.000, min x1.000, max x1.000, sd 0.000 |
| elementwise_add_plain | archive | 1 | median x1.000, mean x1.000, min x1.000, max x1.000, sd 0.000 |
| elementwise_add_scalar | archive | 1 | median x1.000, mean x1.000, min x1.000, max x1.000, sd 0.000 |
| elementwise_mul | archive | 1 | median x1.000, mean x1.000, min x1.000, max x1.000, sd 0.000 |
| elementwise_mul_plain | archive | 1 | median x1.000, mean x1.000, min x1.000, max x1.000, sd 0.000 |
| elementwise_mul_scalar | archive | 1 | median x1.000, mean x1.000, min x1.000, max x1.000, sd 0.000 |
| embedding | archive | 1 | median x1.000, mean x1.000, min x1.000, max x1.000, sd 0.000 |
| extract | archive | 1 | median x1.000, mean x1.000, min x1.000, max x1.000, sd 0.000 |
| extract_linear | archive | 1 | median x1.000, mean x1.000, min x1.000, max x1.000, sd 0.000 |
| extract_sparse | archive | 1 | median x1.379, mean x1.379, min x1.379, max x1.379, sd 0.000 |
| relu_composite | archive | 1 | median x1.014, mean x1.014, min x1.014, max x1.014, sd 0.000 |
| table_mult | archive | 1 | median x1.000, mean x1.000, min x1.000, max x1.000, sd 0.000 |

## Across tasks (geometric mean of the per-task mean speedups)

| strategy | tasks | geometric mean |
|---|---:|---:|
| archive | 14 | x1.068 |

## Approximation error (task property, separate from FHE error)

| task | max_fit_error (fitting grid) | approx. error, fixed points | approx. error, dense points |
|---|---:|---:|---:|
| chebyshev_relu_d2 | 0.375091 | 0.375091 | 0.359784 |
| chebyshev_relu_d7 | 0.17094 | 0.17094 | 0.155796 |
| elementwise_add | - | - | - |
| elementwise_add_plain | - | - | - |
| elementwise_add_scalar | - | - | - |
| elementwise_mul | - | - | - |
| elementwise_mul_plain | - | - | - |
| elementwise_mul_scalar | - | - | - |
| embedding | - | - | - |
| extract | - | - | - |
| extract_linear | - | - | - |
| extract_sparse | - | - | - |
| relu_composite | 0.0118026 | 2.30792e-06 | 0.0118007 |
| table_mult | - | - | - |

## Re-scored kernels

| task | kernel | used by | correct | eval (ms) | speedup |
|---|---|---|---|---:|---:|
| chebyshev_relu_d7 | baseline | baseline | True | 177.06 | 1.0 |
| chebyshev_relu_d7 | k1 | archive/run1 | True | 126.13 | 1.404 |
| chebyshev_relu_d2 | baseline | baseline | True | 60.13 | 1.0 |
| chebyshev_relu_d2 | k1 | archive/run1 | True | 47.13 | 1.276 |
| relu_composite | baseline | baseline | True | 370.15 | 1.0 |
| relu_composite | k1 | archive/run1 | True | 365.13 | 1.014 |
| elementwise_add | baseline | archive/run1 | True | 0.53 | 1.0 |
| elementwise_add_plain | baseline | archive/run1 | True | 2.84 | 1.0 |
| elementwise_add_scalar | baseline | archive/run1 | True | 0.28 | 1.0 |
| elementwise_mul | baseline | archive/run1 | True | 36.92 | 1.0 |
| elementwise_mul_plain | baseline | archive/run1 | True | 7.72 | 1.0 |
| elementwise_mul_scalar | baseline | archive/run1 | True | 5.21 | 1.0 |
| extract | baseline | archive/run1 | True | 33.81 | 1.0 |
| extract_linear | baseline | archive/run1 | True | 34.39 | 1.0 |
| extract_sparse | baseline | baseline | True | 101.59 | 1.0 |
| extract_sparse | k1 | archive/run1 | True | 73.69 | 1.379 |
| table_mult | baseline | archive/run1 | True | 663.67 | 1.0 |
| embedding | baseline | archive/run1 | True | 759.42 | 1.0 |
