# Phase-2 pilot: neutral re-scoring

Commit af0dad4ecf (clean), Darwin arm64 arm, Apple clang version 17.0.0 (clang-1700.3.19.1), SEAL 4.1. Re-scored 2026-10-26T21:14:21.

Speedup = baseline eval-only median / kernel eval-only median, both from the same neutral session (1 warmup + 10 measured processes of ./main --repeats 10, interleaved). The final kernel of each run is the strategy's own selection; re-scoring never re-selects. E2E = CHEHAB execution_time_(ms) (parse + encode + encrypt + fhe()). Compile time: median of 3, reported only.

## Every run

| task | strategy | run | final | speedup | eval (ms) | e2e (ms) | FHE err | depth | xdepth | levels | ct x ct | ct x pt | align | rescale | mod sw | C (ms) | correct / 8 | dup | failures | out tok | think tok | search (s) |
|---|---|---:|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---|---:|---:|---:|
| chebyshev_elu_d7 | archive | 1 | #3 (k1) | x1.090 | 189.2 | 219.3 | 1.78e-07 | 13 | 5 | 5 | 4 | 9 | 5 | 18 | 2 | 0.63 | 8 / 8 | 0 | - | 96244 | 87692 | 628.6 |
| chebyshev_elu_d7 | archive | 2 | #2 (k2) | x1.078 | 191.4 | 221.5 | 2.23e-07 | 13 | 5 | 5 | 4 | 9 | 6 | 19 | 3 | 0.56 | 6 / 8 | 1 | chehab 1 | 83440 | 74830 | 523.8 |
| chebyshev_elu_d7 | archive | 3 | #4 (k3) | x1.086 | 189.9 | 219.6 | 1.47e-07 | 16 | 5 | 5 | 4 | 9 | 5 | 18 | 2 | 0.70 | 6 / 8 | 0 | chehab 1, wrong 1 | 95429 | 86379 | 589.7 |
| chebyshev_elu_d7 | archive | 4 | #7 (k4) | x1.092 | 188.9 | 220.2 | 1.80e-07 | 14 | 5 | 5 | 4 | 9 | 5 | 18 | 2 | 0.56 | 4 / 8 | 0 | chehab 4 | 75614 | 67902 | 472.1 |
| chebyshev_elu_d7 | archive | 5 | #5 (k5) | x1.094 | 188.6 | 217.7 | 1.96e-07 | 13 | 5 | 5 | 4 | 9 | 5 | 18 | 2 | 0.57 | 6 / 8 | 0 | chehab 2 | 80758 | 73013 | 512.9 |
| chebyshev_elu_d7 | linear | 1 | #4 (k6) | x1.095 | 188.5 | 217.1 | 1.72e-07 | 14 | 5 | 5 | 4 | 9 | 5 | 18 | 2 | 0.55 | 6 / 8 | 2 | - | 52856 | 45160 | 390.2 |
| chebyshev_elu_d7 | linear | 2 | #1 (k7) | x1.075 | 191.9 | 222.4 | 1.79e-07 | 13 | 5 | 5 | 4 | 9 | 6 | 19 | 3 | 0.80 | 4 / 8 | 4 | - | 40787 | 33955 | 290.3 |
| chebyshev_elu_d7 | linear | 3 | #3 (k8) | x1.102 | 187.3 | 216.4 | 1.60e-07 | 13 | 5 | 5 | 4 | 9 | 5 | 18 | 2 | 0.56 | 6 / 8 | 2 | - | 51410 | 43787 | 371.8 |
| chebyshev_elu_d7 | linear | 4 | #6 (k9) | x1.076 | 191.7 | 221.7 | 2.05e-07 | 14 | 5 | 5 | 4 | 9 | 6 | 19 | 3 | 0.56 | 6 / 8 | 1 | chehab 1 | 54137 | 45856 | 414.1 |
| chebyshev_elu_d7 | linear | 5 | #1 (k10) | x1.075 | 191.9 | 222.3 | 2.25e-07 | 13 | 5 | 5 | 4 | 9 | 6 | 19 | 3 | 0.58 | 3 / 8 | 5 | - | 56913 | 50339 | 348.9 |
| chebyshev_gelu_d7 | archive | 1 | #5 (k1) | x1.548 | 111.9 | 140.3 | 1.96e-07 | 18 | 5 | 5 | 3 | 3 | 1 | 7 | 4 | 0.49 | 6 / 8 | 0 | chehab 2 | 59417 | 53261 | 411.7 |
| chebyshev_gelu_d7 | archive | 2 | #3 (k2) | x1.410 | 122.9 | 152.4 | 1.78e-07 | 9 | 4 | 4 | 3 | 4 | 2 | 9 | 0 | 0.48 | 5 / 8 | 2 | chehab 1 | 74192 | 67226 | 445.6 |
| chebyshev_gelu_d7 | archive | 3 | #5 (k3) | x1.466 | 118.1 | 146.6 | 1.63e-07 | 15 | 5 | 5 | 3 | 4 | 2 | 9 | 4 | 0.49 | 6 / 8 | 1 | chehab 1 | 72890 | 67155 | 462.2 |
| chebyshev_gelu_d7 | archive | 4 | #7 (k4) | x1.554 | 111.5 | 140.4 | 2.30e-07 | 15 | 5 | 5 | 3 | 3 | 1 | 7 | 4 | 0.49 | 7 / 8 | 0 | chehab 1 | 73001 | 66218 | 485.2 |
| chebyshev_gelu_d7 | archive | 5 | #4 (k5) | x1.413 | 122.6 | 151.6 | 1.89e-07 | 11 | 4 | 4 | 3 | 4 | 2 | 9 | 1 | 0.47 | 6 / 8 | 0 | chehab 1, wrong 1 | 54817 | 49194 | 382.6 |
| chebyshev_gelu_d7 | linear | 1 | #4 (k6) | x1.371 | 126.3 | 156.6 | 2.44e-07 | 15 | 5 | 5 | 3 | 5 | 3 | 11 | 5 | 0.49 | 5 / 8 | 2 | chehab 1 | 33950 | 29423 | 272.1 |
| chebyshev_gelu_d7 | linear | 2 | #2 (k7) | x1.466 | 118.1 | 146.6 | 1.91e-07 | 15 | 5 | 5 | 3 | 4 | 2 | 9 | 4 | 0.50 | 4 / 8 | 4 | - | 50506 | 45719 | 334.4 |
| chebyshev_gelu_d7 | linear | 3 | #5 (k8) | x1.424 | 121.6 | 150.0 | 1.46e-07 | 10 | 4 | 4 | 3 | 4 | 2 | 9 | 0 | 0.48 | 3 / 8 | 5 | - | 61184 | 55738 | 369.4 |
| chebyshev_gelu_d7 | linear | 4 | #2 (k9) | x1.415 | 122.4 | 151.5 | 1.84e-07 | 10 | 4 | 4 | 3 | 4 | 2 | 9 | 0 | 0.49 | 3 / 8 | 5 | - | 41768 | 36822 | 293.8 |
| chebyshev_gelu_d7 | linear | 5 | #3 (k10) | x1.410 | 122.8 | 151.6 | 1.44e-07 | 10 | 4 | 4 | 3 | 4 | 2 | 9 | 1 | 0.48 | 4 / 8 | 4 | - | 36945 | 30679 | 269.2 |
| chebyshev_sigmoid_d7 | archive | 1 | #5 (k1) | x1.444 | 127.9 | 156.3 | 7.93e-08 | 17 | 6 | 6 | 4 | 3 | 1 | 8 | 6 | 0.53 | 5 / 8 | 2 | chehab 1 | 94314 | 88240 | 572.6 |
| chebyshev_sigmoid_d7 | archive | 2 | #8 (k2) | x1.184 | 156.0 | 185.0 | 8.49e-08 | 15 | 5 | 5 | 4 | 5 | 2 | 11 | 6 | 0.56 | 7 / 8 | 1 | - | 80339 | 74850 | 537.7 |
| chebyshev_sigmoid_d7 | archive | 3 | #6 (k3) | x1.224 | 150.9 | 179.3 | 9.25e-08 | 14 | 5 | 5 | 4 | 5 | 3 | 12 | 7 | 0.56 | 6 / 8 | 1 | chehab 1 | 110002 | 105011 | 638.3 |
| chebyshev_sigmoid_d7 | archive | 4 | #7 (k4) | x1.303 | 141.8 | 170.0 | 6.60e-08 | 15 | 5 | 5 | 4 | 4 | 1 | 9 | 3 | 0.55 | 6 / 8 | 1 | chehab 1 | 114795 | 108260 | 1000.9 |
| chebyshev_sigmoid_d7 | archive | 5 | #8 (k5) | x1.317 | 140.2 | 169.1 | 8.76e-08 | 13 | 5 | 5 | 4 | 3 | 2 | 9 | 3 | 0.61 | 7 / 8 | 0 | chehab 1 | 107298 | 101530 | 658.5 |
| chebyshev_sigmoid_d7 | linear | 1 | #1 (k6) | x1.183 | 156.1 | 184.4 | 8.12e-08 | 16 | 6 | 6 | 4 | 5 | 6 | 15 | 8 | 0.63 | 4 / 8 | 4 | - | 41080 | 35457 | 286.1 |
| chebyshev_sigmoid_d7 | linear | 2 | #4 (k7) | x1.219 | 151.5 | 180.8 | 8.31e-08 | 13 | 5 | 5 | 4 | 5 | 3 | 12 | 7 | 0.57 | 5 / 8 | 3 | - | 52277 | 46058 | 360.8 |
| chebyshev_sigmoid_d7 | linear | 3 | #1 (k8) | x1.207 | 153.0 | 182.2 | 9.38e-08 | 16 | 6 | 6 | 4 | 5 | 5 | 14 | 7 | 0.60 | 3 / 8 | 5 | - | 37236 | 31262 | 255.3 |
| chebyshev_sigmoid_d7 | linear | 4 | #1 (k9) | x1.186 | 155.7 | 183.9 | 9.09e-08 | 16 | 6 | 6 | 4 | 5 | 6 | 15 | 8 | 0.57 | 5 / 8 | 3 | - | 54229 | 48846 | 374.6 |
| chebyshev_sigmoid_d7 | linear | 5 | #2 (k10) | x1.319 | 140.0 | 172.2 | 5.51e-08 | 17 | 6 | 6 | 4 | 4 | 3 | 11 | 6 | 0.55 | 3 / 8 | 5 | - | 103835 | 98329 | 563.4 |

