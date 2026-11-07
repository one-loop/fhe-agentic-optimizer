# Phase-2 pilot: neutral re-scoring

Commit 29347817aa (clean), Darwin arm64 arm, Apple clang version 17.0.0 (clang-1700.3.19.1), SEAL 4.1. Re-scored 2026-11-07T21:32:18.

Speedup = baseline eval-only median / kernel eval-only median, both from the same neutral session (1 warmup + 10 measured processes of ./main --repeats 10, interleaved). The final kernel of each run is the strategy's own selection; re-scoring never re-selects. E2E = CHEHAB execution_time_(ms) (parse + encode + encrypt + fhe()). Compile time: median of 3, reported only.

## Every run

| task | strategy | run | final | speedup | eval (ms) | e2e (ms) | FHE err | depth | xdepth | levels | ct x ct | ct x pt | align | rescale | mod sw | C (ms) | correct / 8 | dup | failures | out tok | think tok | search (s) |
|---|---|---:|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---|---:|---:|---:|
| chebyshev_relu_d2 | archive | 1 | #1 (k1) | x1.281 | 48.1 | 73.3 | 1.79e-07 | 7 | 3 | 3 | 1 | 2 | 0 | 3 | 1 | 0.40 | 5 / 8 | 3 | - | 18641 | 15461 | 336.7 |
| chebyshev_relu_d7 | archive | 1 | #5 (k1) | x1.438 | 123.5 | 152.4 | 1.99e-07 | 11 | 4 | 4 | 3 | 4 | 2 | 9 | 0 | 0.51 | 7 / 8 | 1 | - | 48863 | 43523 | 743.9 |
| elementwise_add | archive | 1 | #0 (baseline) | x1.000 | 0.6 | 43.7 | 4.70e-08 | 1 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0.55 | 0 / 8 | 8 | - | 2224 | 568 | 62.5 |
| elementwise_add_plain | archive | 1 | #0 (baseline) | x1.000 | 3.0 | 27.6 | 2.98e-08 | 1 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0.54 | 0 / 8 | 8 | - | 2450 | 691 | 64.3 |
| elementwise_add_scalar | archive | 1 | #0 (baseline) | x1.000 | 0.3 | 22.9 | 3.43e-08 | 1 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0.43 | 0 / 8 | 8 | - | 2436 | 869 | 63.2 |
| elementwise_mul | archive | 1 | #0 (baseline) | x1.000 | 38.8 | 85.0 | 5.47e-08 | 1 | 1 | 1 | 1 | 0 | 0 | 1 | 0 | 0.49 | 0 / 8 | 8 | - | 2724 | 908 | 65.6 |
| elementwise_mul_plain | archive | 1 | #0 (baseline) | x1.000 | 8.2 | 31.1 | 4.69e-08 | 1 | 1 | 1 | 0 | 1 | 0 | 1 | 0 | 0.37 | 1 / 8 | 7 | - | 2257 | 559 | 83.1 |
| elementwise_mul_scalar | archive | 1 | #0 (baseline) | x1.000 | 5.4 | 27.5 | 6.18e-08 | 1 | 1 | 1 | 0 | 1 | 0 | 1 | 0 | 0.38 | 1 / 8 | 7 | - | 2372 | 645 | 85.1 |
| embedding | archive | 1 | #0 (baseline) | x1.000 | 740.7 | 769.8 | 5.73e-05 | 21 | 2 | 2 | 0 | 6 | 0 | 6 | 0 | 0.57 | 7 / 8 | 1 | - | 70752 | 65873 | 1415.7 |
| extract | archive | 1 | #0 (baseline) | x1.000 | 35.2 | 61.7 | 5.61e-06 | 2 | 1 | 1 | 0 | 1 | 0 | 1 | 0 | 0.56 | 4 / 8 | 4 | - | 3824 | 1847 | 136.4 |
| extract_linear | archive | 1 | #0 (baseline) | x1.000 | 35.0 | 63.3 | 6.73e-06 | 2 | 1 | 1 | 0 | 1 | 0 | 1 | 0 | 0.52 | 2 / 8 | 6 | - | 4275 | 2417 | 119.2 |
| extract_sparse | archive | 1 | #2 (k1) | x1.373 | 76.5 | 121.4 | 8.48e-06 | 4 | 1 | 1 | 0 | 2 | 1 | 3 | 0 | 0.38 | 7 / 8 | 1 | - | 17912 | 13540 | 364.4 |
| relu_composite | archive | 1 | #0 (baseline) | x1.000 | 377.8 | 406.3 | 9.96e-07 | 17 | 10 | 10 | 16 | 12 | 9 | 37 | 9 | 0.71 | 3 / 8 | 1 | chehab 2, held-out 1 | 120606 | 112949 | 1466.7 |
| table_mult | archive | 1 | #0 (baseline) | x1.000 | 684.1 | 708.4 | 6.85e-05 | 17 | 1 | 1 | 0 | 3 | 0 | 3 | 0 | 0.49 | 6 / 8 | 2 | - | 13870 | 10083 | 655.7 |

