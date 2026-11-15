# Phase-2 pilot: neutral re-scoring

Commit 1d44c9e562 (clean), Darwin arm64 arm, Apple clang version 17.0.0 (clang-1700.3.19.1), SEAL 4.1. Re-scored 2026-11-09T18:44:02.

Speedup = baseline eval-only median / kernel eval-only median, both from the same neutral session (1 warmup + 10 measured processes of ./main --repeats 10, interleaved). The final kernel of each run is the strategy's own selection; re-scoring never re-selects. E2E = CHEHAB execution_time_(ms) (parse + encode + encrypt + fhe()). Compile time: median of 3, reported only.

## Every run

| task | strategy | run | final | speedup | eval (ms) | e2e (ms) | FHE err | depth | xdepth | levels | ct x ct | ct x pt | align | rescale | mod sw | C (ms) | correct / 8 | dup | failures | out tok | think tok | search (s) |
|---|---|---:|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---|---:|---:|---:|
| batchnorm1d | archive | 1 | #0 (baseline) | x1.000 | 11.1 | 33.9 | 5.31e-08 | 2 | 1 | 1 | 0 | 1 | 0 | 1 | 0 | 0.30 | 0 / 8 | 8 | - | 2921 | 1086 | 75.6 |
| batchnorm2d | archive | 1 | #0 (baseline) | x1.000 | 11.0 | 33.9 | 6.54e-08 | 2 | 1 | 1 | 0 | 1 | 0 | 1 | 0 | 0.33 | 2 / 8 | 6 | - | 2874 | 1055 | 87.1 |
| quad | archive | 1 | #1 (k1) | x1.005 | 37.2 | 62.7 | 1.03e-07 | 1 | 1 | 1 | 1 | 0 | 0 | 1 | 0 | 0.32 | 1 / 8 | 7 | - | 1737 | 300 | 74.7 |

## Per task and strategy (3 runs each)

| task | strategy | runs | speedup |
|---|---|---:|---|
| batchnorm1d | archive | 1 | median x1.000, mean x1.000, min x1.000, max x1.000, sd 0.000 |
| batchnorm2d | archive | 1 | median x1.000, mean x1.000, min x1.000, max x1.000, sd 0.000 |
| quad | archive | 1 | median x1.005, mean x1.005, min x1.005, max x1.005, sd 0.000 |

## Across tasks (geometric mean of the per-task mean speedups)

| strategy | tasks | geometric mean |
|---|---:|---:|
| archive | 3 | x1.002 |

## Approximation error (task property, separate from FHE error)

| task | max_fit_error (fitting grid) | approx. error, fixed points | approx. error, dense points |
|---|---:|---:|---:|
| batchnorm1d | - | - | - |
| batchnorm2d | - | - | - |
| quad | - | - | - |

## Re-scored kernels

| task | kernel | used by | correct | eval (ms) | speedup |
|---|---|---|---|---:|---:|
| quad | baseline | baseline | True | 37.34 | 1.0 |
| quad | k1 | archive/run1 | True | 37.16 | 1.005 |
| batchnorm1d | baseline | archive/run1 | True | 11.11 | 1.0 |
| batchnorm2d | baseline | archive/run1 | True | 11.03 | 1.0 |
