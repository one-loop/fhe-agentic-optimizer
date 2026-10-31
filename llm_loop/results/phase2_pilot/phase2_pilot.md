# Phase-2 pilot: neutral re-scoring

Commit 33fc1da13c (clean), Darwin arm64 arm, Apple clang version 17.0.0 (clang-1700.3.19.1), SEAL 4.1. Re-scored 2026-10-24T23:37:14.

Speedup = baseline eval-only median / kernel eval-only median, both from the same neutral session (1 warmup + 10 measured processes of ./main --repeats 10, interleaved). The final kernel of each run is the strategy's own selection; re-scoring never re-selects. E2E = CHEHAB execution_time_(ms) (parse + encode + encrypt + fhe()). Compile time: median of 3, reported only.

## Every run

| task | strategy | run | final | speedup | eval (ms) | e2e (ms) | FHE err | depth | xdepth | levels | ct x ct | ct x pt | align | rescale | mod sw | C (ms) | correct / 8 | dup | failures | out tok | think tok | search (s) |
|---|---|---:|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---|---:|---:|---:|
| chebyshev_elu_d7 | archive | 1 | #3 (k1) | x1.090 | 188.9 | 217.0 | 2.69e-07 | 13 | 5 | 5 | 4 | 9 | 5 | 18 | 2 | 0.54 | 8 / 8 | 0 | - | 96244 | 87692 | 628.6 |
| chebyshev_elu_d7 | archive | 2 | #2 (k2) | x1.078 | 191.0 | 219.3 | 1.99e-07 | 13 | 5 | 5 | 4 | 9 | 6 | 19 | 3 | 0.55 | 6 / 8 | 1 | chehab 1 | 83440 | 74830 | 523.8 |
| chebyshev_elu_d7 | archive | 3 | #4 (k3) | x1.086 | 189.6 | 218.1 | 1.51e-07 | 16 | 5 | 5 | 4 | 9 | 5 | 18 | 2 | 0.57 | 6 / 8 | 0 | chehab 1, wrong 1 | 95429 | 86379 | 589.7 |
| chebyshev_elu_d7 | linear | 1 | #4 (k4) | x1.095 | 188.0 | 216.0 | 2.61e-07 | 14 | 5 | 5 | 4 | 9 | 5 | 18 | 2 | 0.55 | 6 / 8 | 2 | - | 52856 | 45160 | 390.2 |
| chebyshev_elu_d7 | linear | 2 | #1 (k5) | x1.075 | 191.5 | 219.3 | 1.87e-07 | 13 | 5 | 5 | 4 | 9 | 6 | 19 | 3 | 0.50 | 4 / 8 | 4 | - | 40787 | 33955 | 290.3 |
| chebyshev_elu_d7 | linear | 3 | #3 (k6) | x1.101 | 187.0 | 214.7 | 2.30e-07 | 13 | 5 | 5 | 4 | 9 | 5 | 18 | 2 | 0.55 | 6 / 8 | 2 | - | 51410 | 43787 | 371.8 |
| chebyshev_gelu_d7 | archive | 1 | #5 (k1) | x1.548 | 111.7 | 138.0 | 1.60e-07 | 18 | 5 | 5 | 3 | 3 | 1 | 7 | 4 | 0.56 | 6 / 8 | 0 | chehab 2 | 59417 | 53261 | 411.7 |
| chebyshev_gelu_d7 | archive | 2 | #3 (k2) | x1.411 | 122.6 | 148.6 | 1.73e-07 | 9 | 4 | 4 | 3 | 4 | 2 | 9 | 0 | 0.50 | 5 / 8 | 2 | chehab 1 | 74192 | 67226 | 445.6 |
| chebyshev_gelu_d7 | archive | 3 | #5 (k3) | x1.467 | 117.9 | 144.2 | 1.44e-07 | 15 | 5 | 5 | 3 | 4 | 2 | 9 | 4 | 0.48 | 6 / 8 | 1 | chehab 1 | 72890 | 67155 | 462.2 |
| chebyshev_gelu_d7 | linear | 1 | #4 (k4) | x1.371 | 126.1 | 152.8 | 1.77e-07 | 15 | 5 | 5 | 3 | 5 | 3 | 11 | 5 | 0.50 | 5 / 8 | 2 | chehab 1 | 33950 | 29423 | 272.1 |
| chebyshev_gelu_d7 | linear | 2 | #2 (k5) | x1.468 | 117.8 | 144.1 | 1.66e-07 | 15 | 5 | 5 | 3 | 4 | 2 | 9 | 4 | 0.51 | 4 / 8 | 4 | - | 50506 | 45719 | 334.4 |
| chebyshev_gelu_d7 | linear | 3 | #5 (k6) | x1.427 | 121.2 | 147.2 | 1.47e-07 | 10 | 4 | 4 | 3 | 4 | 2 | 9 | 0 | 0.50 | 3 / 8 | 5 | - | 61184 | 55738 | 369.4 |
| chebyshev_sigmoid_d7 | archive | 1 | #5 (k1) | x1.445 | 127.2 | 154.0 | 9.45e-08 | 17 | 6 | 6 | 4 | 3 | 1 | 8 | 6 | 0.52 | 5 / 8 | 2 | chehab 1 | 94314 | 88240 | 572.6 |
| chebyshev_sigmoid_d7 | archive | 2 | #8 (k2) | x1.184 | 155.2 | 182.1 | 9.80e-08 | 15 | 5 | 5 | 4 | 5 | 2 | 11 | 6 | 0.54 | 7 / 8 | 1 | - | 80339 | 74850 | 537.7 |
| chebyshev_sigmoid_d7 | archive | 3 | #6 (k3) | x1.226 | 150.0 | 176.6 | 8.87e-08 | 14 | 5 | 5 | 4 | 5 | 3 | 12 | 7 | 0.52 | 6 / 8 | 1 | chehab 1 | 110002 | 105011 | 638.3 |
| chebyshev_sigmoid_d7 | linear | 1 | #1 (k4) | x1.186 | 155.0 | 181.3 | 8.72e-08 | 16 | 6 | 6 | 4 | 5 | 6 | 15 | 8 | 0.54 | 4 / 8 | 4 | - | 41080 | 35457 | 286.1 |
| chebyshev_sigmoid_d7 | linear | 2 | #4 (k5) | x1.219 | 150.8 | 177.4 | 1.03e-07 | 13 | 5 | 5 | 4 | 5 | 3 | 12 | 7 | 0.52 | 5 / 8 | 3 | - | 52277 | 46058 | 360.8 |
| chebyshev_sigmoid_d7 | linear | 3 | #1 (k6) | x1.207 | 152.3 | 179.0 | 8.86e-08 | 16 | 6 | 6 | 4 | 5 | 5 | 14 | 7 | 0.55 | 3 / 8 | 5 | - | 37236 | 31262 | 255.3 |

