# Phase-2 pilot: neutral re-scoring

Commit 30c298e840 (clean), Darwin arm64 arm, Apple clang version 17.0.0 (clang-1700.3.19.1), SEAL 4.1. Re-scored 2026-11-02T02:48:52.

Speedup = baseline eval-only median / kernel eval-only median, both from the same neutral session (1 warmup + 10 measured processes of ./main --repeats 10, interleaved). The final kernel of each run is the strategy's own selection; re-scoring never re-selects. E2E = CHEHAB execution_time_(ms) (parse + encode + encrypt + fhe()). Compile time: median of 3, reported only.

## Every run

| task | strategy | run | final | speedup | eval (ms) | e2e (ms) | FHE err | depth | xdepth | levels | ct x ct | ct x pt | align | rescale | mod sw | C (ms) | correct / 8 | dup | failures | out tok | think tok | search (s) |
|---|---|---:|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---|---:|---:|---:|
| nn_avgpool2d | archive | 1 | #0 (baseline) | x1.000 | 4.3 | 174.2 | 0.00e+00 | 3 | 0 |  | 0 | 0 | 0 | 0 | 0 | 0.25 | 3 / 8 | 5 | - | 10175 | 5978 | 141.7 |
| nn_conv2d | archive | 1 | #1 (k1) | x1.428 | 2659.0 | 2891.1 | 0.00e+00 | 13 | 1 |  | 100 | 0 | 0 | 0 | 0 | 0.77 | 2 / 8 | 6 | - | 146199 | 124166 | 923.4 |
| nn_conv2d | archive | 2 | #1 (k2) | x1.432 | 2653.3 | 2888.3 | 0.00e+00 | 10 | 1 |  | 100 | 0 | 0 | 0 | 0 | 0.86 | 2 / 8 | 6 | - | 123844 | 104008 | 952.5 |
| nn_conv2d | archive | 3 | #1 (k3) | x1.429 | 2658.3 | 2897.1 | 0.00e+00 | 14 | 1 |  | 100 | 0 | 0 | 0 | 0 | 0.90 | 3 / 8 | 5 | - | 115013 | 95911 | 883.8 |
| nn_linear | archive | 1 | #0 (baseline) | x1.000 | 949.4 | 1184.5 | 0.00e+00 | 7 | 1 |  | 36 | 0 | 0 | 0 | 0 | 0.33 | 4 / 8 | 4 | - | 40369 | 34098 | 445.7 |

## Per task and strategy (3 runs each)

| task | strategy | runs | speedup |
|---|---|---:|---|
| nn_avgpool2d | archive | 1 | median x1.000, mean x1.000, min x1.000, max x1.000, sd 0.000 |
| nn_conv2d | archive | 3 | median x1.429, mean x1.430, min x1.428, max x1.432, sd 0.002 |
| nn_linear | archive | 1 | median x1.000, mean x1.000, min x1.000, max x1.000, sd 0.000 |

## Across tasks (geometric mean of the per-task mean speedups)

| strategy | tasks | geometric mean |
|---|---:|---:|
| archive | 3 | x1.127 |

## Approximation error (task property, separate from FHE error)

| task | max_fit_error (fitting grid) | approx. error, fixed points | approx. error, dense points |
|---|---:|---:|---:|
| nn_avgpool2d | - | - | - |
| nn_conv2d | - | - | - |
| nn_linear | - | - | - |

## Re-scored kernels

| task | kernel | used by | correct | eval (ms) | speedup |
|---|---|---|---|---:|---:|
| nn_linear | baseline | archive/run1 | True | 949.4 | 1.0 |
| nn_avgpool2d | baseline | archive/run1 | True | 4.34 | 1.0 |
| nn_conv2d | baseline | baseline | True | 3798.35 | 1.0 |
| nn_conv2d | k1 | archive/run1 | True | 2658.98 | 1.428 |
| nn_conv2d | k2 | archive/run2 | True | 2653.31 | 1.432 |
| nn_conv2d | k3 | archive/run3 | True | 2658.28 | 1.429 |
