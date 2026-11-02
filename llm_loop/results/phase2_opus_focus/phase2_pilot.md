# Phase-2 pilot: neutral re-scoring

Commit 3c62a442cd (clean), Darwin arm64 arm, Apple clang version 17.0.0 (clang-1700.3.19.1), SEAL 4.1. Re-scored 2026-11-02T09:20:34.

Speedup = baseline eval-only median / kernel eval-only median, both from the same neutral session (1 warmup + 10 measured processes of ./main --repeats 10, interleaved). The final kernel of each run is the strategy's own selection; re-scoring never re-selects. E2E = CHEHAB execution_time_(ms) (parse + encode + encrypt + fhe()). Compile time: median of 3, reported only.

## Every run

| task | strategy | run | final | speedup | eval (ms) | e2e (ms) | FHE err | depth | xdepth | levels | ct x ct | ct x pt | align | rescale | mod sw | C (ms) | correct / 8 | dup | failures | out tok | think tok | search (s) |
|---|---|---:|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---|---:|---:|---:|
| chebyshev_elu_d7 | archive | 1 | #8 (k1) | x1.268 | 162.4 | 190.6 | 1.56e-07 | 20 | 6 | 6 | 4 | 7 | 5 | 16 | 4 | 0.60 | 7 / 8 | 1 | - | 79523 | 71703 | 1068.3 |
| chebyshev_elu_d7 | archive | 2 | #8 (k2) | x1.360 | 151.4 | 178.1 | 1.34e-07 | 20 | 6 | 6 | 4 | 7 | 3 | 14 | 6 | 0.60 | 8 / 8 | 0 | - | 81388 | 73464 | 1109.9 |
| chebyshev_elu_d7 | archive | 3 | #6 (k3) | x1.248 | 164.9 | 192.0 | 1.65e-07 | 20 | 6 | 6 | 4 | 7 | 6 | 17 | 6 | 0.61 | 6 / 8 | 0 | runtime 1, wrong 1 | 71845 | 63964 | 962.3 |
| chebyshev_gelu_d7 | archive | 1 | #8 (k1) | x2.626 | 65.8 | 90.8 | 5.92e-07 | 18 | 10 | 10 | 3 | 10 | 2 | 15 | 0 | 0.50 | 8 / 8 | 0 | - | 51697 | 45505 | 769.1 |
| chebyshev_gelu_d7 | archive | 2 | #4 (k2) | x1.578 | 109.5 | 135.4 | 1.77e-07 | 17 | 5 | 5 | 3 | 3 | 1 | 7 | 4 | 0.46 | 8 / 8 | 0 | - | 38777 | 33539 | 625.3 |
| chebyshev_gelu_d7 | archive | 3 | #2 (k3) | x1.596 | 108.3 | 134.2 | 1.94e-07 | 16 | 5 | 5 | 3 | 3 | 1 | 7 | 4 | 0.51 | 8 / 8 | 0 | - | 44850 | 39457 | 692.0 |
| chebyshev_sigmoid_d7 | archive | 1 | #8 (k1) | x2.546 | 72.2 | 97.8 | 6.97e-08 | 20 | 10 | 10 | 4 | 6 | 0 | 10 | 7 | 0.55 | 8 / 8 | 0 | - | 73406 | 67554 | 1010.3 |
| chebyshev_sigmoid_d7 | archive | 2 | #5 (k2) | x1.575 | 116.7 | 142.8 | 7.74e-08 | 17 | 6 | 6 | 4 | 2 | 0 | 6 | 7 | 0.53 | 7 / 8 | 0 | - | 65041 | 59237 | 888.4 |
| chebyshev_sigmoid_d7 | archive | 3 | #5 (k3) | x1.379 | 133.2 | 159.7 | 7.76e-08 | 13 | 5 | 5 | 4 | 3 | 1 | 8 | 3 | 0.51 | 7 / 8 | 1 | - | 66043 | 60005 | 932.1 |

## Per task and strategy (3 runs each)

| task | strategy | runs | speedup |
|---|---|---:|---|
| chebyshev_elu_d7 | archive | 3 | median x1.268, mean x1.292, min x1.248, max x1.360, sd 0.059 |
| chebyshev_gelu_d7 | archive | 3 | median x1.596, mean x1.933, min x1.578, max x2.626, sd 0.600 |
| chebyshev_sigmoid_d7 | archive | 3 | median x1.575, mean x1.833, min x1.379, max x2.546, sd 0.625 |

## Across tasks (geometric mean of the per-task mean speedups)

| strategy | tasks | geometric mean |
|---|---:|---:|
| archive | 3 | x1.661 |

## Approximation error (task property, separate from FHE error)

| task | max_fit_error (fitting grid) | approx. error, fixed points | approx. error, dense points |
|---|---:|---:|---:|
| chebyshev_elu_d7 | 0.0427493 | 0.0427493 | 0.0330832 |
| chebyshev_gelu_d7 | 0.0581538 | 0.0581538 | 0.0470933 |
| chebyshev_sigmoid_d7 | 0.002824 | 0.002824 | 0.0021268 |

## Re-scored kernels

| task | kernel | used by | correct | eval (ms) | speedup |
|---|---|---|---|---:|---:|
| chebyshev_gelu_d7 | baseline | baseline | True | 172.87 | 1.0 |
| chebyshev_gelu_d7 | k1 | archive/run1 | True | 65.83 | 2.626 |
| chebyshev_gelu_d7 | k2 | archive/run2 | True | 109.53 | 1.578 |
| chebyshev_gelu_d7 | k3 | archive/run3 | True | 108.34 | 1.596 |
| chebyshev_sigmoid_d7 | baseline | baseline | True | 183.78 | 1.0 |
| chebyshev_sigmoid_d7 | k1 | archive/run1 | True | 72.18 | 2.546 |
| chebyshev_sigmoid_d7 | k2 | archive/run2 | True | 116.67 | 1.575 |
| chebyshev_sigmoid_d7 | k3 | archive/run3 | True | 133.24 | 1.379 |
| chebyshev_elu_d7 | baseline | baseline | True | 205.84 | 1.0 |
| chebyshev_elu_d7 | k1 | archive/run1 | True | 162.37 | 1.268 |
| chebyshev_elu_d7 | k2 | archive/run2 | True | 151.39 | 1.36 |
| chebyshev_elu_d7 | k3 | archive/run3 | True | 164.88 | 1.248 |
