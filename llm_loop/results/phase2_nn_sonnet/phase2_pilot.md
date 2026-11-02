# Phase-2 pilot: neutral re-scoring

Commit 45234b7a2d (clean), Darwin arm64 arm, Apple clang version 17.0.0 (clang-1700.3.19.1), SEAL 4.1. Re-scored 2026-11-02T04:27:45.

Speedup = baseline eval-only median / kernel eval-only median, both from the same neutral session (1 warmup + 10 measured processes of ./main --repeats 10, interleaved). The final kernel of each run is the strategy's own selection; re-scoring never re-selects. E2E = CHEHAB execution_time_(ms) (parse + encode + encrypt + fhe()). Compile time: median of 3, reported only.

## Every run

| task | strategy | run | final | speedup | eval (ms) | e2e (ms) | FHE err | depth | xdepth | levels | ct x ct | ct x pt | align | rescale | mod sw | C (ms) | correct / 8 | dup | failures | out tok | think tok | search (s) |
|---|---|---:|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---|---:|---:|---:|
| nn_avgpool2d | archive | 1 | #6 (k1) | x1.010 | 4.4 | 175.7 | 0.00e+00 | 2 | 0 |  | 0 | 0 | 0 | 0 | 0 | 0.27 | 3 / 8 | 5 | - | 5181 | 1480 | 130.0 |
| nn_conv2d | archive | 1 | #1 (k1) | x2.216 | 1708.7 | 1947.2 | 0.00e+00 | 10 | 1 |  | 64 | 0 | 0 | 0 | 0 | 0.83 | 3 / 8 | 5 | - | 50325 | 30454 | 637.1 |
| nn_conv2d | archive | 2 | #7 (k2) | x1.351 | 2802.8 | 3038.8 | 0.00e+00 | 11 | 1 |  | 106 | 0 | 0 | 0 | 0 | 0.80 | 7 / 8 | 0 | chehab 1 | 42707 | 32202 | 1247.1 |
| nn_conv2d | archive | 3 | #3 (k3) | x1.299 | 2915.0 | 3153.6 | 0.00e+00 | 15 | 1 |  | 110 | 0 | 0 | 0 | 0 | 0.91 | 3 / 8 | 4 | wrong 1 | 58197 | 45662 | 924.8 |
| nn_linear | archive | 1 | #0 (baseline) | x1.000 | 949.7 | 1188.3 | 0.00e+00 | 7 | 1 |  | 36 | 0 | 0 | 0 | 0 | 0.37 | 4 / 8 | 4 | - | 12739 | 8405 | 338.5 |

## Per task and strategy (3 runs each)

| task | strategy | runs | speedup |
|---|---|---:|---|
| nn_avgpool2d | archive | 1 | median x1.010, mean x1.010, min x1.010, max x1.010, sd 0.000 |
| nn_conv2d | archive | 3 | median x1.351, mean x1.622, min x1.299, max x2.216, sd 0.515 |
| nn_linear | archive | 1 | median x1.000, mean x1.000, min x1.000, max x1.000, sd 0.000 |

## Across tasks (geometric mean of the per-task mean speedups)

| strategy | tasks | geometric mean |
|---|---:|---:|
| archive | 3 | x1.179 |

## Approximation error (task property, separate from FHE error)

| task | max_fit_error (fitting grid) | approx. error, fixed points | approx. error, dense points |
|---|---:|---:|---:|
| nn_avgpool2d | - | - | - |
| nn_conv2d | - | - | - |
| nn_linear | - | - | - |

## Re-scored kernels

| task | kernel | used by | correct | eval (ms) | speedup |
|---|---|---|---|---:|---:|
| nn_linear | baseline | archive/run1 | True | 949.73 | 1.0 |
| nn_avgpool2d | baseline | baseline | True | 4.41 | 1.0 |
| nn_avgpool2d | k1 | archive/run1 | True | 4.37 | 1.01 |
| nn_conv2d | baseline | baseline | True | 3786.83 | 1.0 |
| nn_conv2d | k1 | archive/run1 | True | 1708.68 | 2.216 |
| nn_conv2d | k2 | archive/run2 | True | 2802.76 | 1.351 |
| nn_conv2d | k3 | archive/run3 | True | 2915.03 | 1.299 |
