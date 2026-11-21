# Phase-2 pilot: neutral re-scoring

Commit 1d44c9e562 (dirty), Darwin arm64 arm, Apple clang version 21.0.0 (clang-2100.1.1.101), SEAL 4.1. Re-scored 2026-11-15T16:08:25.

Speedup = baseline eval-only median / kernel eval-only median, both from the same neutral session (1 warmup + 10 measured processes of ./main --repeats 10, interleaved). The final kernel of each run is the strategy's own selection; re-scoring never re-selects. E2E = CHEHAB execution_time_(ms) (parse + encode + encrypt + fhe()). Compile time: median of 3, reported only.

## Every run

| task | strategy | run | final | speedup | eval (ms) | e2e (ms) | FHE err | depth | xdepth | levels | ct x ct | ct x pt | align | rescale | mod sw | C (ms) | correct / 8 | dup | failures | out tok | think tok | search (s) |
|---|---|---:|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---|---:|---:|---:|
| chebyshev_elu_d7 | archive | 1 | #5 (k1) | x1.103 | 184.1 | 211.7 | 1.99e-07 | 14 | 5 | 5 | 4 | 9 | 5 | 18 | 2 | 0.52 | 6 / 8 | 1 | chehab 1 | 104357 | 97350 | 688.1 |
| chebyshev_elu_d7 | archive | 2 | #1 (k2) | x1.076 | 188.6 | 218.1 | 1.83e-07 | 14 | 5 | 5 | 4 | 9 | 6 | 19 | 3 | 0.53 | 7 / 8 | 1 | - | 79254 | 71309 | 621.1 |
| chebyshev_elu_d7 | archive | 3 | #0 (baseline) | x1.000 | 203.0 | 231.3 | 1.65e-07 | 22 | 8 | 8 | 6 | 8 | 11 | 25 | 10 | 0.55 | 0 / 8 | 1 | chehab 7 | 54182 | 46749 | 261.9 |
| chebyshev_elu_d7 | independent | 1 | #1 (k3) | x1.090 | 186.1 | 214.5 | 1.61e-07 | 13 | 5 | 5 | 4 | 9 | 5 | 18 | 2 | 0.53 | 5 / 8 | 0 | chehab 3 | 86774 | 79283 | 579.1 |
| chebyshev_elu_d7 | independent | 2 | #3 (k4) | x1.096 | 185.2 | 213.2 | 1.64e-07 | 13 | 5 | 5 | 4 | 9 | 5 | 18 | 2 | 0.52 | 6 / 8 | 0 | chehab 1, wrong 1 | 64901 | 58024 | 513.1 |
| chebyshev_elu_d7 | independent | 3 | #3 (k5) | x1.074 | 188.9 | 217.0 | 1.91e-07 | 14 | 5 | 5 | 4 | 9 | 6 | 19 | 4 | 0.65 | 5 / 8 | 0 | chehab 2, wrong 1 | 76618 | 69042 | 547.3 |
| chebyshev_elu_d7 | linear | 1 | #1 (k6) | x1.080 | 187.9 | 216.5 | 1.81e-07 | 14 | 5 | 5 | 4 | 9 | 6 | 19 | 3 | 0.56 | 4 / 8 | 4 | - | 42849 | 36018 | 346.4 |
| chebyshev_elu_d7 | linear | 2 | #3 (k7) | x1.092 | 185.9 | 214.6 | 1.88e-07 | 13 | 5 | 5 | 4 | 9 | 5 | 18 | 2 | 0.50 | 6 / 8 | 1 | chehab 1 | 67127 | 58629 | 526.1 |
| chebyshev_elu_d7 | linear | 3 | #4 (k8) | x1.097 | 185.1 | 213.2 | 1.73e-07 | 14 | 5 | 5 | 4 | 9 | 5 | 18 | 2 | 0.49 | 6 / 8 | 1 | wrong 1 | 52951 | 45934 | 464.2 |
| chebyshev_gelu_d7 | archive | 1 | #2 (k1) | x1.411 | 122.0 | 148.9 | 2.64e-07 | 9 | 4 | 4 | 3 | 4 | 2 | 9 | 0 | 0.42 | 8 / 8 | 0 | - | 80003 | 74759 | 601.0 |
| chebyshev_gelu_d7 | archive | 2 | #8 (k2) | x1.432 | 120.2 | 148.8 | 1.96e-07 | 10 | 4 | 4 | 3 | 4 | 2 | 9 | 0 | 0.38 | 6 / 8 | 1 | chehab 1 | 55971 | 50386 | 451.6 |
| chebyshev_gelu_d7 | archive | 3 | #4 (k3) | x1.425 | 120.8 | 150.2 | 2.16e-07 | 10 | 4 | 4 | 3 | 4 | 2 | 9 | 0 | 0.40 | 7 / 8 | 1 | - | 59765 | 53759 | 479.2 |
| chebyshev_gelu_d7 | independent | 1 | #7 (k4) | x1.482 | 116.2 | 144.3 | 1.70e-07 | 15 | 5 | 5 | 3 | 4 | 2 | 9 | 4 | 0.41 | 6 / 8 | 0 | - | 34670 | 29408 | 347.6 |
| chebyshev_gelu_d7 | independent | 2 | #2 (k5) | x1.387 | 124.1 | 151.5 | 1.67e-07 | 12 | 4 | 4 | 3 | 4 | 3 | 10 | 1 | 0.44 | 6 / 8 | 0 | chehab 1 | 43097 | 37884 | 380.9 |
| chebyshev_gelu_d7 | independent | 3 | #6 (k6) | x1.433 | 120.1 | 146.7 | 1.87e-07 | 10 | 4 | 4 | 3 | 4 | 2 | 9 | 0 | 0.40 | 6 / 8 | 0 | chehab 2 | 48635 | 43497 | 407.5 |
| chebyshev_gelu_d7 | linear | 1 | #2 (k7) | x1.473 | 116.8 | 145.5 | 2.04e-07 | 15 | 5 | 5 | 3 | 4 | 2 | 9 | 4 | 0.51 | 5 / 8 | 3 | - | 39142 | 33558 | 326.5 |
| chebyshev_gelu_d7 | linear | 2 | #2 (k8) | x1.416 | 121.5 | 149.3 | 2.74e-07 | 11 | 4 | 4 | 3 | 4 | 2 | 9 | 1 | 0.39 | 3 / 8 | 5 | - | 46723 | 41786 | 324.0 |
| chebyshev_gelu_d7 | linear | 3 | #7 (k9) | x1.360 | 126.5 | 155.5 | 2.04e-07 | 15 | 5 | 5 | 3 | 4 | 4 | 11 | 4 | 0.51 | 6 / 8 | 1 | chehab 1 | 65102 | 57456 | 487.3 |
| chebyshev_sigmoid_d7 | archive | 1 | #7 (k1) | x1.222 | 147.6 | 174.5 | 9.28e-08 | 13 | 5 | 5 | 4 | 5 | 3 | 12 | 6 | 0.59 | 6 / 8 | 1 | chehab 1 | 114693 | 107538 | 722.2 |
| chebyshev_sigmoid_d7 | archive | 2 | #5 (k2) | x1.592 | 113.3 | 140.1 | 9.11e-08 | 17 | 6 | 6 | 4 | 2 | 0 | 6 | 7 | 0.44 | 6 / 8 | 0 | - | 128600 | 122668 | 786.5 |
| chebyshev_sigmoid_d7 | archive | 3 | #4 (k3) | x1.357 | 133.0 | 160.3 | 8.26e-08 | 13 | 5 | 5 | 4 | 3 | 1 | 8 | 3 | 0.40 | 5 / 8 | 3 | - | 138346 | 132965 | 806.9 |
| chebyshev_sigmoid_d7 | independent | 1 | #2 (k4) | x1.207 | 149.5 | 175.8 | 8.46e-08 | 16 | 6 | 6 | 4 | 5 | 5 | 14 | 7 | 0.44 | 6 / 8 | 0 | - | 75153 | 68919 | 545.5 |
| chebyshev_sigmoid_d7 | independent | 2 | #2 (k5) | x1.207 | 149.4 | 176.0 | 9.86e-08 | 16 | 6 | 6 | 4 | 5 | 5 | 14 | 7 | 0.49 | 5 / 8 | 0 | chehab 2 | 53596 | 48013 | 405.6 |
| chebyshev_sigmoid_d7 | independent | 3 | #2 (k6) | x1.186 | 152.2 | 179.6 | 8.58e-08 | 16 | 6 | 6 | 4 | 5 | 6 | 15 | 8 | 0.48 | 6 / 8 | 1 | chehab 1 | 79966 | 73471 | 571.7 |
| chebyshev_sigmoid_d7 | linear | 1 | #1 (k7) | x1.185 | 152.3 | 178.9 | 7.09e-08 | 16 | 6 | 6 | 4 | 5 | 6 | 15 | 8 | 0.46 | 2 / 8 | 6 | - | 33038 | 28253 | 224.3 |
| chebyshev_sigmoid_d7 | linear | 2 | #3 (k8) | x1.225 | 147.3 | 175.6 | 5.74e-08 | 17 | 6 | 6 | 4 | 4 | 5 | 13 | 6 | 0.47 | 5 / 8 | 2 | wrong 1 | 53892 | 47196 | 412.9 |
| chebyshev_sigmoid_d7 | linear | 3 | #2 (k9) | x1.078 | 167.3 | 194.7 | 1.80e-07 | 13 | 5 | 5 | 4 | 7 | 4 | 15 | 6 | 0.45 | 4 / 8 | 4 | - | 52286 | 47038 | 384.8 |

