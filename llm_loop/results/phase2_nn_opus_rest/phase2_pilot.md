# Phase-2 pilot: neutral re-scoring

Commit 1d44c9e562 (clean), Darwin arm64 arm, Apple clang version 17.0.0 (clang-1700.3.19.1), SEAL 4.1. Re-scored 2026-11-10T01:32:38.

Speedup = baseline eval-only median / kernel eval-only median, both from the same neutral session (1 warmup + 10 measured processes of ./main --repeats 10, interleaved). The final kernel of each run is the strategy's own selection; re-scoring never re-selects. E2E = CHEHAB execution_time_(ms) (parse + encode + encrypt + fhe()). Compile time: median of 3, reported only.

## Every run

| task | strategy | run | final | speedup | eval (ms) | e2e (ms) | FHE err | depth | xdepth | levels | ct x ct | ct x pt | align | rescale | mod sw | C (ms) | correct / 8 | dup | failures | out tok | think tok | search (s) |
|---|---|---:|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---|---:|---:|---:|
| nn_avgpool2d | archive | 1 | #2 (k1) | x1.007 | 4.5 | 182.8 | 0.00e+00 | 2 | 0 |  | 0 | 0 | 0 | 0 | 0 | 1.04 | 5 / 8 | 3 | - | 5005 | 1068 | 166.0 |
| nn_linear | archive | 1 | #0 (baseline) | x1.000 | 954.4 | 1196.3 | 0.00e+00 | 7 | 1 |  | 36 | 0 | 0 | 0 | 0 | 0.36 | 6 / 8 | 2 | - | 12838 | 7888 | 473.2 |

## Per task and strategy (3 runs each)

| task | strategy | runs | speedup |
|---|---|---:|---|
| nn_avgpool2d | archive | 1 | median x1.007, mean x1.007, min x1.007, max x1.007, sd 0.000 |
| nn_linear | archive | 1 | median x1.000, mean x1.000, min x1.000, max x1.000, sd 0.000 |

## Across tasks (geometric mean of the per-task mean speedups)

| strategy | tasks | geometric mean |
|---|---:|---:|
| archive | 2 | x1.003 |

## Approximation error (task property, separate from FHE error)

| task | max_fit_error (fitting grid) | approx. error, fixed points | approx. error, dense points |
|---|---:|---:|---:|
| nn_avgpool2d | - | - | - |
| nn_linear | - | - | - |

## Re-scored kernels

| task | kernel | used by | correct | eval (ms) | speedup |
|---|---|---|---|---:|---:|
| nn_linear | baseline | archive/run1 | True | 954.41 | 1.0 |
| nn_avgpool2d | baseline | baseline | True | 4.53 | 1.0 |
| nn_avgpool2d | k1 | archive/run1 | True | 4.49 | 1.007 |
