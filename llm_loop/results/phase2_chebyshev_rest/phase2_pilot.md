# Phase-2 pilot: neutral re-scoring

Commit af0dad4ecf (clean), Darwin arm64 arm, Apple clang version 17.0.0 (clang-1700.3.19.1), SEAL 4.1. Re-scored 2026-10-26T19:11:08.

Speedup = baseline eval-only median / kernel eval-only median, both from the same neutral session (1 warmup + 10 measured processes of ./main --repeats 10, interleaved). The final kernel of each run is the strategy's own selection; re-scoring never re-selects. E2E = CHEHAB execution_time_(ms) (parse + encode + encrypt + fhe()). Compile time: median of 3, reported only.

## Every run

| task | strategy | run | final | speedup | eval (ms) | e2e (ms) | FHE err | depth | xdepth | levels | ct x ct | ct x pt | align | rescale | mod sw | C (ms) | correct / 8 | dup | failures | out tok | think tok | search (s) |
|---|---|---:|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---|---:|---:|---:|
| chebyshev_hardshrink_d7 | archive | 1 | #7 (k1) | x1.376 | 134.7 | 162.2 | 1.32e-07 | 13 | 5 | 5 | 4 | 3 | 1 | 8 | 3 | 0.53 | 8 / 8 | 0 | - | 113572 | 108054 | 701.0 |
| chebyshev_hardshrink_d7 | archive | 2 | #8 (k2) | x1.229 | 150.8 | 182.6 | 1.66e-07 | 12 | 5 | 5 | 4 | 5 | 3 | 12 | 6 | 0.67 | 7 / 8 | 0 | chehab 1 | 110592 | 105001 | 670.4 |
| chebyshev_hardshrink_d7 | archive | 3 | #2 (k3) | x1.233 | 150.3 | 178.6 | 1.41e-07 | 20 | 6 | 6 | 4 | 4 | 5 | 13 | 6 | 0.59 | 5 / 8 | 1 | chehab 1 | 120832 | 113504 | 694.5 |
| chebyshev_hardshrink_d7 | linear | 1 | #3 (k4) | x1.460 | 127.0 | 154.8 | 1.86e-07 | 16 | 6 | 6 | 4 | 3 | 1 | 8 | 6 | 0.61 | 2 / 8 | 5 | chehab 1 | 51675 | 45375 | 309.3 |
| chebyshev_hardshrink_d7 | linear | 2 | #2 (k5) | x1.199 | 154.6 | 183.6 | 1.63e-07 | 15 | 6 | 6 | 4 | 5 | 6 | 15 | 8 | 0.52 | 5 / 8 | 3 | - | 104467 | 97582 | 621.1 |
| chebyshev_hardshrink_d7 | linear | 3 | #5 (k6) | x1.231 | 150.5 | 178.6 | 1.54e-07 | 13 | 5 | 5 | 4 | 5 | 2 | 11 | 6 | 0.62 | 6 / 8 | 2 | - | 47720 | 42428 | 358.3 |
| chebyshev_mish_d7 | archive | 1 | #5 (k1) | x1.090 | 192.1 | 223.8 | 2.06e-07 | 13 | 5 | 5 | 4 | 9 | 5 | 18 | 2 | 0.54 | 8 / 8 | 0 | - | 106024 | 98434 | 679.6 |
| chebyshev_mish_d7 | archive | 2 | #5 (k2) | x1.086 | 192.7 | 226.8 | 2.08e-07 | 13 | 5 | 5 | 4 | 9 | 6 | 19 | 3 | 0.56 | 6 / 8 | 0 | chehab 2 | 79718 | 71049 | 529.8 |
| chebyshev_mish_d7 | archive | 3 | #6 (k3) | x1.088 | 192.5 | 227.2 | 2.10e-07 | 13 | 5 | 5 | 4 | 9 | 6 | 19 | 3 | 0.55 | 2 / 8 | 0 | chehab 6 | 101294 | 93754 | 542.3 |
| chebyshev_mish_d7 | linear | 1 | #2 (k4) | x1.076 | 194.5 | 221.4 | 1.63e-07 | 13 | 5 | 5 | 4 | 9 | 5 | 18 | 2 | 0.56 | 4 / 8 | 4 | - | 38560 | 32044 | 288.2 |
| chebyshev_mish_d7 | linear | 2 | #2 (k5) | x1.070 | 195.7 | 229.4 | 1.93e-07 | 13 | 5 | 5 | 4 | 9 | 6 | 19 | 3 | 0.56 | 6 / 8 | 2 | - | 57243 | 50268 | 411.0 |
| chebyshev_mish_d7 | linear | 3 | #4 (k6) | x1.080 | 193.8 | 229.2 | 2.00e-07 | 14 | 5 | 5 | 4 | 9 | 6 | 19 | 4 | 0.55 | 3 / 8 | 3 | chehab 2 | 43219 | 35038 | 287.8 |
| chebyshev_selu_d7 | archive | 1 | #4 (k1) | x1.083 | 192.7 | 222.7 | 1.84e-07 | 13 | 5 | 5 | 4 | 9 | 6 | 19 | 4 | 0.57 | 4 / 8 | 0 | chehab 4 | 53509 | 45201 | 357.7 |
| chebyshev_selu_d7 | archive | 2 | #1 (k2) | x1.076 | 193.9 | 225.2 | 2.01e-07 | 13 | 5 | 5 | 4 | 9 | 6 | 19 | 3 | 0.58 | 4 / 8 | 2 | chehab 1, wrong 1 | 63850 | 56219 | 424.0 |
| chebyshev_selu_d7 | archive | 3 | #3 (k3) | x1.081 | 193.0 | 223.0 | 1.91e-07 | 14 | 5 | 5 | 4 | 9 | 6 | 19 | 5 | 0.60 | 5 / 8 | 2 | chehab 1 | 73503 | 65383 | 471.7 |
| chebyshev_selu_d7 | linear | 1 | #5 (k4) | x1.095 | 190.5 | 221.8 | 1.86e-07 | 13 | 5 | 5 | 4 | 9 | 5 | 18 | 2 | 0.56 | 5 / 8 | 3 | - | 68949 | 60397 | 451.3 |
| chebyshev_selu_d7 | linear | 2 | #2 (k5) | x1.093 | 190.9 | 220.2 | 1.86e-07 | 13 | 5 | 5 | 4 | 9 | 5 | 18 | 2 | 0.58 | 5 / 8 | 3 | - | 55662 | 47681 | 384.2 |
| chebyshev_selu_d7 | linear | 3 | #2 (k6) | x1.084 | 192.5 | 223.4 | 1.78e-07 | 13 | 5 | 5 | 4 | 9 | 6 | 19 | 3 | 0.56 | 5 / 8 | 3 | - | 49623 | 42244 | 358.1 |
| chebyshev_sigmoid_d5 | archive | 1 | #1 (k1) | x1.135 | 130.9 | 159.5 | 6.71e-08 | 13 | 5 | 5 | 3 | 4 | 5 | 12 | 6 | 0.53 | 6 / 8 | 0 | chehab 2 | 89182 | 80902 | 552.5 |
| chebyshev_sigmoid_d5 | archive | 2 | #2 (k2) | x1.362 | 109.1 | 137.5 | 1.26e-07 | 22 | 5 | 5 | 3 | 2 | 0 | 5 | 4 | 0.78 | 4 / 8 | 1 | chehab 1 | 91436 | 83496 | 538.2 |
| chebyshev_sigmoid_d5 | archive | 3 | #2 (k3) | x1.516 | 98.0 | 125.0 | 6.02e-08 | 12 | 5 | 5 | 3 | 2 | 0 | 5 | 4 | 0.72 | 2 / 8 | 3 | - | 72938 | 66812 | 409.0 |
| chebyshev_sigmoid_d5 | linear | 1 | #6 (k4) | x1.308 | 113.6 | 142.1 | 4.23e-08 | 16 | 5 | 5 | 3 | 3 | 1 | 7 | 3 | 0.55 | 6 / 8 | 1 | - | 67310 | 59780 | 432.6 |
| chebyshev_sigmoid_d5 | linear | 2 | #4 (k5) | x1.249 | 119.0 | 148.9 | 3.87e-08 | 15 | 5 | 5 | 3 | 3 | 3 | 9 | 3 | 0.55 | 3 / 8 | 5 | - | 47875 | 42260 | 321.9 |
| chebyshev_sigmoid_d5 | linear | 3 | #1 (k6) | x1.239 | 119.9 | 149.3 | 7.26e-08 | 15 | 5 | 5 | 3 | 3 | 3 | 9 | 3 | 0.54 | 4 / 8 | 4 | - | 56500 | 50813 | 363.1 |
| chebyshev_silu_d7 | archive | 1 | #2 (k1) | x1.402 | 124.3 | 152.3 | 1.81e-07 | 9 | 4 | 4 | 3 | 4 | 2 | 9 | 0 | 0.77 | 5 / 8 | 2 | chehab 1 | 100743 | 94680 | 610.6 |
| chebyshev_silu_d7 | archive | 2 | #8 (k2) | x1.552 | 112.3 | 141.9 | 1.96e-07 | 15 | 5 | 5 | 3 | 3 | 1 | 7 | 4 | 0.72 | 7 / 8 | 0 | chehab 1 | 70096 | 63909 | 479.2 |
| chebyshev_silu_d7 | archive | 3 | #2 (k3) | x1.413 | 123.3 | 150.6 | 1.57e-07 | 10 | 4 | 4 | 3 | 4 | 2 | 9 | 0 | 0.49 | 7 / 8 | 1 | - | 110196 | 104517 | 670.7 |
| chebyshev_silu_d7 | linear | 1 | #5 (k4) | x1.406 | 123.9 | 152.0 | 1.56e-07 | 10 | 4 | 4 | 3 | 4 | 2 | 9 | 1 | 0.51 | 4 / 8 | 2 | compile 1, chehab 1 | 41388 | 36477 | 294.6 |
| chebyshev_silu_d7 | linear | 2 | #4 (k5) | x1.412 | 123.4 | 153.4 | 1.59e-07 | 9 | 4 | 4 | 3 | 4 | 2 | 9 | 0 | 0.61 | 6 / 8 | 1 | chehab 1 | 43272 | 37422 | 343.8 |
| chebyshev_silu_d7 | linear | 3 | #4 (k6) | x1.192 | 146.2 | 178.8 | 1.48e-07 | 17 | 5 | 5 | 3 | 5 | 6 | 14 | 4 | 0.59 | 5 / 8 | 3 | - | 70390 | 62321 | 447.8 |
| chebyshev_softplus_d7 | archive | 1 | #6 (k1) | x1.425 | 121.6 | 149.0 | 1.19e-07 | 10 | 4 | 4 | 3 | 4 | 2 | 9 | 1 | 0.56 | 6 / 8 | 2 | - | 67068 | 61439 | 441.2 |
| chebyshev_softplus_d7 | archive | 2 | #2 (k2) | x1.469 | 118.0 | 146.3 | 2.13e-07 | 14 | 5 | 5 | 3 | 4 | 2 | 9 | 3 | 0.68 | 5 / 8 | 2 | chehab 1 | 67930 | 62843 | 429.5 |
| chebyshev_softplus_d7 | archive | 3 | #6 (k3) | x1.412 | 122.7 | 149.7 | 1.68e-07 | 11 | 4 | 4 | 3 | 4 | 2 | 9 | 1 | 0.50 | 6 / 8 | 0 | chehab 1, wrong 1 | 61780 | 55936 | 424.0 |
| chebyshev_softplus_d7 | linear | 1 | #4 (k4) | x1.467 | 118.2 | 146.3 | 1.08e-07 | 15 | 5 | 5 | 3 | 4 | 2 | 9 | 4 | 0.58 | 5 / 8 | 2 | chehab 1 | 72812 | 67677 | 453.1 |
| chebyshev_softplus_d7 | linear | 2 | #2 (k5) | x1.415 | 122.5 | 150.5 | 1.45e-07 | 10 | 4 | 4 | 3 | 4 | 2 | 9 | 0 | 0.51 | 5 / 8 | 3 | - | 32936 | 26680 | 256.4 |
| chebyshev_softplus_d7 | linear | 3 | #1 (k6) | x1.195 | 145.1 | 174.1 | 1.58e-07 | 17 | 5 | 5 | 3 | 5 | 6 | 14 | 5 | 0.64 | 5 / 8 | 2 | held-out 1 | 82614 | 72100 | 504.0 |