## Per task and strategy (3 runs each)

| task | strategy | runs | speedup |
|---|---|---:|---|
| chebyshev_elu_d7 | archive | 3 | median x1.076, mean x1.060, min x1.000, max x1.103, sd 0.053 |
| chebyshev_gelu_d7 | archive | 3 | median x1.425, mean x1.423, min x1.411, max x1.432, sd 0.011 |
| chebyshev_sigmoid_d7 | archive | 3 | median x1.357, mean x1.390, min x1.222, max x1.592, sd 0.187 |
| chebyshev_elu_d7 | independent | 3 | median x1.090, mean x1.087, min x1.074, max x1.096, sd 0.011 |
| chebyshev_gelu_d7 | independent | 3 | median x1.433, mean x1.434, min x1.387, max x1.482, sd 0.047 |
| chebyshev_sigmoid_d7 | independent | 3 | median x1.207, mean x1.200, min x1.186, max x1.207, sd 0.012 |
| chebyshev_elu_d7 | linear | 3 | median x1.092, mean x1.089, min x1.080, max x1.097, sd 0.008 |
| chebyshev_gelu_d7 | linear | 3 | median x1.416, mean x1.417, min x1.360, max x1.473, sd 0.056 |
| chebyshev_sigmoid_d7 | linear | 3 | median x1.185, mean x1.162, min x1.078, max x1.225, sd 0.076 |

