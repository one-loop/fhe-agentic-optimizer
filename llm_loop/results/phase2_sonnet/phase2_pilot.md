# Phase-2 pilot: neutral re-scoring

Commit 865e5933a5 (clean), Darwin arm64 arm, Apple clang version 17.0.0 (clang-1700.3.19.1), SEAL 4.1. Re-scored 2026-11-02T01:05:56.

Speedup = baseline eval-only median / kernel eval-only median, both from the same neutral session (1 warmup + 10 measured processes of ./main --repeats 10, interleaved). The final kernel of each run is the strategy's own selection; re-scoring never re-selects. E2E = CHEHAB execution_time_(ms) (parse + encode + encrypt + fhe()). Compile time: median of 3, reported only.

## Every run

| task | strategy | run | final | speedup | eval (ms) | e2e (ms) | FHE err | depth | xdepth | levels | ct x ct | ct x pt | align | rescale | mod sw | C (ms) | correct / 8 | dup | failures | out tok | think tok | search (s) |
|---|---|---:|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---|---:|---:|---:|
| chebyshev_elu_d7 | archive | 1 | #6 (k1) | x1.092 | 189.1 | 217.5 | 1.84e-07 | 13 | 5 | 5 | 4 | 9 | 5 | 18 | 2 | 0.59 | 7 / 8 | 1 | - | 23829 | 18675 | 477.2 |
| chebyshev_elu_d7 | archive | 2 | #4 (k2) | x1.054 | 195.9 | 225.0 | 1.69e-07 | 20 | 6 | 6 | 4 | 7 | 12 | 23 | 7 | 0.69 | 6 / 8 | 1 | chehab 1 | 32727 | 26635 | 521.3 |
| chebyshev_elu_d7 | archive | 3 | #1 (k3) | x1.071 | 192.7 | 222.7 | 2.01e-07 | 14 | 5 | 5 | 4 | 9 | 6 | 19 | 3 | 0.56 | 7 / 8 | 1 | - | 26276 | 20399 | 478.7 |
| chebyshev_gelu_d7 | archive | 1 | #8 (k1) | x1.230 | 140.8 | 167.8 | 2.46e-07 | 18 | 5 | 5 | 3 | 5 | 6 | 14 | 2 | 0.58 | 5 / 8 | 1 | chehab 2 | 26599 | 20868 | 418.3 |
| chebyshev_gelu_d7 | archive | 2 | #2 (k2) | x1.413 | 122.6 | 149.9 | 2.15e-07 | 13 | 4 | 4 | 3 | 4 | 2 | 9 | 1 | 0.50 | 8 / 8 | 0 | - | 29278 | 25021 | 488.1 |
| chebyshev_gelu_d7 | archive | 3 | #4 (k3) | x1.468 | 118.0 | 144.9 | 2.05e-07 | 14 | 5 | 5 | 3 | 4 | 2 | 9 | 3 | 0.53 | 5 / 8 | 2 | chehab 1 | 16398 | 12291 | 313.2 |
| chebyshev_hardshrink_d7 | archive | 1 | #7 (k1) | x1.186 | 154.6 | 190.1 | 2.68e-07 | 15 | 5 | 5 | 4 | 5 | 3 | 12 | 5 | 0.56 | 5 / 8 | 1 | chehab 2 | 32908 | 26527 | 490.7 |
| chebyshev_hardshrink_d7 | archive | 2 | #4 (k2) | x1.369 | 133.9 | 167.3 | 1.15e-07 | 17 | 6 | 6 | 4 | 3 | 3 | 10 | 6 | 0.69 | 3 / 8 | 2 | chehab 2, wrong 1 | 35235 | 30595 | 427.5 |
| chebyshev_hardshrink_d7 | archive | 3 | #7 (k3) | x1.210 | 151.5 | 181.6 | 5.40e-07 | 12 | 5 | 5 | 4 | 5 | 3 | 12 | 5 | 0.56 | 7 / 8 | 0 | - | 35460 | 28224 | 533.0 |
| chebyshev_mish_d7 | archive | 1 | #2 (k1) | x1.082 | 190.4 | 219.7 | 2.23e-07 | 14 | 5 | 5 | 4 | 9 | 6 | 19 | 3 | 0.60 | 6 / 8 | 2 | - | 20831 | 15409 | 392.7 |
| chebyshev_mish_d7 | archive | 2 | #5 (k2) | x1.093 | 188.3 | 216.6 | 1.62e-07 | 13 | 5 | 5 | 4 | 9 | 5 | 18 | 2 | 0.57 | 6 / 8 | 1 | chehab 1 | 23826 | 18521 | 430.8 |
| chebyshev_mish_d7 | archive | 3 | #0 (baseline) | x1.000 | 205.9 | 234.6 | 1.70e-07 | 22 | 8 | 8 | 6 | 8 | 11 | 25 | 10 | 0.63 | 0 / 8 | 0 | chehab 8 | 17981 | 12543 | 190.7 |
| chebyshev_selu_d7 | archive | 1 | #6 (k1) | x1.095 | 188.3 | 216.9 | 2.25e-07 | 14 | 5 | 5 | 4 | 9 | 5 | 18 | 2 | 0.59 | 7 / 8 | 1 | - | 19115 | 13397 | 421.3 |
| chebyshev_selu_d7 | archive | 2 | #0 (baseline) | x1.000 | 206.3 | 235.2 | 1.97e-07 | 22 | 8 | 8 | 6 | 8 | 11 | 25 | 10 | 0.63 | 1 / 8 | 2 | chehab 5 | 19693 | 14274 | 236.0 |
| chebyshev_selu_d7 | archive | 3 | #1 (k2) | x1.072 | 192.5 | 221.1 | 2.04e-07 | 14 | 5 | 5 | 4 | 9 | 6 | 19 | 3 | 0.57 | 6 / 8 | 2 | - | 24637 | 19523 | 437.8 |
| chebyshev_sigmoid_d5 | archive | 1 | #4 (k1) | x1.524 | 96.8 | 123.0 | 6.40e-08 | 12 | 5 | 5 | 3 | 2 | 0 | 5 | 4 | 0.44 | 6 / 8 | 1 | - | 24152 | 19793 | 422.8 |
| chebyshev_sigmoid_d5 | archive | 2 | #1 (k2) | x1.210 | 121.9 | 149.7 | 1.50e-07 | 12 | 4 | 4 | 3 | 4 | 2 | 9 | 2 | 0.48 | 8 / 8 | 0 | - | 20911 | 15817 | 418.9 |
| chebyshev_sigmoid_d5 | archive | 3 | #3 (k3) | x1.524 | 96.8 | 122.5 | 5.72e-08 | 12 | 5 | 5 | 3 | 2 | 0 | 5 | 4 | 0.44 | 6 / 8 | 2 | - | 26320 | 22138 | 414.9 |
| chebyshev_sigmoid_d7 | archive | 1 | #1 (k1) | x1.263 | 145.8 | 174.3 | 4.87e-08 | 16 | 6 | 6 | 4 | 4 | 4 | 12 | 6 | 0.63 | 6 / 8 | 0 | chehab 2 | 30208 | 25099 | 484.3 |
| chebyshev_sigmoid_d7 | archive | 2 | #1 (k2) | x1.305 | 141.1 | 168.0 | 9.14e-08 | 15 | 6 | 6 | 4 | 4 | 3 | 11 | 5 | 0.57 | 6 / 8 | 1 | chehab 1 | 23858 | 19266 | 427.9 |
| chebyshev_sigmoid_d7 | archive | 3 | #1 (k3) | x1.184 | 155.6 | 185.8 | 2.00e-07 | 16 | 5 | 5 | 4 | 5 | 3 | 12 | 5 | 0.58 | 5 / 8 | 2 | wrong 1 | 31709 | 25784 | 452.8 |
| chebyshev_silu_d7 | archive | 1 | #5 (k1) | x1.414 | 122.8 | 149.6 | 2.23e-07 | 13 | 4 | 4 | 3 | 4 | 2 | 9 | 1 | 0.52 | 6 / 8 | 1 | wrong 1 | 29966 | 24510 | 463.5 |
| chebyshev_silu_d7 | archive | 2 | #5 (k2) | x1.427 | 121.7 | 148.2 | 1.74e-07 | 10 | 4 | 4 | 3 | 4 | 2 | 9 | 0 | 0.51 | 8 / 8 | 0 | - | 23073 | 19050 | 459.1 |
| chebyshev_silu_d7 | archive | 3 | #3 (k3) | x1.416 | 122.6 | 150.1 | 2.02e-07 | 10 | 4 | 4 | 3 | 4 | 2 | 9 | 0 | 0.56 | 5 / 8 | 2 | chehab 1 | 15787 | 12128 | 302.2 |
| chebyshev_softplus_d7 | archive | 1 | #4 (k1) | x1.415 | 122.5 | 149.7 | 1.32e-07 | 10 | 4 | 4 | 3 | 4 | 2 | 9 | 0 | 0.48 | 6 / 8 | 1 | chehab 1 | 14349 | 10564 | 327.7 |
| chebyshev_softplus_d7 | archive | 2 | #3 (k2) | x1.411 | 122.9 | 149.1 | 1.66e-07 | 9 | 4 | 4 | 3 | 4 | 2 | 9 | 0 | 0.50 | 8 / 8 | 0 | - | 14885 | 11049 | 354.3 |
| chebyshev_softplus_d7 | archive | 3 | #2 (k3) | x1.471 | 117.9 | 145.8 | 1.54e-07 | 14 | 5 | 5 | 3 | 4 | 2 | 9 | 3 | 0.52 | 7 / 8 | 1 | - | 19057 | 15198 | 403.0 |

