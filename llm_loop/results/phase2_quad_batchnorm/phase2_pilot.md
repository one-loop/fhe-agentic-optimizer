# Phase-2 pilot: neutral re-scoring

Commit af0dad4ecf (clean), Darwin arm64 arm, Apple clang version 17.0.0 (clang-1700.3.19.1), SEAL 4.1. Re-scored 2026-10-27T12:01:34.

Speedup = baseline eval-only median / kernel eval-only median, both from the same neutral session (1 warmup + 10 measured processes of ./main --repeats 10, interleaved). The final kernel of each run is the strategy's own selection; re-scoring never re-selects. E2E = CHEHAB execution_time_(ms) (parse + encode + encrypt + fhe()). Compile time: median of 3, reported only.

## Every run

| task | strategy | run | final | speedup | eval (ms) | e2e (ms) | FHE err | depth | xdepth | levels | ct x ct | ct x pt | align | rescale | mod sw | C (ms) | correct / 8 | dup | failures | out tok | think tok | search (s) |
|---|---|---:|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---|---:|---:|---:|
| batchnorm1d | archive | 1 | #0 (baseline) | x1.000 | 10.7 | 33.1 | 6.27e-08 | 2 | 1 | 1 | 0 | 1 | 0 | 1 | 0 | 0.20 | 1 / 8 | 7 | - | 5779 | 3656 | 79.9 |
| batchnorm1d | linear | 1 | #0 (baseline) | x1.000 | 10.7 | 33.1 | 6.27e-08 | 2 | 1 | 1 | 0 | 1 | 0 | 1 | 0 | 0.20 | 1 / 8 | 7 | - | 6274 | 4218 | 69.0 |
| batchnorm2d | archive | 1 | #5 (k1) | x1.008 | 10.7 | 32.5 | 3.28e-08 | 2 | 1 | 1 | 0 | 1 | 0 | 1 | 0 | 0.37 | 1 / 8 | 7 | - | 6489 | 4238 | 83.9 |
| batchnorm2d | linear | 1 | #0 (baseline) | x1.000 | 10.8 | 32.5 | 4.14e-08 | 2 | 1 | 1 | 0 | 1 | 0 | 1 | 0 | 0.38 | 0 / 8 | 8 | - | 5837 | 3935 | 67.4 |
| quad | archive | 1 | #0 (baseline) | x1.000 | 36.5 | 60.6 | 7.25e-08 | 1 | 1 | 1 | 1 | 0 | 0 | 1 | 0 | 0.23 | 1 / 8 | 7 | - | 4033 | 2015 | 74.5 |
| quad | linear | 1 | #0 (baseline) | x1.000 | 36.5 | 60.6 | 7.25e-08 | 1 | 1 | 1 | 1 | 0 | 0 | 1 | 0 | 0.23 | 1 / 8 | 7 | - | 3043 | 1514 | 57.5 |

## Per task and strategy (3 runs each)

| task | strategy | runs | speedup |
|---|---|---:|---|
| batchnorm1d | archive | 1 | median x1.000, mean x1.000, min x1.000, max x1.000, sd 0.000 |
| batchnorm2d | archive | 1 | median x1.008, mean x1.008, min x1.008, max x1.008, sd 0.000 |
| quad | archive | 1 | median x1.000, mean x1.000, min x1.000, max x1.000, sd 0.000 |
| batchnorm1d | linear | 1 | median x1.000, mean x1.000, min x1.000, max x1.000, sd 0.000 |
| batchnorm2d | linear | 1 | median x1.000, mean x1.000, min x1.000, max x1.000, sd 0.000 |
| quad | linear | 1 | median x1.000, mean x1.000, min x1.000, max x1.000, sd 0.000 |

## Across tasks (geometric mean of the per-task mean speedups)

| strategy | tasks | geometric mean |
|---|---:|---:|
| archive | 3 | x1.003 |
| linear | 3 | x1.000 |

## Approximation error (task property, separate from FHE error)

| task | max_fit_error (fitting grid) | approx. error, fixed points | approx. error, dense points |
|---|---:|---:|---:|
| batchnorm1d | - | - | - |
| batchnorm2d | - | - | - |
| quad | - | - | - |

## Re-scored kernels

| task | kernel | used by | correct | eval (ms) | speedup |
|---|---|---|---|---:|---:|
| quad | baseline | archive/run1 linear/run1 | True | 36.54 | 1.0 |
| batchnorm1d | baseline | archive/run1 linear/run1 | True | 10.73 | 1.0 |
| batchnorm2d | baseline | linear/run1 | True | 10.79 | 1.0 |
| batchnorm2d | k1 | archive/run1 | True | 10.71 | 1.008 |
