# Phase-2 pilot: neutral re-scoring

Commit 1d44c9e562 (clean), Darwin arm64 arm, Apple clang version 17.0.0 (clang-1700.3.19.1), SEAL 4.1. Re-scored 2026-11-10T01:17:16.

Speedup = baseline eval-only median / kernel eval-only median, both from the same neutral session (1 warmup + 10 measured processes of ./main --repeats 10, interleaved). The final kernel of each run is the strategy's own selection; re-scoring never re-selects. E2E = CHEHAB execution_time_(ms) (parse + encode + encrypt + fhe()). Compile time: median of 3, reported only.

## Every run

| task | strategy | run | final | speedup | eval (ms) | e2e (ms) | FHE err | depth | xdepth | levels | ct x ct | ct x pt | align | rescale | mod sw | C (ms) | correct / 8 | dup | failures | out tok | think tok | search (s) |
|---|---|---:|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---|---:|---:|---:|
| chebyshev_hardshrink_d7 | archive | 1 | #4 (k1) | x1.588 | 116.0 | 146.4 | 1.57e-07 | 15 | 6 | 6 | 4 | 2 | 1 | 7 | 8 | 0.52 | 7 / 8 | 1 | - | 91086 | 85251 | 1132.9 |
| chebyshev_hardshrink_d7 | archive | 2 | #2 (k2) | x1.568 | 117.5 | 144.2 | 1.75e-07 | 17 | 6 | 6 | 4 | 2 | 0 | 6 | 7 | 0.58 | 7 / 8 | 0 | - | 85300 | 79900 | 1096.4 |
| chebyshev_hardshrink_d7 | archive | 3 | #3 (k3) | x1.577 | 116.8 | 143.5 | 1.56e-07 | 14 | 6 | 6 | 4 | 2 | 1 | 7 | 8 | 0.61 | 4 / 8 | 0 | wrong 1 | 59202 | 53207 | 732.0 |
| chebyshev_mish_d7 | archive | 1 | #4 (k1) | x1.086 | 191.5 | 222.8 | 2.60e-07 | 14 | 5 | 5 | 4 | 9 | 5 | 18 | 4 | 0.61 | 7 / 8 | 0 | chehab 1 | 108447 | 100366 | 1369.3 |
| chebyshev_mish_d7 | archive | 2 | #4 (k2) | x1.184 | 175.7 | 208.6 | 1.94e-07 | 18 | 6 | 6 | 4 | 7 | 8 | 19 | 6 | 0.60 | 8 / 8 | 0 | - | 129292 | 122711 | 1563.1 |
| chebyshev_mish_d7 | archive | 3 | #3 (k3) | x1.188 | 175.2 | 208.7 | 1.52e-07 | 15 | 6 | 6 | 4 | 7 | 7 | 18 | 5 | 0.60 | 8 / 8 | 0 | - | 88773 | 81613 | 1205.8 |
| chebyshev_selu_d7 | archive | 1 | #7 (k1) | x1.099 | 189.0 | 222.3 | 2.27e-07 | 15 | 5 | 5 | 4 | 9 | 5 | 18 | 2 | 0.75 | 7 / 8 | 0 | chehab 1 | 129985 | 121805 | 1692.6 |
| chebyshev_selu_d7 | archive | 2 | #8 (k2) | x1.212 | 171.3 | 199.9 | 1.82e-07 | 16 | 5 | 5 | 4 | 9 | 3 | 16 | 2 | 0.62 | 6 / 8 | 1 | chehab 1 | 83510 | 76568 | 1085.0 |
| chebyshev_selu_d7 | archive | 3 | #8 (k3) | x1.255 | 165.4 | 194.4 | 3.74e-07 | 12 | 5 | 5 | 4 | 5 | 3 | 12 | 5 | 0.57 | 6 / 8 | 0 | chehab 2 | 102261 | 95017 | 3921.7 |
| chebyshev_sigmoid_d5 | archive | 1 | #3 (k1) | x1.513 | 97.6 | 124.8 | 9.01e-08 | 12 | 5 | 5 | 3 | 2 | 0 | 5 | 4 | 0.51 | 6 / 8 | 1 | wrong 1 | 60443 | 56126 | 804.6 |
| chebyshev_sigmoid_d5 | archive | 2 | #2 (k2) | x1.504 | 98.1 | 126.9 | 6.75e-08 | 12 | 5 | 5 | 3 | 2 | 0 | 5 | 4 | 0.46 | 5 / 8 | 2 | - | 79087 | 73605 | 979.3 |
| chebyshev_sigmoid_d5 | archive | 3 | #2 (k3) | x1.516 | 97.3 | 124.8 | 6.28e-08 | 12 | 5 | 5 | 3 | 2 | 0 | 5 | 4 | 0.56 | 4 / 8 | 1 | - | 48537 | 43368 | 633.3 |
| chebyshev_silu_d7 | archive | 1 | #4 (k1) | x1.443 | 120.3 | 152.7 | 2.42e-07 | 11 | 4 | 4 | 3 | 4 | 2 | 9 | 0 | 0.53 | 7 / 8 | 1 | - | 44880 | 39925 | 708.3 |
| chebyshev_silu_d7 | archive | 2 | #3 (k2) | x1.431 | 121.3 | 149.7 | 1.67e-07 | 11 | 4 | 4 | 3 | 4 | 2 | 9 | 0 | 0.53 | 4 / 8 | 2 | chehab 2 | 46931 | 41429 | 646.2 |
| chebyshev_silu_d7 | archive | 3 | #4 (k3) | x1.596 | 108.7 | 137.8 | 3.67e-07 | 12 | 5 | 5 | 3 | 3 | 2 | 8 | 3 | 0.56 | 8 / 8 | 0 | - | 35503 | 30553 | 611.3 |
| chebyshev_softplus_d7 | archive | 1 | #8 (k1) | x1.594 | 109.3 | 136.7 | 1.24e-07 | 16 | 5 | 5 | 3 | 3 | 1 | 7 | 4 | 0.53 | 8 / 8 | 0 | - | 37434 | 32718 | 625.9 |
| chebyshev_softplus_d7 | archive | 2 | #6 (k2) | x1.427 | 122.1 | 150.5 | 1.85e-07 | 12 | 4 | 4 | 3 | 4 | 2 | 9 | 1 | 0.62 | 7 / 8 | 0 | chehab 1 | 45636 | 40596 | 698.8 |
| chebyshev_softplus_d7 | archive | 3 | #1 (k3) | x1.408 | 123.7 | 151.4 | 1.10e-07 | 10 | 4 | 4 | 3 | 4 | 2 | 9 | 1 | 0.49 | 8 / 8 | 0 | - | 43838 | 38715 | 710.7 |

