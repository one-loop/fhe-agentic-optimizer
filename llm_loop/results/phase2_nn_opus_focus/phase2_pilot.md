# Phase-2 pilot: neutral re-scoring

Commit 3c62a442cd (clean), Darwin arm64 arm, Apple clang version 17.0.0 (clang-1700.3.19.1), SEAL 4.1. Re-scored 2026-11-02T11:16:36.

Speedup = baseline eval-only median / kernel eval-only median, both from the same neutral session (1 warmup + 10 measured processes of ./main --repeats 10, interleaved). The final kernel of each run is the strategy's own selection; re-scoring never re-selects. E2E = CHEHAB execution_time_(ms) (parse + encode + encrypt + fhe()). Compile time: median of 3, reported only.

## Every run

| task | strategy | run | final | speedup | eval (ms) | e2e (ms) | FHE err | depth | xdepth | levels | ct x ct | ct x pt | align | rescale | mod sw | C (ms) | correct / 8 | dup | failures | out tok | think tok | search (s) |
|---|---|---:|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---|---:|---:|---:|
| nn_conv2d | archive | 1 | #5 (k1) | x2.346 | 1613.5 | 1856.0 | 0.00e+00 | 23 | 1 |  | 60 | 0 | 0 | 0 | 0 | 3.75 | 6 / 8 | 0 | wrong 2 | 144768 | 96542 | 1829.8 |
| nn_conv2d | archive | 2 | #5 (k2) | x3.738 | 1012.5 | 1241.6 | 0.00e+00 | 24 | 2 |  | 36 | 9 | 0 | 0 | 0 | 1.11 | 7 / 8 | 0 | wrong 1 | 97672 | 79109 | 1437.8 |
| nn_conv2d | archive | 3 | #8 (k3) | x2.312 | 1636.8 | 1872.9 | 0.00e+00 | 19 | 1 |  | 61 | 0 | 0 | 0 | 0 | 0.86 | 7 / 8 | 0 | wrong 1 | 137026 | 112025 | 1869.0 |

## Per task and strategy (3 runs each)

| task | strategy | runs | speedup |
|---|---|---:|---|
| nn_conv2d | archive | 3 | median x2.346, mean x2.799, min x2.312, max x3.738, sd 0.814 |

## Across tasks (geometric mean of the per-task mean speedups)

| strategy | tasks | geometric mean |
|---|---:|---:|
| archive | 1 | x2.799 |

## Approximation error (task property, separate from FHE error)

| task | max_fit_error (fitting grid) | approx. error, fixed points | approx. error, dense points |
|---|---:|---:|---:|
| nn_conv2d | - | - | - |

## Re-scored kernels

| task | kernel | used by | correct | eval (ms) | speedup |
|---|---|---|---|---:|---:|
| nn_conv2d | baseline | baseline | True | 3784.69 | 1.0 |
| nn_conv2d | k1 | archive/run1 | True | 1613.45 | 2.346 |
| nn_conv2d | k2 | archive/run2 | True | 1012.48 | 3.738 |
| nn_conv2d | k3 | archive/run3 | True | 1636.81 | 2.312 |