## Per task and strategy (3 runs each)

| task | strategy | runs | speedup |
|---|---|---:|---|
| chebyshev_elu_d7 | archive | 3 | median x1.086, mean x1.085, min x1.078, max x1.090, sd 0.006 |
| chebyshev_gelu_d7 | archive | 3 | median x1.467, mean x1.475, min x1.411, max x1.548, sd 0.069 |
| chebyshev_sigmoid_d7 | archive | 3 | median x1.226, mean x1.285, min x1.184, max x1.445, sd 0.140 |
| chebyshev_elu_d7 | linear | 3 | median x1.095, mean x1.091, min x1.075, max x1.101, sd 0.014 |
| chebyshev_gelu_d7 | linear | 3 | median x1.427, mean x1.422, min x1.371, max x1.468, sd 0.049 |
| chebyshev_sigmoid_d7 | linear | 3 | median x1.207, mean x1.204, min x1.186, max x1.219, sd 0.017 |

## Across tasks (geometric mean of the per-task mean speedups)

| strategy | tasks | geometric mean |
|---|---:|---:|
| archive | 3 | x1.272 |
| linear | 3 | x1.231 |

## Approximation error (task property, separate from FHE error)

| task | max_fit_error (fitting grid) | approx. error, fixed points | approx. error, dense points |
|---|---:|---:|---:|
| chebyshev_elu_d7 | 0.0427493 | 0.0427493 | 0.0330832 |
| chebyshev_gelu_d7 | 0.0581538 | 0.0581538 | 0.0470933 |
| chebyshev_sigmoid_d7 | 0.002824 | 0.002824 | 0.0021268 |

## Re-scored kernels

| task | kernel | used by | correct | eval (ms) | speedup |
|---|---|---|---|---:|---:|
| chebyshev_elu_d7 | baseline | baseline | True | 205.88 | 1.0 |
| chebyshev_elu_d7 | k1 | archive/run1 | True | 188.86 | 1.09 |
| chebyshev_elu_d7 | k2 | archive/run2 | True | 190.96 | 1.078 |
| chebyshev_elu_d7 | k3 | archive/run3 | True | 189.56 | 1.086 |
| chebyshev_elu_d7 | k4 | linear/run1 | True | 187.97 | 1.095 |
| chebyshev_elu_d7 | k5 | linear/run2 | True | 191.47 | 1.075 |
| chebyshev_elu_d7 | k6 | linear/run3 | True | 186.96 | 1.101 |
| chebyshev_gelu_d7 | baseline | baseline | True | 172.95 | 1.0 |
| chebyshev_gelu_d7 | k1 | archive/run1 | True | 111.73 | 1.548 |
| chebyshev_gelu_d7 | k2 | archive/run2 | True | 122.61 | 1.411 |
| chebyshev_gelu_d7 | k3 | archive/run3 | True | 117.87 | 1.467 |
| chebyshev_gelu_d7 | k4 | linear/run1 | True | 126.13 | 1.371 |
| chebyshev_gelu_d7 | k5 | linear/run2 | True | 117.81 | 1.468 |
| chebyshev_gelu_d7 | k6 | linear/run3 | True | 121.19 | 1.427 |
| chebyshev_sigmoid_d7 | baseline | baseline | True | 183.84 | 1.0 |
| chebyshev_sigmoid_d7 | k1 | archive/run1 | True | 127.23 | 1.445 |
| chebyshev_sigmoid_d7 | k2 | archive/run2 | True | 155.21 | 1.184 |
| chebyshev_sigmoid_d7 | k3 | archive/run3 | True | 149.99 | 1.226 |
| chebyshev_sigmoid_d7 | k4 | linear/run1 | True | 155.0 | 1.186 |
| chebyshev_sigmoid_d7 | k5 | linear/run2 | True | 150.8 | 1.219 |
| chebyshev_sigmoid_d7 | k6 | linear/run3 | True | 152.28 | 1.207 |
