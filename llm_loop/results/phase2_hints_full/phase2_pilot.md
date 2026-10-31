# Phase-2 pilot: neutral re-scoring

Commit af0dad4ecf (clean), Darwin arm64 arm, Apple clang version 17.0.0 (clang-1700.3.19.1), SEAL 4.1. Re-scored 2026-10-27T00:01:40.

Speedup = baseline eval-only median / kernel eval-only median, both from the same neutral session (1 warmup + 10 measured processes of ./main --repeats 10, interleaved). The final kernel of each run is the strategy's own selection; re-scoring never re-selects. E2E = CHEHAB execution_time_(ms) (parse + encode + encrypt + fhe()). Compile time: median of 3, reported only.

## Every run

| task | strategy | run | final | speedup | eval (ms) | e2e (ms) | FHE err | depth | xdepth | levels | ct x ct | ct x pt | align | rescale | mod sw | C (ms) | correct / 8 | dup | failures | out tok | think tok | search (s) |
|---|---|---:|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---|---:|---:|---:|
| chebyshev_elu_d7 | archive | 1 | #5 (k1) | x1.257 | 164.1 | 193.8 | 2.26e-07 | 12 | 5 | 5 | 4 | 5 | 3 | 12 | 5 | 0.57 | 5 / 8 | 2 | - | 106364 | 96576 | 607.6 |
| chebyshev_elu_d7 | archive | 2 | #2 (k2) | x1.129 | 182.7 | 213.6 | 1.59e-07 | 17 | 6 | 6 | 4 | 7 | 9 | 20 | 7 | 0.69 | 6 / 8 | 0 | chehab 1 | 149739 | 139177 | 850.5 |
| chebyshev_elu_d7 | archive | 3 | #1 (k3) | x1.129 | 182.7 | 212.3 | 1.17e-07 | 17 | 6 | 6 | 4 | 7 | 9 | 20 | 7 | 0.60 | 5 / 8 | 2 | compile 1 | 156937 | 147900 | 865.3 |
| chebyshev_elu_d7 | linear | 1 | #1 (k4) | x1.067 | 193.3 | 222.7 | 1.49e-07 | 13 | 5 | 5 | 4 | 9 | 6 | 19 | 3 | 0.58 | 4 / 8 | 4 | - | 48667 | 41656 | 345.8 |
| chebyshev_elu_d7 | linear | 2 | #3 (k5) | x1.149 | 179.5 | 210.1 | 1.40e-07 | 19 | 6 | 6 | 4 | 7 | 8 | 19 | 6 | 0.59 | 4 / 8 | 4 | - | 74605 | 67755 | 449.3 |
| chebyshev_elu_d7 | linear | 3 | #2 (k6) | x1.129 | 182.7 | 213.1 | 1.50e-07 | 17 | 6 | 6 | 4 | 7 | 9 | 20 | 7 | 0.64 | 2 / 8 | 6 | - | 60457 | 52377 | 355.0 |
| chebyshev_gelu_d7 | archive | 1 | #3 (k1) | x1.413 | 122.7 | 149.2 | 1.78e-07 | 11 | 4 | 4 | 3 | 4 | 2 | 9 | 1 | 0.51 | 4 / 8 | 2 | chehab 2 | 59725 | 53110 | 398.1 |
| chebyshev_gelu_d7 | archive | 2 | #3 (k2) | x1.458 | 118.8 | 145.3 | 1.67e-07 | 14 | 5 | 5 | 3 | 4 | 2 | 9 | 4 | 0.44 | 4 / 8 | 1 | chehab 3 | 86845 | 80408 | 519.2 |
| chebyshev_gelu_d7 | archive | 3 | #4 (k3) | x1.467 | 118.2 | 145.3 | 1.50e-07 | 15 | 5 | 5 | 3 | 4 | 2 | 9 | 4 | 0.46 | 6 / 8 | 1 | chehab 1 | 72893 | 67231 | 474.4 |
| chebyshev_gelu_d7 | linear | 1 | #4 (k4) | x1.467 | 118.1 | 144.9 | 1.72e-07 | 15 | 5 | 5 | 3 | 4 | 2 | 9 | 4 | 0.45 | 5 / 8 | 2 | chehab 1 | 40560 | 34935 | 297.7 |
| chebyshev_gelu_d7 | linear | 2 | #6 (k5) | x1.548 | 111.9 | 139.1 | 1.81e-07 | 18 | 5 | 5 | 3 | 3 | 1 | 7 | 4 | 0.52 | 6 / 8 | 2 | - | 75619 | 69898 | 480.4 |
| chebyshev_gelu_d7 | linear | 3 | #3 (k6) | x1.412 | 122.7 | 149.1 | 1.79e-07 | 11 | 4 | 4 | 3 | 4 | 2 | 9 | 1 | 0.45 | 2 / 8 | 4 | chehab 2 | 47504 | 41798 | 296.3 |
| chebyshev_sigmoid_d7 | archive | 1 | #3 (k1) | x1.280 | 144.0 | 172.0 | 1.39e-07 | 15 | 5 | 5 | 4 | 4 | 1 | 9 | 3 | 0.51 | 3 / 8 | 3 | chehab 1, runtime 1 | 117628 | 109371 | 641.8 |
| chebyshev_sigmoid_d7 | archive | 2 | #2 (k2) | x1.229 | 150.0 | 178.3 | 6.13e-08 | 17 | 6 | 6 | 4 | 4 | 5 | 13 | 6 | 0.57 | 5 / 8 | 0 | chehab 3 | 72026 | 64401 | 452.9 |
| chebyshev_sigmoid_d7 | archive | 3 | #7 (k3) | x1.364 | 135.0 | 163.1 | 1.59e-07 | 17 | 6 | 6 | 4 | 4 | 2 | 10 | 6 | 0.55 | 5 / 8 | 1 | chehab 2 | 116523 | 109627 | 662.9 |
| chebyshev_sigmoid_d7 | linear | 1 | #1 (k4) | x1.194 | 154.3 | 183.1 | 5.42e-08 | 17 | 6 | 6 | 4 | 4 | 6 | 14 | 6 | 0.56 | 3 / 8 | 4 | chehab 1 | 58560 | 52290 | 360.3 |
| chebyshev_sigmoid_d7 | linear | 2 | #4 (k5) | x1.343 | 137.1 | 164.3 | 8.29e-08 | 13 | 5 | 5 | 4 | 3 | 1 | 8 | 3 | 0.53 | 4 / 8 | 2 | compile 1, chehab 1 | 97705 | 90745 | 549.7 |
| chebyshev_sigmoid_d7 | linear | 3 | #2 (k6) | x1.228 | 150.1 | 178.1 | 5.91e-08 | 17 | 6 | 6 | 4 | 4 | 5 | 13 | 6 | 0.56 | 5 / 8 | 3 | - | 59534 | 52025 | 400.3 |