## Across tasks (geometric mean of the per-task mean speedups)

| strategy | tasks | geometric mean |
|---|---:|---:|
| archive | 3 | x1.280 |
| independent | 3 | x1.232 |
| linear | 3 | x1.215 |

## Approximation error (task property, separate from FHE error)

| task | max_fit_error (fitting grid) | approx. error, fixed points | approx. error, dense points |
|---|---:|---:|---:|
| chebyshev_elu_d7 | 0.0427493 | 0.0427493 | 0.0330832 |
| chebyshev_gelu_d7 | 0.0581538 | 0.0581538 | 0.0470933 |
| chebyshev_sigmoid_d7 | 0.002824 | 0.002824 | 0.0021268 |

## Re-scored kernels

| task | kernel | used by | correct | eval (ms) | speedup |
|---|---|---|---|---:|---:|
| chebyshev_elu_d7 | baseline | archive/run3 | True | 202.95 | 1.0 |
| chebyshev_elu_d7 | k1 | archive/run1 | True | 184.05 | 1.103 |
| chebyshev_elu_d7 | k2 | archive/run2 | True | 188.56 | 1.076 |
| chebyshev_elu_d7 | k3 | independent/run1 | True | 186.12 | 1.09 |
| chebyshev_elu_d7 | k4 | independent/run2 | True | 185.16 | 1.096 |
| chebyshev_elu_d7 | k5 | independent/run3 | True | 188.94 | 1.074 |
| chebyshev_elu_d7 | k6 | linear/run1 | True | 187.88 | 1.08 |
| chebyshev_elu_d7 | k7 | linear/run2 | True | 185.92 | 1.092 |
| chebyshev_elu_d7 | k8 | linear/run3 | True | 185.07 | 1.097 |
| chebyshev_gelu_d7 | baseline | baseline | True | 172.13 | 1.0 |
| chebyshev_gelu_d7 | k1 | archive/run1 | True | 121.96 | 1.411 |
| chebyshev_gelu_d7 | k2 | archive/run2 | True | 120.16 | 1.432 |
| chebyshev_gelu_d7 | k3 | archive/run3 | True | 120.78 | 1.425 |
| chebyshev_gelu_d7 | k4 | independent/run1 | True | 116.18 | 1.482 |
| chebyshev_gelu_d7 | k5 | independent/run2 | True | 124.13 | 1.387 |
| chebyshev_gelu_d7 | k6 | independent/run3 | True | 120.09 | 1.433 |
| chebyshev_gelu_d7 | k7 | linear/run1 | True | 116.85 | 1.473 |
| chebyshev_gelu_d7 | k8 | linear/run2 | True | 121.52 | 1.416 |
| chebyshev_gelu_d7 | k9 | linear/run3 | True | 126.52 | 1.36 |
| chebyshev_sigmoid_d7 | baseline | baseline | True | 180.4 | 1.0 |
| chebyshev_sigmoid_d7 | k1 | archive/run1 | True | 147.62 | 1.222 |
| chebyshev_sigmoid_d7 | k2 | archive/run2 | True | 113.29 | 1.592 |
| chebyshev_sigmoid_d7 | k3 | archive/run3 | True | 132.97 | 1.357 |
| chebyshev_sigmoid_d7 | k4 | independent/run1 | True | 149.49 | 1.207 |
| chebyshev_sigmoid_d7 | k5 | independent/run2 | True | 149.43 | 1.207 |
| chebyshev_sigmoid_d7 | k6 | independent/run3 | True | 152.16 | 1.186 |
| chebyshev_sigmoid_d7 | k7 | linear/run1 | True | 152.26 | 1.185 |
| chebyshev_sigmoid_d7 | k8 | linear/run2 | True | 147.32 | 1.225 |
| chebyshev_sigmoid_d7 | k9 | linear/run3 | True | 167.34 | 1.078 |
