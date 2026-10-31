# Phase-2 pilot: neutral re-scoring

Commit af0dad4ecf (clean), Darwin arm64 arm, Apple clang version 17.0.0 (clang-1700.3.19.1), SEAL 4.1. Re-scored 2026-10-27T11:53:05.

Speedup = baseline eval-only median / kernel eval-only median, both from the same neutral session (1 warmup + 10 measured processes of ./main --repeats 10, interleaved). The final kernel of each run is the strategy's own selection; re-scoring never re-selects. E2E = CHEHAB execution_time_(ms) (parse + encode + encrypt + fhe()). Compile time: median of 3, reported only.

## Every run

| task | strategy | run | final | speedup | eval (ms) | e2e (ms) | FHE err | depth | xdepth | levels | ct x ct | ct x pt | align | rescale | mod sw | C (ms) | correct / 8 | dup | failures | out tok | think tok | search (s) |
|---|---|---:|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---|---:|---:|---:|
| chebyshev_elu_d7 | archive | 1 | #2 (k1) | x1.068 | 194.9 | 291.2 | 1.96e-07 | 14 | 5 | 5 | 4 | 9 | 5 | 18 | 2 | 0.53 | 8 / 8 | 0 | - | 99661 | 90932 | 657.8 |
| chebyshev_elu_d7 | archive | 2 | #5 (k2) | x1.102 | 189.0 | 220.6 | 1.80e-07 | 13 | 5 | 5 | 4 | 9 | 5 | 18 | 2 | 0.54 | 6 / 8 | 1 | chehab 1 | 83116 | 75763 | 529.6 |
| chebyshev_elu_d7 | archive | 3 | #2 (k3) | x1.099 | 189.5 | 217.7 | 1.75e-07 | 13 | 5 | 5 | 4 | 9 | 5 | 18 | 2 | 0.58 | 7 / 8 | 1 | - | 90993 | 82267 | 585.6 |
| chebyshev_elu_d7 | linear | 1 | #6 (k4) | x1.079 | 193.0 | 232.0 | 1.87e-07 | 13 | 5 | 5 | 4 | 9 | 5 | 18 | 2 | 0.55 | 5 / 8 | 3 | - | 49541 | 41744 | 354.4 |
| chebyshev_elu_d7 | linear | 2 | #4 (k5) | x1.093 | 190.5 | 223.4 | 1.41e-07 | 13 | 5 | 5 | 4 | 9 | 5 | 18 | 2 | 0.61 | 4 / 8 | 4 | - | 50764 | 42444 | 358.2 |
| chebyshev_elu_d7 | linear | 3 | #3 (k6) | x1.106 | 188.2 | 222.3 | 2.41e-07 | 13 | 5 | 5 | 4 | 9 | 5 | 18 | 2 | 0.66 | 6 / 8 | 2 | - | 43598 | 35147 | 338.0 |
| chebyshev_gelu_d7 | archive | 1 | #2 (k1) | x1.421 | 127.8 | 157.3 | 1.96e-07 | 11 | 4 | 4 | 3 | 4 | 2 | 9 | 1 | 0.53 | 7 / 8 | 1 | - | 62256 | 56660 | 423.7 |
| chebyshev_gelu_d7 | archive | 2 | #5 (k2) | x1.423 | 127.7 | 157.8 | 1.89e-07 | 9 | 4 | 4 | 3 | 4 | 2 | 9 | 0 | 0.53 | 7 / 8 | 1 | - | 51880 | 45796 | 384.0 |
| chebyshev_gelu_d7 | archive | 3 | #2 (k3) | x1.478 | 122.9 | 152.0 | 1.91e-07 | 14 | 5 | 5 | 3 | 4 | 2 | 9 | 3 | 0.86 | 7 / 8 | 0 | chehab 1 | 40551 | 34783 | 338.1 |
| chebyshev_gelu_d7 | linear | 1 | #3 (k4) | x1.414 | 128.4 | 159.5 | 1.82e-07 | 9 | 4 | 4 | 3 | 4 | 2 | 9 | 0 | 0.55 | 6 / 8 | 2 | - | 37138 | 31490 | 312.7 |
| chebyshev_gelu_d7 | linear | 2 | #2 (k5) | x1.417 | 128.1 | 157.5 | 1.67e-07 | 11 | 4 | 4 | 3 | 4 | 2 | 9 | 1 | 0.50 | 5 / 8 | 3 | - | 29465 | 24493 | 248.8 |
| chebyshev_gelu_d7 | linear | 3 | #2 (k6) | x1.464 | 124.0 | 153.5 | 1.60e-07 | 15 | 5 | 5 | 3 | 4 | 2 | 9 | 4 | 0.62 | 5 / 8 | 2 | chehab 1 | 58050 | 53094 | 375.3 |
| chebyshev_sigmoid_d7 | archive | 1 | #6 (k1) | x1.352 | 140.9 | 167.9 | 1.27e-07 | 14 | 5 | 5 | 4 | 3 | 1 | 8 | 3 | 0.57 | 7 / 8 | 1 | - | 129941 | 124152 | 751.4 |
| chebyshev_sigmoid_d7 | archive | 2 | #5 (k2) | x1.192 | 159.9 | 187.8 | 1.06e-07 | 14 | 5 | 5 | 4 | 5 | 4 | 13 | 8 | 0.63 | 6 / 8 | 2 | - | 97743 | 92703 | 592.3 |
| chebyshev_sigmoid_d7 | archive | 3 | #5 (k3) | x1.226 | 155.4 | 184.5 | 7.44e-08 | 12 | 5 | 5 | 4 | 5 | 3 | 12 | 6 | 0.68 | 8 / 8 | 0 | - | 90925 | 84898 | 597.2 |
| chebyshev_sigmoid_d7 | linear | 1 | #0 (baseline) | x1.000 | 190.6 | 221.4 | 7.57e-08 | 22 | 8 | 8 | 6 | 5 | 8 | 19 | 13 | 0.66 | 4 / 8 | 1 | chehab 3 | 84941 | 78106 | 510.4 |
| chebyshev_sigmoid_d7 | linear | 2 | #3 (k4) | x1.219 | 156.3 | 188.8 | 5.66e-08 | 21 | 6 | 6 | 4 | 4 | 5 | 13 | 6 | 0.67 | 3 / 8 | 4 | chehab 1 | 57202 | 50048 | 359.2 |
| chebyshev_sigmoid_d7 | linear | 3 | #7 (k5) | x1.583 | 120.4 | 154.3 | 7.47e-08 | 17 | 6 | 6 | 4 | 2 | 2 | 8 | 8 | 0.57 | 6 / 8 | 1 | wrong 1 | 76644 | 69588 | 471.3 |

