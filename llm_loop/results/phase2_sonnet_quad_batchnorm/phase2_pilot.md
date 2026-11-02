# Phase-2 pilot: neutral re-scoring

Commit 45234b7a2d (clean), Darwin arm64 arm, Apple clang version 17.0.0 (clang-1700.3.19.1), SEAL 4.1. Re-scored 2026-11-02T04:33:43.

Speedup = baseline eval-only median / kernel eval-only median, both from the same neutral session (1 warmup + 10 measured processes of ./main --repeats 10, interleaved). The final kernel of each run is the strategy's own selection; re-scoring never re-selects. E2E = CHEHAB execution_time_(ms) (parse + encode + encrypt + fhe()). Compile time: median of 3, reported only.

## Every run

| task | strategy | run | final | speedup | eval (ms) | e2e (ms) | FHE err | depth | xdepth | levels | ct x ct | ct x pt | align | rescale | mod sw | C (ms) | correct / 8 | dup | failures | out tok | think tok | search (s) |
|---|---|---:|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---|---:|---:|---:|
| batchnorm1d | archive | 1 | #0 (baseline) | x1.000 | 10.7 | 32.9 | 3.78e-08 | 2 | 1 | 1 | 0 | 1 | 0 | 1 | 0 | 0.22 | 3 / 8 | 5 | - | 3781 | 2163 | 92.7 |
| batchnorm2d | archive | 1 | #0 (baseline) | x1.000 | 10.7 | 33.0 | 4.08e-08 | 2 | 1 | 1 | 0 | 1 | 0 | 1 | 0 | 0.37 | 2 / 8 | 6 | - | 3764 | 2141 | 90.0 |
| quad | archive | 1 | #0 (baseline) | x1.000 | 36.4 | 60.3 | 1.18e-07 | 1 | 1 | 1 | 1 | 0 | 0 | 1 | 0 | 0.30 | 1 / 8 | 7 | - | 1255 | 0 | 67.9 |

## Per task and strategy (3 runs each)

| task | strategy | runs | speedup |
|---|---|---:|---|
| batchnorm1d | archive | 1 | median x1.000, mean x1.000, min x1.000, max x1.000, sd 0.000 |
| batchnorm2d | archive | 1 | median x1.000, mean x1.000, min x1.000, max x1.000, sd 0.000 |
| quad | archive | 1 | median x1.000, mean x1.000, min x1.000, max x1.000, sd 0.000 |

## Across tasks (geometric mean of the per-task mean speedups)

| strategy | tasks | geometric mean |
|---|---:|---:|
| archive | 3 | x1.000 |

## Approximation error (task property, separate from FHE error)

| task | max_fit_error (fitting grid) | approx. error, fixed points | approx. error, dense points |
|---|---:|---:|---:|
| batchnorm1d | - | - | - |
| batchnorm2d | - | - | - |
| quad | - | - | - |

## Re-scored kernels

| task | kernel | used by | correct | eval (ms) | speedup |
|---|---|---|---|---:|---:|
| quad | baseline | archive/run1 | True | 36.4 | 1.0 |
| batchnorm1d | baseline | archive/run1 | True | 10.73 | 1.0 |
| batchnorm2d | baseline | archive/run1 | True | 10.74 | 1.0 |