## Per task and strategy (3 runs each)

| task | strategy | runs | speedup |
|---|---|---:|---|
| chebyshev_elu_d7 | archive | 5 | median x1.090, mean x1.088, min x1.078, max x1.094, sd 0.006 |
| chebyshev_gelu_d7 | archive | 5 | median x1.466, mean x1.478, min x1.410, max x1.554, sd 0.070 |
| chebyshev_sigmoid_d7 | archive | 5 | median x1.303, mean x1.294, min x1.184, max x1.444, sd 0.100 |
| chebyshev_elu_d7 | linear | 5 | median x1.076, mean x1.085, min x1.075, max x1.102, sd 0.013 |
| chebyshev_gelu_d7 | linear | 5 | median x1.415, mean x1.417, min x1.371, max x1.466, sd 0.034 |
| chebyshev_sigmoid_d7 | linear | 5 | median x1.207, mean x1.223, min x1.183, max x1.319, sd 0.056 |

## Across tasks (geometric mean of the per-task mean speedups)

| strategy | tasks | geometric mean |
|---|---:|---:|
| archive | 3 | x1.277 |
| linear | 3 | x1.234 |

## Approximation error (task property, separate from FHE error)

| task | max_fit_error (fitting grid) | approx. error, fixed points | approx. error, dense points |
|---|---:|---:|---:|
| chebyshev_elu_d7 | 0.0427493 | 0.0427493 | 0.0330832 |
| chebyshev_gelu_d7 | 0.0581538 | 0.0581538 | 0.0470933 |
| chebyshev_sigmoid_d7 | 0.002824 | 0.002824 | 0.0021268 |

