# Phase-2 pilot: neutral re-scoring

Commit d1ccac7b28 (clean), Darwin arm64 arm, Apple clang version 17.0.0 (clang-1700.3.19.1), SEAL 4.1. Re-scored 2026-11-01T02:17:08.

Speedup = baseline eval-only median / kernel eval-only median, both from the same neutral session (1 warmup + 10 measured processes of ./main --repeats 10, interleaved). The final kernel of each run is the strategy's own selection; re-scoring never re-selects. E2E = CHEHAB execution_time_(ms) (parse + encode + encrypt + fhe()). Compile time: median of 3, reported only.

## Every run

| task | strategy | run | final | speedup | eval (ms) | e2e (ms) | FHE err | depth | xdepth | levels | ct x ct | ct x pt | align | rescale | mod sw | C (ms) | correct / 8 | dup | failures | out tok | think tok | search (s) |
|---|---|---:|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---|---:|---:|---:|
| chebyshev_elu_d7 | archive | 1 | #2 (k1) | x1.098 | 192.7 | 224.2 | 2.07e-07 | 14 | 5 | 5 | 4 | 9 | 5 | 18 | 2 | 0.64 | 8 / 8 | 0 | - | 99661 | 90932 | 657.8 |
| chebyshev_elu_d7 | archive | 2 | #5 (k2) | x1.095 | 193.2 | 222.5 | 1.71e-07 | 13 | 5 | 5 | 4 | 9 | 5 | 18 | 2 | 0.56 | 6 / 8 | 1 | chehab 1 | 83116 | 75763 | 529.6 |
| chebyshev_elu_d7 | archive | 3 | #2 (k3) | x1.092 | 193.7 | 223.4 | 2.08e-07 | 13 | 5 | 5 | 4 | 9 | 5 | 18 | 2 | 0.62 | 7 / 8 | 1 | - | 90993 | 82267 | 585.6 |
| chebyshev_elu_d7 | linear | 1 | #6 (k4) | x1.094 | 193.5 | 222.6 | 1.49e-07 | 13 | 5 | 5 | 4 | 9 | 5 | 18 | 2 | 0.63 | 5 / 8 | 3 | - | 49541 | 41744 | 354.4 |
| chebyshev_elu_d7 | linear | 2 | #4 (k5) | x1.091 | 194.0 | 224.5 | 2.34e-07 | 13 | 5 | 5 | 4 | 9 | 5 | 18 | 2 | 0.61 | 4 / 8 | 4 | - | 50764 | 42444 | 358.2 |
| chebyshev_elu_d7 | linear | 3 | #3 (k6) | x1.094 | 193.4 | 223.3 | 1.83e-07 | 13 | 5 | 5 | 4 | 9 | 5 | 18 | 2 | 0.56 | 6 / 8 | 2 | - | 43598 | 35147 | 338.0 |
| chebyshev_gelu_d7 | archive | 1 | #2 (k1) | x1.411 | 123.5 | 151.0 | 1.70e-07 | 11 | 4 | 4 | 3 | 4 | 2 | 9 | 1 | 0.50 | 7 / 8 | 1 | - | 62256 | 56660 | 423.7 |
| chebyshev_gelu_d7 | archive | 2 | #5 (k2) | x1.413 | 123.3 | 150.0 | 1.78e-07 | 9 | 4 | 4 | 3 | 4 | 2 | 9 | 0 | 0.50 | 7 / 8 | 1 | - | 51880 | 45796 | 384.0 |
| chebyshev_gelu_d7 | archive | 3 | #2 (k3) | x1.469 | 118.6 | 145.5 | 1.60e-07 | 14 | 5 | 5 | 3 | 4 | 2 | 9 | 3 | 0.51 | 7 / 8 | 0 | chehab 1 | 40551 | 34783 | 338.1 |
| chebyshev_gelu_d7 | linear | 1 | #3 (k4) | x1.408 | 123.8 | 150.5 | 1.46e-07 | 9 | 4 | 4 | 3 | 4 | 2 | 9 | 0 | 0.51 | 6 / 8 | 2 | - | 37138 | 31490 | 312.7 |
| chebyshev_gelu_d7 | linear | 2 | #2 (k5) | x1.412 | 123.4 | 150.0 | 1.83e-07 | 11 | 4 | 4 | 3 | 4 | 2 | 9 | 1 | 0.51 | 5 / 8 | 3 | - | 29465 | 24493 | 248.8 |
| chebyshev_gelu_d7 | linear | 3 | #2 (k6) | x1.466 | 118.9 | 146.1 | 1.35e-07 | 15 | 5 | 5 | 3 | 4 | 2 | 9 | 4 | 0.54 | 5 / 8 | 2 | chehab 1 | 58050 | 53094 | 375.3 |
| chebyshev_sigmoid_d7 | archive | 1 | #6 (k1) | x1.362 | 136.1 | 164.4 | 1.19e-07 | 14 | 5 | 5 | 4 | 3 | 1 | 8 | 3 | 0.55 | 7 / 8 | 1 | - | 129941 | 124152 | 751.4 |
| chebyshev_sigmoid_d7 | archive | 2 | #5 (k2) | x1.201 | 154.3 | 181.8 | 1.03e-07 | 14 | 5 | 5 | 4 | 5 | 4 | 13 | 8 | 0.54 | 6 / 8 | 2 | - | 97743 | 92703 | 592.3 |
| chebyshev_sigmoid_d7 | archive | 3 | #5 (k3) | x1.223 | 151.4 | 178.8 | 9.30e-08 | 12 | 5 | 5 | 4 | 5 | 3 | 12 | 6 | 0.58 | 8 / 8 | 0 | - | 90925 | 84898 | 597.2 |
| chebyshev_sigmoid_d7 | linear | 1 | #0 (baseline) | x1.000 | 185.3 | 214.3 | 1.18e-07 | 22 | 8 | 8 | 6 | 5 | 8 | 19 | 13 | 0.70 | 4 / 8 | 1 | chehab 3 | 84941 | 78106 | 510.4 |
| chebyshev_sigmoid_d7 | linear | 2 | #3 (k4) | x1.218 | 152.1 | 179.4 | 4.86e-08 | 21 | 6 | 6 | 4 | 4 | 5 | 13 | 6 | 0.71 | 3 / 8 | 4 | chehab 1 | 57202 | 50048 | 359.2 |
| chebyshev_sigmoid_d7 | linear | 3 | #7 (k5) | x1.559 | 118.8 | 145.7 | 8.83e-08 | 17 | 6 | 6 | 4 | 2 | 2 | 8 | 8 | 0.53 | 6 / 8 | 1 | wrong 1 | 76644 | 69588 | 471.3 |