## Per task and strategy (3 runs each)

| task | strategy | runs | speedup |
|---|---|---:|---|
| chebyshev_elu_d7 | archive | 3 | median x1.099, mean x1.090, min x1.068, max x1.102, sd 0.019 |
| chebyshev_gelu_d7 | archive | 3 | median x1.423, mean x1.441, min x1.421, max x1.478, sd 0.033 |
| chebyshev_sigmoid_d7 | archive | 3 | median x1.226, mean x1.257, min x1.192, max x1.352, sd 0.084 |
| chebyshev_elu_d7 | linear | 3 | median x1.093, mean x1.093, min x1.079, max x1.106, sd 0.014 |
| chebyshev_gelu_d7 | linear | 3 | median x1.417, mean x1.432, min x1.414, max x1.464, sd 0.028 |
| chebyshev_sigmoid_d7 | linear | 3 | median x1.219, mean x1.267, min x1.000, max x1.583, sd 0.294 |

## Across tasks (geometric mean of the per-task mean speedups)

| strategy | tasks | geometric mean |
|---|---:|---:|
| archive | 3 | x1.254 |
| linear | 3 | x1.256 |

## Approximation error (task property, separate from FHE error)

| task | max_fit_error (fitting grid) | approx. error, fixed points | approx. error, dense points |
|---|---:|---:|---:|
| chebyshev_elu_d7 | 0.0427493 | 0.0427493 | 0.0330832 |
| chebyshev_gelu_d7 | 0.0581538 | 0.0581538 | 0.0470933 |
| chebyshev_sigmoid_d7 | 0.002824 | 0.002824 | 0.0021268 |

## Re-scored kernels

| task | kernel | used by | correct | eval (ms) | speedup |
|---|---|---|---|---:|---:|
| chebyshev_gelu_d7 | baseline | baseline | True | 181.64 | 1.0 |
| chebyshev_gelu_d7 | k1 | archive/run1 | True | 127.79 | 1.421 |
| chebyshev_gelu_d7 | k2 | archive/run2 | True | 127.67 | 1.423 |
| chebyshev_gelu_d7 | k3 | archive/run3 | True | 122.85 | 1.478 |
| chebyshev_gelu_d7 | k4 | linear/run1 | True | 128.41 | 1.414 |
| chebyshev_gelu_d7 | k5 | linear/run2 | True | 128.14 | 1.417 |
| chebyshev_gelu_d7 | k6 | linear/run3 | True | 124.05 | 1.464 |
| chebyshev_sigmoid_d7 | baseline | linear/run1 | True | 190.56 | 1.0 |
| chebyshev_sigmoid_d7 | k1 | archive/run1 | True | 140.94 | 1.352 |
| chebyshev_sigmoid_d7 | k2 | archive/run2 | True | 159.89 | 1.192 |
| chebyshev_sigmoid_d7 | k3 | archive/run3 | True | 155.4 | 1.226 |
| chebyshev_sigmoid_d7 | k4 | linear/run2 | True | 156.27 | 1.219 |
| chebyshev_sigmoid_d7 | k5 | linear/run3 | True | 120.4 | 1.583 |
| chebyshev_elu_d7 | baseline | baseline | True | 208.21 | 1.0 |
| chebyshev_elu_d7 | k1 | archive/run1 | True | 194.94 | 1.068 |
| chebyshev_elu_d7 | k2 | archive/run2 | True | 188.96 | 1.102 |
| chebyshev_elu_d7 | k3 | archive/run3 | True | 189.46 | 1.099 |
| chebyshev_elu_d7 | k4 | linear/run1 | True | 192.96 | 1.079 |
| chebyshev_elu_d7 | k5 | linear/run2 | True | 190.51 | 1.093 |
| chebyshev_elu_d7 | k6 | linear/run3 | True | 188.19 | 1.106 |