## Per task and strategy (3 runs each)

| task | strategy | runs | speedup |
|---|---|---:|---|
| chebyshev_hardshrink_d7 | archive | 3 | median x1.233, mean x1.279, min x1.229, max x1.376, sd 0.084 |
| chebyshev_mish_d7 | archive | 3 | median x1.088, mean x1.088, min x1.086, max x1.090, sd 0.002 |
| chebyshev_selu_d7 | archive | 3 | median x1.081, mean x1.080, min x1.076, max x1.083, sd 0.003 |
| chebyshev_sigmoid_d5 | archive | 3 | median x1.362, mean x1.337, min x1.135, max x1.516, sd 0.192 |
| chebyshev_silu_d7 | archive | 3 | median x1.413, mean x1.456, min x1.402, max x1.552, sd 0.083 |
| chebyshev_softplus_d7 | archive | 3 | median x1.425, mean x1.436, min x1.412, max x1.469, sd 0.030 |
| chebyshev_hardshrink_d7 | linear | 3 | median x1.231, mean x1.297, min x1.199, max x1.460, sd 0.142 |
| chebyshev_mish_d7 | linear | 3 | median x1.076, mean x1.075, min x1.070, max x1.080, sd 0.005 |
| chebyshev_selu_d7 | linear | 3 | median x1.093, mean x1.091, min x1.084, max x1.095, sd 0.006 |
| chebyshev_sigmoid_d5 | linear | 3 | median x1.249, mean x1.265, min x1.239, max x1.308, sd 0.037 |
| chebyshev_silu_d7 | linear | 3 | median x1.406, mean x1.336, min x1.192, max x1.412, sd 0.125 |
| chebyshev_softplus_d7 | linear | 3 | median x1.415, mean x1.359, min x1.195, max x1.467, sd 0.145 |