## Per task and strategy (3 runs each)

| task | strategy | runs | speedup |
|---|---|---:|---|
| chebyshev_elu_d7 | archive | 3 | median x1.095, mean x1.095, min x1.092, max x1.098, sd 0.003 |
| chebyshev_gelu_d7 | archive | 3 | median x1.413, mean x1.431, min x1.411, max x1.469, sd 0.033 |
| chebyshev_sigmoid_d7 | archive | 3 | median x1.223, mean x1.262, min x1.201, max x1.362, sd 0.087 |
| chebyshev_elu_d7 | linear | 3 | median x1.094, mean x1.093, min x1.091, max x1.094, sd 0.002 |
| chebyshev_gelu_d7 | linear | 3 | median x1.412, mean x1.428, min x1.408, max x1.466, sd 0.032 |
| chebyshev_sigmoid_d7 | linear | 3 | median x1.218, mean x1.259, min x1.000, max x1.559, sd 0.282 |

## Across tasks (geometric mean of the per-task mean speedups)

| strategy | tasks | geometric mean |
|---|---:|---:|
| archive | 3 | x1.255 |
| linear | 3 | x1.253 |

## Approximation error (task property, separate from FHE error)

| task | max_fit_error (fitting grid) | approx. error, fixed points | approx. error, dense points |
|---|---:|---:|---:|
| chebyshev_elu_d7 | 0.0427493 | 0.0427493 | 0.0330832 |
| chebyshev_gelu_d7 | 0.0581538 | 0.0581538 | 0.0470933 |
| chebyshev_sigmoid_d7 | 0.002824 | 0.002824 | 0.0021268 |

## Re-scored kernels

| task | kernel | used by | correct | eval (ms) | speedup |
|---|---|---|---|---:|---:|
| chebyshev_elu_d7 | baseline | baseline | True | 211.58 | 1.0 |
| chebyshev_elu_d7 | k1 | archive/run1 | True | 192.74 | 1.098 |
| chebyshev_elu_d7 | k2 | archive/run2 | True | 193.15 | 1.095 |
| chebyshev_elu_d7 | k3 | archive/run3 | True | 193.67 | 1.092 |
| chebyshev_elu_d7 | k4 | linear/run1 | True | 193.48 | 1.094 |
| chebyshev_elu_d7 | k5 | linear/run2 | True | 193.98 | 1.091 |
| chebyshev_elu_d7 | k6 | linear/run3 | True | 193.37 | 1.094 |
| chebyshev_gelu_d7 | baseline | baseline | True | 174.22 | 1.0 |
| chebyshev_gelu_d7 | k1 | archive/run1 | True | 123.51 | 1.411 |
| chebyshev_gelu_d7 | k2 | archive/run2 | True | 123.31 | 1.413 |
| chebyshev_gelu_d7 | k3 | archive/run3 | True | 118.64 | 1.469 |
| chebyshev_gelu_d7 | k4 | linear/run1 | True | 123.76 | 1.408 |
| chebyshev_gelu_d7 | k5 | linear/run2 | True | 123.4 | 1.412 |
| chebyshev_gelu_d7 | k6 | linear/run3 | True | 118.86 | 1.466 |
| chebyshev_sigmoid_d7 | baseline | linear/run1 | True | 185.28 | 1.0 |
| chebyshev_sigmoid_d7 | k1 | archive/run1 | True | 136.06 | 1.362 |
| chebyshev_sigmoid_d7 | k2 | archive/run2 | True | 154.28 | 1.201 |
| chebyshev_sigmoid_d7 | k3 | archive/run3 | True | 151.45 | 1.223 |
| chebyshev_sigmoid_d7 | k4 | linear/run2 | True | 152.15 | 1.218 |
| chebyshev_sigmoid_d7 | k5 | linear/run3 | True | 118.82 | 1.559 |