## Per task and strategy (3 runs each)

| task | strategy | runs | speedup |
|---|---|---:|---|
| chebyshev_elu_d7 | archive | 3 | median x1.129, mean x1.172, min x1.129, max x1.257, sd 0.074 |
| chebyshev_gelu_d7 | archive | 3 | median x1.458, mean x1.446, min x1.413, max x1.467, sd 0.029 |
| chebyshev_sigmoid_d7 | archive | 3 | median x1.280, mean x1.291, min x1.229, max x1.364, sd 0.069 |
| chebyshev_elu_d7 | linear | 3 | median x1.129, mean x1.115, min x1.067, max x1.149, sd 0.043 |
| chebyshev_gelu_d7 | linear | 3 | median x1.467, mean x1.476, min x1.412, max x1.548, sd 0.069 |
| chebyshev_sigmoid_d7 | linear | 3 | median x1.228, mean x1.255, min x1.194, max x1.343, sd 0.078 |

## Across tasks (geometric mean of the per-task mean speedups)

| strategy | tasks | geometric mean |
|---|---:|---:|
| archive | 3 | x1.298 |
| linear | 3 | x1.274 |

## Approximation error (task property, separate from FHE error)

| task | max_fit_error (fitting grid) | approx. error, fixed points | approx. error, dense points |
|---|---:|---:|---:|
| chebyshev_elu_d7 | 0.0427493 | 0.0427493 | 0.0330832 |
| chebyshev_gelu_d7 | 0.0581538 | 0.0581538 | 0.0470933 |
| chebyshev_sigmoid_d7 | 0.002824 | 0.002824 | 0.0021268 |

## Re-scored kernels

| task | kernel | used by | correct | eval (ms) | speedup |
|---|---|---|---|---:|---:|
| chebyshev_gelu_d7 | baseline | baseline | True | 173.33 | 1.0 |
| chebyshev_gelu_d7 | k1 | archive/run1 | True | 122.69 | 1.413 |
| chebyshev_gelu_d7 | k2 | archive/run2 | True | 118.84 | 1.458 |
| chebyshev_gelu_d7 | k3 | archive/run3 | True | 118.15 | 1.467 |
| chebyshev_gelu_d7 | k4 | linear/run1 | True | 118.11 | 1.467 |
| chebyshev_gelu_d7 | k5 | linear/run2 | True | 111.94 | 1.548 |
| chebyshev_gelu_d7 | k6 | linear/run3 | True | 122.75 | 1.412 |
| chebyshev_sigmoid_d7 | baseline | baseline | True | 184.24 | 1.0 |
| chebyshev_sigmoid_d7 | k1 | archive/run1 | True | 143.96 | 1.28 |
| chebyshev_sigmoid_d7 | k2 | archive/run2 | True | 149.96 | 1.229 |
| chebyshev_sigmoid_d7 | k3 | archive/run3 | True | 135.04 | 1.364 |
| chebyshev_sigmoid_d7 | k4 | linear/run1 | True | 154.31 | 1.194 |
| chebyshev_sigmoid_d7 | k5 | linear/run2 | True | 137.14 | 1.343 |
| chebyshev_sigmoid_d7 | k6 | linear/run3 | True | 150.05 | 1.228 |
| chebyshev_elu_d7 | baseline | baseline | True | 206.26 | 1.0 |
| chebyshev_elu_d7 | k1 | archive/run1 | True | 164.08 | 1.257 |
| chebyshev_elu_d7 | k2 | archive/run2 | True | 182.67 | 1.129 |
| chebyshev_elu_d7 | k3 | archive/run3 | True | 182.66 | 1.129 |
| chebyshev_elu_d7 | k4 | linear/run1 | True | 193.3 | 1.067 |
| chebyshev_elu_d7 | k5 | linear/run2 | True | 179.46 | 1.149 |
| chebyshev_elu_d7 | k6 | linear/run3 | True | 182.71 | 1.129 |