## Across tasks (geometric mean of the per-task mean speedups)

| strategy | tasks | geometric mean |
|---|---:|---:|
| archive | 6 | x1.270 |
| linear | 6 | x1.232 |

## Approximation error (task property, separate from FHE error)

| task | max_fit_error (fitting grid) | approx. error, fixed points | approx. error, dense points |
|---|---:|---:|---:|
| chebyshev_hardshrink_d7 | 0.464224 | 0.050663 | 0.455449 |
| chebyshev_mish_d7 | 0.03818 | 0.03818 | 0.0258338 |
| chebyshev_selu_d7 | 0.0947603 | 0.0947603 | 0.0892641 |
| chebyshev_sigmoid_d5 | 0.0105848 | 0.0105848 | 0.00897247 |
| chebyshev_silu_d7 | 0.0172606 | 0.0172606 | 0.013855 |
| chebyshev_softplus_d7 | 0.00357233 | 0.00357233 | 0.00285445 |

## Re-scored kernels

| task | kernel | used by | correct | eval (ms) | speedup |
|---|---|---|---|---:|---:|
| chebyshev_silu_d7 | baseline | baseline | True | 174.24 | 1.0 |
| chebyshev_silu_d7 | k1 | archive/run1 | True | 124.28 | 1.402 |
| chebyshev_silu_d7 | k2 | archive/run2 | True | 112.31 | 1.552 |
| chebyshev_silu_d7 | k3 | archive/run3 | True | 123.3 | 1.413 |
| chebyshev_silu_d7 | k4 | linear/run1 | True | 123.94 | 1.406 |
| chebyshev_silu_d7 | k5 | linear/run2 | True | 123.43 | 1.412 |
| chebyshev_silu_d7 | k6 | linear/run3 | True | 146.22 | 1.192 |
| chebyshev_selu_d7 | baseline | baseline | True | 208.69 | 1.0 |
| chebyshev_selu_d7 | k1 | archive/run1 | True | 192.73 | 1.083 |
| chebyshev_selu_d7 | k2 | archive/run2 | True | 193.87 | 1.076 |
| chebyshev_selu_d7 | k3 | archive/run3 | True | 193.05 | 1.081 |
| chebyshev_selu_d7 | k4 | linear/run1 | True | 190.51 | 1.095 |
| chebyshev_selu_d7 | k5 | linear/run2 | True | 190.86 | 1.093 |
| chebyshev_selu_d7 | k6 | linear/run3 | True | 192.51 | 1.084 |
| chebyshev_softplus_d7 | baseline | baseline | True | 173.33 | 1.0 |
| chebyshev_softplus_d7 | k1 | archive/run1 | True | 121.59 | 1.425 |
| chebyshev_softplus_d7 | k2 | archive/run2 | True | 117.99 | 1.469 |
| chebyshev_softplus_d7 | k3 | archive/run3 | True | 122.71 | 1.412 |
| chebyshev_softplus_d7 | k4 | linear/run1 | True | 118.18 | 1.467 |
| chebyshev_softplus_d7 | k5 | linear/run2 | True | 122.49 | 1.415 |
| chebyshev_softplus_d7 | k6 | linear/run3 | True | 145.1 | 1.195 |
| chebyshev_mish_d7 | baseline | baseline | True | 209.33 | 1.0 |
| chebyshev_mish_d7 | k1 | archive/run1 | True | 192.07 | 1.09 |
| chebyshev_mish_d7 | k2 | archive/run2 | True | 192.7 | 1.086 |
| chebyshev_mish_d7 | k3 | archive/run3 | True | 192.49 | 1.088 |
| chebyshev_mish_d7 | k4 | linear/run1 | True | 194.52 | 1.076 |
| chebyshev_mish_d7 | k5 | linear/run2 | True | 195.69 | 1.07 |
| chebyshev_mish_d7 | k6 | linear/run3 | True | 193.77 | 1.08 |
| chebyshev_hardshrink_d7 | baseline | baseline | True | 185.32 | 1.0 |
| chebyshev_hardshrink_d7 | k1 | archive/run1 | True | 134.72 | 1.376 |
| chebyshev_hardshrink_d7 | k2 | archive/run2 | True | 150.8 | 1.229 |
| chebyshev_hardshrink_d7 | k3 | archive/run3 | True | 150.33 | 1.233 |
| chebyshev_hardshrink_d7 | k4 | linear/run1 | True | 126.96 | 1.46 |
| chebyshev_hardshrink_d7 | k5 | linear/run2 | True | 154.55 | 1.199 |
| chebyshev_hardshrink_d7 | k6 | linear/run3 | True | 150.51 | 1.231 |
| chebyshev_sigmoid_d5 | baseline | baseline | True | 148.57 | 1.0 |
| chebyshev_sigmoid_d5 | k1 | archive/run1 | True | 130.94 | 1.135 |
| chebyshev_sigmoid_d5 | k2 | archive/run2 | True | 109.1 | 1.362 |
| chebyshev_sigmoid_d5 | k3 | archive/run3 | True | 98.0 | 1.516 |
| chebyshev_sigmoid_d5 | k4 | linear/run1 | True | 113.59 | 1.308 |
| chebyshev_sigmoid_d5 | k5 | linear/run2 | True | 118.96 | 1.249 |
| chebyshev_sigmoid_d5 | k6 | linear/run3 | True | 119.93 | 1.239 |