## Per task and strategy (3 runs each)

| task | strategy | runs | speedup |
|---|---|---:|---|
| chebyshev_relu_d2 | archive | 1 | median x1.281, mean x1.281, min x1.281, max x1.281, sd 0.000 |
| chebyshev_relu_d7 | archive | 1 | median x1.438, mean x1.438, min x1.438, max x1.438, sd 0.000 |
| elementwise_add | archive | 1 | median x1.000, mean x1.000, min x1.000, max x1.000, sd 0.000 |
| elementwise_add_plain | archive | 1 | median x1.000, mean x1.000, min x1.000, max x1.000, sd 0.000 |
| elementwise_add_scalar | archive | 1 | median x1.000, mean x1.000, min x1.000, max x1.000, sd 0.000 |
| elementwise_mul | archive | 1 | median x1.000, mean x1.000, min x1.000, max x1.000, sd 0.000 |
| elementwise_mul_plain | archive | 1 | median x1.000, mean x1.000, min x1.000, max x1.000, sd 0.000 |
| elementwise_mul_scalar | archive | 1 | median x1.000, mean x1.000, min x1.000, max x1.000, sd 0.000 |
| embedding | archive | 1 | median x1.000, mean x1.000, min x1.000, max x1.000, sd 0.000 |
| extract | archive | 1 | median x1.000, mean x1.000, min x1.000, max x1.000, sd 0.000 |
| extract_linear | archive | 1 | median x1.000, mean x1.000, min x1.000, max x1.000, sd 0.000 |
| extract_sparse | archive | 1 | median x1.373, mean x1.373, min x1.373, max x1.373, sd 0.000 |
| relu_composite | archive | 1 | median x1.000, mean x1.000, min x1.000, max x1.000, sd 0.000 |
| table_mult | archive | 1 | median x1.000, mean x1.000, min x1.000, max x1.000, sd 0.000 |

## Across tasks (geometric mean of the per-task mean speedups)

| strategy | tasks | geometric mean |
|---|---:|---:|
| archive | 14 | x1.069 |

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
| chebyshev_relu_d7 | baseline | baseline | True | 177.57 | 1.0 |
| chebyshev_relu_d7 | k1 | archive/run1 | True | 123.5 | 1.438 |
| chebyshev_relu_d2 | baseline | baseline | True | 61.57 | 1.0 |
| chebyshev_relu_d2 | k1 | archive/run1 | True | 48.07 | 1.281 |
| relu_composite | baseline | archive/run1 | True | 377.8 | 1.0 |
| elementwise_add | baseline | archive/run1 | True | 0.57 | 1.0 |
| elementwise_add_plain | baseline | archive/run1 | True | 3.03 | 1.0 |
| elementwise_add_scalar | baseline | archive/run1 | True | 0.3 | 1.0 |
| elementwise_mul | baseline | archive/run1 | True | 38.79 | 1.0 |
| elementwise_mul_plain | baseline | archive/run1 | True | 8.24 | 1.0 |
| elementwise_mul_scalar | baseline | archive/run1 | True | 5.41 | 1.0 |
| extract | baseline | archive/run1 | True | 35.16 | 1.0 |
| extract_linear | baseline | archive/run1 | True | 34.95 | 1.0 |
| extract_sparse | baseline | baseline | True | 105.1 | 1.0 |
| extract_sparse | k1 | archive/run1 | True | 76.54 | 1.373 |
| table_mult | baseline | archive/run1 | True | 684.12 | 1.0 |
| embedding | baseline | archive/run1 | True | 740.7 | 1.0 |