## Per task and strategy (3 runs each)

| task | strategy | runs | speedup |
|---|---|---:|---|
| chebyshev_hardshrink_d7 | archive | 3 | median x1.577, mean x1.577, min x1.568, max x1.588, sd 0.010 |
| chebyshev_mish_d7 | archive | 3 | median x1.184, mean x1.153, min x1.086, max x1.188, sd 0.057 |
| chebyshev_selu_d7 | archive | 3 | median x1.212, mean x1.189, min x1.099, max x1.255, sd 0.081 |
| chebyshev_sigmoid_d5 | archive | 3 | median x1.513, mean x1.511, min x1.504, max x1.516, sd 0.006 |
| chebyshev_silu_d7 | archive | 3 | median x1.443, mean x1.490, min x1.431, max x1.596, sd 0.092 |
| chebyshev_softplus_d7 | archive | 3 | median x1.427, mean x1.476, min x1.408, max x1.594, sd 0.102 |

## Across tasks (geometric mean of the per-task mean speedups)

| strategy | tasks | geometric mean |
|---|---:|---:|
| archive | 6 | x1.389 |

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
| chebyshev_silu_d7 | baseline | baseline | True | 173.5 | 1.0 |
| chebyshev_silu_d7 | k1 | archive/run1 | True | 120.27 | 1.443 |
| chebyshev_silu_d7 | k2 | archive/run2 | True | 121.29 | 1.431 |
| chebyshev_silu_d7 | k3 | archive/run3 | True | 108.73 | 1.596 |
| chebyshev_selu_d7 | baseline | baseline | True | 207.68 | 1.0 |
| chebyshev_selu_d7 | k1 | archive/run1 | True | 189.0 | 1.099 |
| chebyshev_selu_d7 | k2 | archive/run2 | True | 171.31 | 1.212 |
| chebyshev_selu_d7 | k3 | archive/run3 | True | 165.45 | 1.255 |
| chebyshev_softplus_d7 | baseline | baseline | True | 174.25 | 1.0 |
| chebyshev_softplus_d7 | k1 | archive/run1 | True | 109.34 | 1.594 |
| chebyshev_softplus_d7 | k2 | archive/run2 | True | 122.09 | 1.427 |
| chebyshev_softplus_d7 | k3 | archive/run3 | True | 123.74 | 1.408 |
| chebyshev_mish_d7 | baseline | baseline | True | 208.07 | 1.0 |
| chebyshev_mish_d7 | k1 | archive/run1 | True | 191.53 | 1.086 |
| chebyshev_mish_d7 | k2 | archive/run2 | True | 175.74 | 1.184 |
| chebyshev_mish_d7 | k3 | archive/run3 | True | 175.2 | 1.188 |
| chebyshev_hardshrink_d7 | baseline | baseline | True | 184.14 | 1.0 |
| chebyshev_hardshrink_d7 | k1 | archive/run1 | True | 115.99 | 1.588 |
| chebyshev_hardshrink_d7 | k2 | archive/run2 | True | 117.46 | 1.568 |
| chebyshev_hardshrink_d7 | k3 | archive/run3 | True | 116.8 | 1.577 |
| chebyshev_sigmoid_d5 | baseline | baseline | True | 147.58 | 1.0 |
| chebyshev_sigmoid_d5 | k1 | archive/run1 | True | 97.57 | 1.513 |
| chebyshev_sigmoid_d5 | k2 | archive/run2 | True | 98.12 | 1.504 |
| chebyshev_sigmoid_d5 | k3 | archive/run3 | True | 97.33 | 1.516 |