## Per task and strategy (3 runs each)

| task | strategy | runs | speedup |
|---|---|---:|---|
| chebyshev_elu_d7 | archive | 3 | median x1.071, mean x1.072, min x1.054, max x1.092, sd 0.019 |
| chebyshev_gelu_d7 | archive | 3 | median x1.413, mean x1.370, min x1.230, max x1.468, sd 0.124 |
| chebyshev_hardshrink_d7 | archive | 3 | median x1.210, mean x1.255, min x1.186, max x1.369, sd 0.100 |
| chebyshev_mish_d7 | archive | 3 | median x1.082, mean x1.058, min x1.000, max x1.093, sd 0.051 |
| chebyshev_selu_d7 | archive | 3 | median x1.072, mean x1.056, min x1.000, max x1.095, sd 0.050 |
| chebyshev_sigmoid_d5 | archive | 3 | median x1.524, mean x1.419, min x1.210, max x1.524, sd 0.181 |
| chebyshev_sigmoid_d7 | archive | 3 | median x1.263, mean x1.251, min x1.184, max x1.305, sd 0.062 |
| chebyshev_silu_d7 | archive | 3 | median x1.416, mean x1.419, min x1.414, max x1.427, sd 0.007 |
| chebyshev_softplus_d7 | archive | 3 | median x1.415, mean x1.432, min x1.411, max x1.471, sd 0.033 |

