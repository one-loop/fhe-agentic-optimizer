# Phase-2 pilot: neutral re-scoring

Commit 3c62a442cd (clean), Darwin arm64 arm, Apple clang version 17.0.0 (clang-1700.3.19.1), SEAL 4.1. Re-scored 2026-11-02T06:04:15.

Speedup = baseline eval-only median / kernel eval-only median, both from the same neutral session (1 warmup + 10 measured processes of ./main --repeats 10, interleaved). The final kernel of each run is the strategy's own selection; re-scoring never re-selects. E2E = CHEHAB execution_time_(ms) (parse + encode + encrypt + fhe()). Compile time: median of 3, reported only.

## Every run

| task | strategy | run | final | speedup | eval (ms) | e2e (ms) | FHE err | depth | xdepth | levels | ct x ct | ct x pt | align | rescale | mod sw | C (ms) | correct / 8 | dup | failures | out tok | think tok | search (s) |
|---|---|---:|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---|---:|---:|---:|
| chebyshev_relu_d2 | archive | 1 | #1 (k1) | x1.275 | 47.0 | 71.9 | 1.51e-07 | 7 | 3 | 3 | 1 | 2 | 0 | 3 | 1 | 0.33 | 3 / 8 | 5 | - | 46999 | 43277 | 326.3 |
| chebyshev_relu_d7 | archive | 1 | #5 (k1) | x1.410 | 122.6 | 148.8 | 2.18e-07 | 9 | 4 | 4 | 3 | 4 | 2 | 9 | 0 | 0.48 | 6 / 8 | 1 | chehab 1 | 65796 | 60035 | 513.5 |
| elementwise_add | archive | 1 | #0 (baseline) | x1.000 | 0.5 | 41.6 | 3.93e-08 | 1 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0.31 | 0 / 8 | 8 | - | 3406 | 1585 | 45.8 |
| elementwise_add_plain | archive | 1 | #0 (baseline) | x1.000 | 2.8 | 23.9 | 2.85e-08 | 1 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0.31 | 0 / 8 | 8 | - | 4253 | 2382 | 55.2 |
| elementwise_add_scalar | archive | 1 | #0 (baseline) | x1.000 | 0.3 | 21.2 | 3.34e-08 | 1 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0.29 | 0 / 8 | 8 | - | 3991 | 2275 | 52.0 |
| elementwise_mul | archive | 1 | #0 (baseline) | x1.000 | 36.6 | 80.3 | 6.14e-08 | 1 | 1 | 1 | 1 | 0 | 0 | 1 | 0 | 0.33 | 0 / 8 | 8 | - | 3906 | 1820 | 53.1 |
| elementwise_mul_plain | archive | 1 | #0 (baseline) | x1.000 | 7.7 | 29.0 | 5.21e-08 | 1 | 1 | 1 | 0 | 1 | 0 | 1 | 0 | 0.40 | 0 / 8 | 8 | - | 3998 | 2336 | 50.9 |
| elementwise_mul_scalar | archive | 1 | #0 (baseline) | x1.000 | 5.1 | 26.3 | 5.85e-08 | 1 | 1 | 1 | 0 | 1 | 0 | 1 | 0 | 0.29 | 0 / 8 | 8 | - | 4762 | 3048 | 61.3 |
| embedding | archive | 1 | #0 (baseline) | x1.000 | 715.1 | 752.4 | 3.85e-05 | 21 | 2 | 2 | 0 | 6 | 0 | 6 | 0 | 0.43 | 4 / 8 | 1 | compile 1, chehab 2 | 92876 | 88168 | 826.6 |
| extract | archive | 1 | #0 (baseline) | x1.000 | 33.4 | 56.5 | 9.82e-06 | 2 | 1 | 1 | 0 | 1 | 0 | 1 | 0 | 0.28 | 1 / 8 | 6 | wrong 1 | 8611 | 6244 | 108.1 |
| extract_linear | archive | 1 | #0 (baseline) | x1.000 | 33.4 | 56.2 | 1.03e-05 | 2 | 1 | 1 | 0 | 1 | 0 | 1 | 0 | 0.30 | 1 / 8 | 7 | - | 8958 | 6738 | 103.8 |
| extract_sparse | archive | 1 | #2 (k1) | x1.387 | 72.7 | 115.8 | 7.90e-06 | 3 | 1 | 1 | 0 | 2 | 1 | 3 | 0 | 0.35 | 7 / 8 | 1 | - | 52358 | 47641 | 430.8 |
| relu_composite | archive | 1 | #0 (baseline) | x1.000 | 364.4 | 394.5 | 6.54e-07 | 17 | 10 | 10 | 16 | 12 | 9 | 37 | 9 | 0.64 | 3 / 8 | 2 | chehab 3 | 136380 | 128844 | 838.0 |
| table_mult | archive | 1 | #0 (baseline) | x1.000 | 651.5 | 684.9 | 6.72e-05 | 17 | 1 | 1 | 0 | 3 | 0 | 3 | 0 | 0.38 | 1 / 8 | 7 | - | 48732 | 45958 | 369.0 |