## Re-scored kernels

| task | kernel | used by | correct | eval (ms) | speedup |
|---|---|---|---|---:|---:|
| chebyshev_gelu_d7 | baseline | baseline | True | 173.21 | 1.0 |
| chebyshev_gelu_d7 | k1 | archive/run1 | True | 111.92 | 1.548 |
| chebyshev_gelu_d7 | k2 | archive/run2 | True | 122.88 | 1.41 |
| chebyshev_gelu_d7 | k3 | archive/run3 | True | 118.13 | 1.466 |
| chebyshev_gelu_d7 | k4 | archive/run4 | True | 111.46 | 1.554 |
| chebyshev_gelu_d7 | k5 | archive/run5 | True | 122.56 | 1.413 |
| chebyshev_gelu_d7 | k6 | linear/run1 | True | 126.33 | 1.371 |
| chebyshev_gelu_d7 | k7 | linear/run2 | True | 118.11 | 1.466 |
| chebyshev_gelu_d7 | k8 | linear/run3 | True | 121.63 | 1.424 |
| chebyshev_gelu_d7 | k9 | linear/run4 | True | 122.42 | 1.415 |
| chebyshev_gelu_d7 | k10 | linear/run5 | True | 122.83 | 1.41 |
| chebyshev_sigmoid_d7 | baseline | baseline | True | 184.71 | 1.0 |
| chebyshev_sigmoid_d7 | k1 | archive/run1 | True | 127.94 | 1.444 |
| chebyshev_sigmoid_d7 | k2 | archive/run2 | True | 156.0 | 1.184 |
| chebyshev_sigmoid_d7 | k3 | archive/run3 | True | 150.86 | 1.224 |
| chebyshev_sigmoid_d7 | k4 | archive/run4 | True | 141.79 | 1.303 |
| chebyshev_sigmoid_d7 | k5 | archive/run5 | True | 140.25 | 1.317 |
| chebyshev_sigmoid_d7 | k6 | linear/run1 | True | 156.1 | 1.183 |
| chebyshev_sigmoid_d7 | k7 | linear/run2 | True | 151.5 | 1.219 |
| chebyshev_sigmoid_d7 | k8 | linear/run3 | True | 152.99 | 1.207 |
| chebyshev_sigmoid_d7 | k9 | linear/run4 | True | 155.73 | 1.186 |
| chebyshev_sigmoid_d7 | k10 | linear/run5 | True | 139.99 | 1.319 |
| chebyshev_elu_d7 | baseline | baseline | True | 206.34 | 1.0 |
| chebyshev_elu_d7 | k1 | archive/run1 | True | 189.24 | 1.09 |
| chebyshev_elu_d7 | k2 | archive/run2 | True | 191.35 | 1.078 |
| chebyshev_elu_d7 | k3 | archive/run3 | True | 189.95 | 1.086 |
| chebyshev_elu_d7 | k4 | archive/run4 | True | 188.89 | 1.092 |
| chebyshev_elu_d7 | k5 | archive/run5 | True | 188.63 | 1.094 |
| chebyshev_elu_d7 | k6 | linear/run1 | True | 188.45 | 1.095 |
| chebyshev_elu_d7 | k7 | linear/run2 | True | 191.92 | 1.075 |
| chebyshev_elu_d7 | k8 | linear/run3 | True | 187.31 | 1.102 |
| chebyshev_elu_d7 | k9 | linear/run4 | True | 191.7 | 1.076 |
| chebyshev_elu_d7 | k10 | linear/run5 | True | 191.89 | 1.075 |