## Across tasks (geometric mean of the per-task mean speedups)

| strategy | tasks | geometric mean |
|---|---:|---:|
| archive | 9 | x1.250 |

## Approximation error (task property, separate from FHE error)

| task | max_fit_error (fitting grid) | approx. error, fixed points | approx. error, dense points |
|---|---:|---:|---:|
| chebyshev_elu_d7 | 0.0427493 | 0.0427493 | 0.0330832 |
| chebyshev_gelu_d7 | 0.0581538 | 0.0581538 | 0.0470933 |
| chebyshev_hardshrink_d7 | 0.464224 | 0.050663 | 0.455449 |
| chebyshev_mish_d7 | 0.03818 | 0.03818 | 0.0258338 |
| chebyshev_selu_d7 | 0.0947603 | 0.0947603 | 0.0892641 |
| chebyshev_sigmoid_d5 | 0.0105848 | 0.0105848 | 0.00897247 |
| chebyshev_sigmoid_d7 | 0.002824 | 0.002824 | 0.0021268 |
| chebyshev_silu_d7 | 0.0172606 | 0.0172606 | 0.013855 |
| chebyshev_softplus_d7 | 0.00357233 | 0.00357233 | 0.00285445 |

## Re-scored kernels

| task | kernel | used by | correct | eval (ms) | speedup |
|---|---|---|---|---:|---:|
| chebyshev_elu_d7 | baseline | baseline | True | 206.5 | 1.0 |
| chebyshev_elu_d7 | k1 | archive/run1 | True | 189.09 | 1.092 |
| chebyshev_elu_d7 | k2 | archive/run2 | True | 195.95 | 1.054 |
| chebyshev_elu_d7 | k3 | archive/run3 | True | 192.75 | 1.071 |
| chebyshev_gelu_d7 | baseline | baseline | True | 173.25 | 1.0 |
| chebyshev_gelu_d7 | k1 | archive/run1 | True | 140.8 | 1.23 |
| chebyshev_gelu_d7 | k2 | archive/run2 | True | 122.65 | 1.413 |
| chebyshev_gelu_d7 | k3 | archive/run3 | True | 118.0 | 1.468 |
| chebyshev_hardshrink_d7 | baseline | baseline | True | 183.31 | 1.0 |
| chebyshev_hardshrink_d7 | k1 | archive/run1 | True | 154.57 | 1.186 |
| chebyshev_hardshrink_d7 | k2 | archive/run2 | True | 133.85 | 1.369 |
| chebyshev_hardshrink_d7 | k3 | archive/run3 | True | 151.55 | 1.21 |
| chebyshev_mish_d7 | baseline | archive/run3 | True | 205.92 | 1.0 |
| chebyshev_mish_d7 | k1 | archive/run1 | True | 190.39 | 1.082 |
| chebyshev_mish_d7 | k2 | archive/run2 | True | 188.32 | 1.093 |
| chebyshev_selu_d7 | baseline | archive/run2 | True | 206.31 | 1.0 |
| chebyshev_selu_d7 | k1 | archive/run1 | True | 188.32 | 1.095 |
| chebyshev_selu_d7 | k2 | archive/run3 | True | 192.53 | 1.072 |
| chebyshev_sigmoid_d5 | baseline | baseline | True | 147.45 | 1.0 |
| chebyshev_sigmoid_d5 | k1 | archive/run1 | True | 96.75 | 1.524 |
| chebyshev_sigmoid_d5 | k2 | archive/run2 | True | 121.91 | 1.21 |
| chebyshev_sigmoid_d5 | k3 | archive/run3 | True | 96.77 | 1.524 |
| chebyshev_sigmoid_d7 | baseline | baseline | True | 184.12 | 1.0 |
| chebyshev_sigmoid_d7 | k1 | archive/run1 | True | 145.77 | 1.263 |
| chebyshev_sigmoid_d7 | k2 | archive/run2 | True | 141.06 | 1.305 |
| chebyshev_sigmoid_d7 | k3 | archive/run3 | True | 155.55 | 1.184 |
| chebyshev_silu_d7 | baseline | baseline | True | 173.65 | 1.0 |
| chebyshev_silu_d7 | k1 | archive/run1 | True | 122.83 | 1.414 |
| chebyshev_silu_d7 | k2 | archive/run2 | True | 121.7 | 1.427 |
| chebyshev_silu_d7 | k3 | archive/run3 | True | 122.62 | 1.416 |
| chebyshev_softplus_d7 | baseline | baseline | True | 173.36 | 1.0 |
| chebyshev_softplus_d7 | k1 | archive/run1 | True | 122.55 | 1.415 |
| chebyshev_softplus_d7 | k2 | archive/run2 | True | 122.9 | 1.411 |
| chebyshev_softplus_d7 | k3 | archive/run3 | True | 117.89 | 1.471 |