## Per task and strategy (3 runs each)

| task | strategy | runs | speedup |
|---|---|---:|---|
| chebyshev_relu_d2 | archive | 1 | median x1.275, mean x1.275, min x1.275, max x1.275, sd 0.000 |
| chebyshev_relu_d7 | archive | 1 | median x1.410, mean x1.410, min x1.410, max x1.410, sd 0.000 |
| elementwise_add | archive | 1 | median x1.000, mean x1.000, min x1.000, max x1.000, sd 0.000 |
| elementwise_add_plain | archive | 1 | median x1.000, mean x1.000, min x1.000, max x1.000, sd 0.000 |
| elementwise_add_scalar | archive | 1 | median x1.000, mean x1.000, min x1.000, max x1.000, sd 0.000 |
| elementwise_mul | archive | 1 | median x1.000, mean x1.000, min x1.000, max x1.000, sd 0.000 |
| elementwise_mul_plain | archive | 1 | median x1.000, mean x1.000, min x1.000, max x1.000, sd 0.000 |
| elementwise_mul_scalar | archive | 1 | median x1.000, mean x1.000, min x1.000, max x1.000, sd 0.000 |
| embedding | archive | 1 | median x1.000, mean x1.000, min x1.000, max x1.000, sd 0.000 |
| extract | archive | 1 | median x1.000, mean x1.000, min x1.000, max x1.000, sd 0.000 |
| extract_linear | archive | 1 | median x1.000, mean x1.000, min x1.000, max x1.000, sd 0.000 |
| extract_sparse | archive | 1 | median x1.387, mean x1.387, min x1.387, max x1.387, sd 0.000 |
| relu_composite | archive | 1 | median x1.000, mean x1.000, min x1.000, max x1.000, sd 0.000 |
| table_mult | archive | 1 | median x1.000, mean x1.000, min x1.000, max x1.000, sd 0.000 |

## Across tasks (geometric mean of the per-task mean speedups)

| strategy | tasks | geometric mean |
|---|---:|---:|
| archive | 14 | x1.067 |

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
| chebyshev_relu_d7 | baseline | baseline | True | 172.95 | 1.0 |
| chebyshev_relu_d7 | k1 | archive/run1 | True | 122.62 | 1.41 |
| chebyshev_relu_d2 | baseline | baseline | True | 59.96 | 1.0 |
| chebyshev_relu_d2 | k1 | archive/run1 | True | 47.02 | 1.275 |
| relu_composite | baseline | archive/run1 | True | 364.44 | 1.0 |
| elementwise_add | baseline | archive/run1 | True | 0.5 | 1.0 |
| elementwise_add_plain | baseline | archive/run1 | True | 2.77 | 1.0 |
| elementwise_add_scalar | baseline | archive/run1 | True | 0.26 | 1.0 |
| elementwise_mul | baseline | archive/run1 | True | 36.62 | 1.0 |
| elementwise_mul_plain | baseline | archive/run1 | True | 7.65 | 1.0 |
| elementwise_mul_scalar | baseline | archive/run1 | True | 5.1 | 1.0 |
| extract | baseline | archive/run1 | True | 33.43 | 1.0 |
| extract_linear | baseline | archive/run1 | True | 33.42 | 1.0 |
| extract_sparse | baseline | baseline | True | 100.83 | 1.0 |
| extract_sparse | k1 | archive/run1 | True | 72.71 | 1.387 |
| table_mult | baseline | archive/run1 | True | 651.53 | 1.0 |
| embedding | baseline | archive/run1 | True | 715.09 | 1.0 |
