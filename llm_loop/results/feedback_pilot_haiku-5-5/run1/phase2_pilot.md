# Phase-2 pilot: neutral re-scoring

Commit 67c8d41bb9 (clean), Darwin arm64 arm, Apple clang version 21.0.0 (clang-2100.1.1.101), SEAL 4.1. Re-scored 2026-11-22T07:17:00.

Speedup = baseline eval-only median / kernel eval-only median, both from the same neutral session (1 warmup + 10 measured processes of ./main --repeats 10, interleaved). The final kernel of each run is the strategy's own selection; re-scoring never re-selects. E2E = CHEHAB execution_time_(ms) (parse + encode + encrypt + fhe()). Compile time: median of 3, reported only. CKKS chain: sized. Repeated correctness checks per case: 20; strict speedup counts a final kernel that fails any check as x1.000.

## Every run

| task | strategy | run | final | speedup | strict | repeats passed | eval (ms) | e2e (ms) | FHE err | depth | xdepth | levels | ct x ct | ct x pt | align | rescale | mod sw | C (ms) | correct / 8 | dup | failures | out tok | think tok | search (s) |
|---|---|---:|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---|---:|---:|---:|
| chebyshev_elu_d7 | archive | 1 | #6 (k1) | x2.577 | x2.577 | 160/160 | 53.6 | 66.9 | 2.75e-07 | 11 | 4 | 4 | 5 | 5 | 3 | 13 | 0 | 0.50 | 4 / 8 | 0 | chehab 2 | 124078 | 115447 | 751.3 |
| chebyshev_elu_d7 | archive-explore | 1 | #5 (k2) | x2.581 | x2.581 | 160/160 | 53.6 | 67.0 | 1.91e-07 | 10 | 4 | 4 | 5 | 5 | 3 | 13 | 0 | 0.50 | 3 / 8 | 0 | chehab 1 | 108070 | 99138 | 640.9 |
| chebyshev_elu_d7 | independent | 1 | #7 (k3) | x2.014 | x2.014 | 160/160 | 68.6 | 84.1 | 2.30e-07 | 13 | 5 | 5 | 4 | 9 | 5 | 18 | 3 | 0.50 | 4 / 8 | 0 | chehab 4 | 69435 | 61858 | 475.7 |
| chebyshev_elu_d7 | linear | 1 | #3 (k4) | x2.009 | x2.009 | 160/160 | 68.8 | 84.3 | 1.85e-07 | 14 | 5 | 5 | 4 | 9 | 5 | 18 | 4 | 0.50 | 7 / 8 | 1 | - | 62750 | 53593 | 470.6 |
| chebyshev_elu_d7 | linear-explore | 1 | #6 (k5) | x2.009 | x2.009 | 160/160 | 68.8 | 84.5 | 1.63e-07 | 13 | 5 | 5 | 4 | 9 | 5 | 18 | 2 | 0.51 | 4 / 8 | 1 | chehab 3 | 67877 | 58291 | 528.9 |
| chebyshev_elu_d7 | linear-minimal | 1 | #0 (baseline) | x1.000 | x1.000 | 160/160 | 138.2 | 160.7 | 1.45e-07 | 22 | 8 | 8 | 6 | 8 | 11 | 25 | 10 | 0.59 | 4 / 8 | 2 | chehab 2 | 60820 | 54839 | 464.1 |
| chebyshev_gelu_d7 | archive | 1 | #2 (k1) | x2.938 | x2.938 | 160/160 | 32.1 | 44.7 | 2.54e-07 | 11 | 4 | 4 | 3 | 4 | 2 | 9 | 1 | 0.43 | 6 / 8 | 2 | - | 68906 | 62825 | 508.9 |
| chebyshev_gelu_d7 | archive-explore | 1 | #4 (k2) | x2.882 | x2.882 | 160/160 | 32.7 | 45.3 | 2.24e-07 | 11 | 4 | 4 | 3 | 4 | 2 | 9 | 0 | 0.43 | 5 / 8 | 0 | chehab 1 | 107042 | 99931 | 736.2 |
| chebyshev_gelu_d7 | independent | 1 | #2 (k3) | x2.896 | x2.896 | 160/160 | 32.6 | 45.1 | 1.95e-07 | 11 | 4 | 4 | 3 | 4 | 2 | 9 | 1 | 0.47 | 2 / 8 | 0 | chehab 2 | 42478 | 37157 | 305.7 |
| chebyshev_gelu_d7 | linear | 1 | #3 (k4) | x2.889 | x2.889 | 160/160 | 32.7 | 45.3 | 2.29e-07 | 9 | 4 | 4 | 3 | 4 | 2 | 9 | 0 | 0.41 | 6 / 8 | 2 | - | 48577 | 43554 | 394.8 |
| chebyshev_gelu_d7 | linear-explore | 1 | #3 (k5) | x2.908 | x2.908 | 160/160 | 32.4 | 45.0 | 2.16e-07 | 11 | 4 | 4 | 3 | 4 | 2 | 9 | 1 | 0.43 | 6 / 8 | 0 | compile 1, chehab 1 | 72122 | 65399 | 546.5 |
| chebyshev_gelu_d7 | linear-minimal | 1 | #3 (k6) | x2.889 | x2.889 | 160/160 | 32.7 | 45.3 | 2.11e-07 | 9 | 4 | 4 | 3 | 4 | 2 | 9 | 0 | 0.48 | 4 / 8 | 3 | chehab 1 | 42922 | 38208 | 351.5 |
| chebyshev_sigmoid_d7 | archive | 1 | #3 (k1) | x2.323 | x2.323 | 160/160 | 52.7 | 67.8 | 1.30e-07 | 15 | 5 | 5 | 4 | 5 | 1 | 10 | 5 | 0.46 | 6 / 8 | 2 | - | 198206 | 192062 | 1239.4 |
| chebyshev_sigmoid_d7 | archive-explore | 1 | #3 (k2) | x2.328 | x2.328 | 160/160 | 52.6 | 67.5 | 9.48e-08 | 13 | 5 | 5 | 4 | 5 | 4 | 13 | 7 | 0.51 | 7 / 8 | 0 | chehab 1 | 120120 | 114034 | 983.2 |
| chebyshev_sigmoid_d7 | independent | 1 | #3 (k3) | x2.013 | x2.013 | 160/160 | 60.8 | 75.8 | 1.08e-07 | 13 | 5 | 5 | 4 | 7 | 4 | 15 | 5 | 0.55 | 2 / 8 | 0 | chehab 6 | 61927 | 55443 | 345.7 |
| chebyshev_sigmoid_d7 | linear | 1 | #4 (k4) | x1.644 | x1.644 | 160/160 | 74.4 | 92.0 | 1.03e-07 | 20 | 6 | 6 | 4 | 5 | 9 | 18 | 11 | 0.54 | 6 / 8 | 1 | chehab 1 | 43947 | 37090 | 402.2 |
| chebyshev_sigmoid_d7 | linear-explore | 1 | #7 (k5) | x2.512 | x2.512 | 160/160 | 48.7 | 61.9 | 2.00e-07 | 10 | 4 | 4 | 5 | 5 | 3 | 13 | 0 | 0.50 | 7 / 8 | 0 | chehab 1 | 119888 | 110548 | 929.5 |
| chebyshev_sigmoid_d7 | linear-minimal | 1 | #3 (k6) | x2.711 | x2.711 | 160/160 | 45.1 | 59.9 | 8.99e-08 | 14 | 5 | 5 | 4 | 3 | 1 | 8 | 3 | 0.47 | 7 / 8 | 1 | - | 112205 | 106781 | 760.1 |
| coyote_tree_100_50_5_1 | archive | 1 | #0 (baseline) | x1.000 | x1.000 | 16/16 | 323.2 | 479.6 | 0.00e+00 | 5 | 3 |  | 12 | 0 | 0 | 0 | 0 | 0.30 | 5 / 8 | 3 | - | 39392 | 31263 | 351.4 |
| coyote_tree_100_50_5_1 | archive-explore | 1 | #0 (baseline) | x1.000 | x1.000 | 16/16 | 323.2 | 479.6 | 0.00e+00 | 5 | 3 |  | 12 | 0 | 0 | 0 | 0 | 0.30 | 8 / 8 | 0 | - | 53503 | 43550 | 567.6 |
| coyote_tree_100_50_5_1 | independent | 1 | #0 (baseline) | x1.000 | x1.000 | 16/16 | 323.2 | 479.6 | 0.00e+00 | 5 | 3 |  | 12 | 0 | 0 | 0 | 0 | 0.30 | 8 / 8 | 0 | - | 49245 | 40832 | 561.0 |
| coyote_tree_100_50_5_1 | linear | 1 | #0 (baseline) | x1.000 | x1.000 | 16/16 | 323.2 | 479.6 | 0.00e+00 | 5 | 3 |  | 12 | 0 | 0 | 0 | 0 | 0.30 | 3 / 8 | 5 | - | 30241 | 22305 | 270.5 |
| coyote_tree_100_50_5_1 | linear-explore | 1 | #0 (baseline) | x1.000 | x1.000 | 16/16 | 323.2 | 479.6 | 0.00e+00 | 5 | 3 |  | 12 | 0 | 0 | 0 | 0 | 0.30 | 7 / 8 | 1 | - | 35659 | 27073 | 411.7 |
| coyote_tree_100_50_5_1 | linear-minimal | 1 | #0 (baseline) | x1.000 | x1.000 | 16/16 | 323.2 | 479.6 | 0.00e+00 | 5 | 3 |  | 12 | 0 | 0 | 0 | 0 | 0.30 | 3 / 8 | 5 | - | 42441 | 33784 | 351.9 |
| extract_sparse | archive | 1 | #2 (k1) | x1.287 | x1.287 | 160/160 | 6.4 | 19.5 | 7.57e-06 | 4 | 1 | 1 | 0 | 2 | 1 | 3 | 0 | 0.32 | 7 / 8 | 1 | - | 75239 | 70705 | 488.4 |
| extract_sparse | archive-explore | 1 | #2 (k2) | x1.290 | x1.290 | 160/160 | 6.3 | 19.5 | 7.29e-06 | 3 | 1 | 1 | 0 | 2 | 1 | 3 | 0 | 0.36 | 6 / 8 | 1 | chehab 1 | 69189 | 64311 | 444.0 |
| extract_sparse | independent | 1 | #1 (k3) | x1.283 | x1.283 | 160/160 | 6.4 | 19.7 | 6.06e-06 | 4 | 1 | 1 | 0 | 2 | 1 | 3 | 0 | 0.30 | 8 / 8 | 0 | - | 38271 | 33890 | 314.3 |
| extract_sparse | linear | 1 | #5 (k4) | x1.283 | x1.283 | 160/160 | 6.4 | 19.6 | 8.60e-06 | 4 | 1 | 1 | 0 | 2 | 1 | 3 | 0 | 0.34 | 5 / 8 | 3 | - | 35674 | 31733 | 249.5 |
| extract_sparse | linear-explore | 1 | #2 (k5) | x1.290 | x1.290 | 160/160 | 6.3 | 19.6 | 5.65e-06 | 4 | 1 | 1 | 0 | 2 | 1 | 3 | 0 | 0.32 | 5 / 8 | 1 | chehab 1 | 60332 | 55882 | 426.4 |
| extract_sparse | linear-minimal | 1 | #2 (k6) | x1.287 | x1.287 | 160/160 | 6.4 | 19.6 | 1.10e-05 | 4 | 1 | 1 | 0 | 2 | 1 | 3 | 0 | 0.29 | 7 / 8 | 1 | - | 33538 | 29692 | 276.6 |
| matmul_2x2 | archive | 1 | #3 (k1) | x1.565 | x1.565 | 16/16 | 137.1 | 178.5 | 0.00e+00 | 10 | 3 |  | 4 | 4 | 0 | 0 | 0 | 0.28 | 6 / 8 | 2 | - | 154284 | 147552 | 1032.6 |
| matmul_2x2 | archive-explore | 1 | #8 (k2) | x1.349 | x1.349 | 16/16 | 159.1 | 200.5 | 0.00e+00 | 12 | 3 |  | 4 | 8 | 0 | 0 | 0 | 0.41 | 6 / 8 | 0 | wrong 2 | 113842 | 105389 | 892.3 |
| matmul_2x2 | independent | 1 | #5 (k3) | x1.407 | x1.407 | 16/16 | 152.5 | 193.8 | 0.00e+00 | 6 | 2 |  | 4 | 4 | 0 | 0 | 0 | 0.28 | 5 / 8 | 0 | wrong 3 | 53602 | 46461 | 427.9 |
| matmul_2x2 | linear | 1 | #5 (k4) | x1.569 | x1.569 | 16/16 | 136.8 | 178.3 | 0.00e+00 | 9 | 3 |  | 4 | 4 | 0 | 0 | 0 | 0.31 | 4 / 8 | 3 | wrong 1 | 63438 | 57049 | 436.1 |
| matmul_2x2 | linear-explore | 1 | #5 (k5) | x1.564 | x1.564 | 16/16 | 137.2 | 178.5 | 0.00e+00 | 10 | 3 |  | 4 | 4 | 0 | 0 | 0 | 0.27 | 8 / 8 | 0 | - | 118879 | 112018 | 828.3 |
| matmul_2x2 | linear-minimal | 1 | #2 (k6) | x1.406 | x1.406 | 16/16 | 152.6 | 193.8 | 0.00e+00 | 6 | 2 |  | 4 | 4 | 0 | 0 | 0 | 0.30 | 4 / 8 | 4 | - | 83623 | 75715 | 568.2 |

## Per task and strategy (strict speedup)

| task | strategy | runs | speedup |
|---|---|---:|---|
| chebyshev_elu_d7 | archive | 1 | median x2.577, mean x2.577, min x2.577, max x2.577, sd 0.000 |
| chebyshev_gelu_d7 | archive | 1 | median x2.938, mean x2.938, min x2.938, max x2.938, sd 0.000 |
| chebyshev_sigmoid_d7 | archive | 1 | median x2.323, mean x2.323, min x2.323, max x2.323, sd 0.000 |
| coyote_tree_100_50_5_1 | archive | 1 | median x1.000, mean x1.000, min x1.000, max x1.000, sd 0.000 |
| extract_sparse | archive | 1 | median x1.287, mean x1.287, min x1.287, max x1.287, sd 0.000 |
| matmul_2x2 | archive | 1 | median x1.565, mean x1.565, min x1.565, max x1.565, sd 0.000 |
| chebyshev_elu_d7 | archive-explore | 1 | median x2.581, mean x2.581, min x2.581, max x2.581, sd 0.000 |
| chebyshev_gelu_d7 | archive-explore | 1 | median x2.882, mean x2.882, min x2.882, max x2.882, sd 0.000 |
| chebyshev_sigmoid_d7 | archive-explore | 1 | median x2.328, mean x2.328, min x2.328, max x2.328, sd 0.000 |
| coyote_tree_100_50_5_1 | archive-explore | 1 | median x1.000, mean x1.000, min x1.000, max x1.000, sd 0.000 |
| extract_sparse | archive-explore | 1 | median x1.290, mean x1.290, min x1.290, max x1.290, sd 0.000 |
| matmul_2x2 | archive-explore | 1 | median x1.349, mean x1.349, min x1.349, max x1.349, sd 0.000 |
| chebyshev_elu_d7 | independent | 1 | median x2.014, mean x2.014, min x2.014, max x2.014, sd 0.000 |
| chebyshev_gelu_d7 | independent | 1 | median x2.896, mean x2.896, min x2.896, max x2.896, sd 0.000 |
| chebyshev_sigmoid_d7 | independent | 1 | median x2.013, mean x2.013, min x2.013, max x2.013, sd 0.000 |
| coyote_tree_100_50_5_1 | independent | 1 | median x1.000, mean x1.000, min x1.000, max x1.000, sd 0.000 |
| extract_sparse | independent | 1 | median x1.283, mean x1.283, min x1.283, max x1.283, sd 0.000 |
| matmul_2x2 | independent | 1 | median x1.407, mean x1.407, min x1.407, max x1.407, sd 0.000 |
| chebyshev_elu_d7 | linear | 1 | median x2.009, mean x2.009, min x2.009, max x2.009, sd 0.000 |
| chebyshev_gelu_d7 | linear | 1 | median x2.889, mean x2.889, min x2.889, max x2.889, sd 0.000 |
| chebyshev_sigmoid_d7 | linear | 1 | median x1.644, mean x1.644, min x1.644, max x1.644, sd 0.000 |
| coyote_tree_100_50_5_1 | linear | 1 | median x1.000, mean x1.000, min x1.000, max x1.000, sd 0.000 |
| extract_sparse | linear | 1 | median x1.283, mean x1.283, min x1.283, max x1.283, sd 0.000 |
| matmul_2x2 | linear | 1 | median x1.569, mean x1.569, min x1.569, max x1.569, sd 0.000 |
| chebyshev_elu_d7 | linear-explore | 1 | median x2.009, mean x2.009, min x2.009, max x2.009, sd 0.000 |
| chebyshev_gelu_d7 | linear-explore | 1 | median x2.908, mean x2.908, min x2.908, max x2.908, sd 0.000 |
| chebyshev_sigmoid_d7 | linear-explore | 1 | median x2.512, mean x2.512, min x2.512, max x2.512, sd 0.000 |
| coyote_tree_100_50_5_1 | linear-explore | 1 | median x1.000, mean x1.000, min x1.000, max x1.000, sd 0.000 |
| extract_sparse | linear-explore | 1 | median x1.290, mean x1.290, min x1.290, max x1.290, sd 0.000 |
| matmul_2x2 | linear-explore | 1 | median x1.564, mean x1.564, min x1.564, max x1.564, sd 0.000 |
| chebyshev_elu_d7 | linear-minimal | 1 | median x1.000, mean x1.000, min x1.000, max x1.000, sd 0.000 |
| chebyshev_gelu_d7 | linear-minimal | 1 | median x2.889, mean x2.889, min x2.889, max x2.889, sd 0.000 |
| chebyshev_sigmoid_d7 | linear-minimal | 1 | median x2.711, mean x2.711, min x2.711, max x2.711, sd 0.000 |
| coyote_tree_100_50_5_1 | linear-minimal | 1 | median x1.000, mean x1.000, min x1.000, max x1.000, sd 0.000 |
| extract_sparse | linear-minimal | 1 | median x1.287, mean x1.287, min x1.287, max x1.287, sd 0.000 |
| matmul_2x2 | linear-minimal | 1 | median x1.406, mean x1.406, min x1.406, max x1.406, sd 0.000 |

## Across tasks (geometric mean of the per-task mean strict speedups)

| strategy | tasks | geometric mean |
|---|---:|---:|
| archive | 6 | x1.812 |
| archive-explore | 6 | x1.764 |
| independent | 6 | x1.664 |
| linear | 6 | x1.636 |
| linear-explore | 6 | x1.759 |
| linear-minimal | 6 | x1.556 |

## Approximation error (task property, separate from FHE error)

| task | max_fit_error (fitting grid) | approx. error, fixed points | approx. error, dense points |
|---|---:|---:|---:|
| chebyshev_elu_d7 | 0.0427493 | 0.0427493 | 0.0330832 |
| chebyshev_gelu_d7 | 0.0581538 | 0.0581538 | 0.0470933 |
| chebyshev_sigmoid_d7 | 0.002824 | 0.002824 | 0.0021268 |
| coyote_tree_100_50_5_1 | - | - | - |
| extract_sparse | - | - | - |
| matmul_2x2 | - | - | - |

## Re-scored kernels

| task | kernel | used by | correct | repeats passed | worst repeat error | eval (ms) | speedup |
|---|---|---|---|---:|---:|---:|---:|
| chebyshev_elu_d7 | baseline | linear-minimal/run1 | True | 160/160 | 2.49e-07 | 138.2 | 1.0 |
| chebyshev_elu_d7 | k1 | archive/run1 | True | 160/160 | 3.36e-07 | 53.63 | 2.577 |
| chebyshev_elu_d7 | k2 | archive-explore/run1 | True | 160/160 | 3.54e-07 | 53.55 | 2.581 |
| chebyshev_elu_d7 | k3 | independent/run1 | True | 160/160 | 2.29e-07 | 68.62 | 2.014 |
| chebyshev_elu_d7 | k4 | linear/run1 | True | 160/160 | 2.20e-07 | 68.79 | 2.009 |
| chebyshev_elu_d7 | k5 | linear-explore/run1 | True | 160/160 | 2.49e-07 | 68.78 | 2.009 |
| chebyshev_gelu_d7 | baseline | baseline | True | 160/160 | 4.06e-07 | 94.36 | 1.0 |
| chebyshev_gelu_d7 | k1 | archive/run1 | True | 160/160 | 3.65e-07 | 32.12 | 2.938 |
| chebyshev_gelu_d7 | k2 | archive-explore/run1 | True | 160/160 | 2.60e-07 | 32.74 | 2.882 |
| chebyshev_gelu_d7 | k3 | independent/run1 | True | 160/160 | 2.99e-07 | 32.58 | 2.896 |
| chebyshev_gelu_d7 | k4 | linear/run1 | True | 160/160 | 3.77e-07 | 32.67 | 2.889 |
| chebyshev_gelu_d7 | k5 | linear-explore/run1 | True | 160/160 | 3.31e-07 | 32.45 | 2.908 |
| chebyshev_gelu_d7 | k6 | linear-minimal/run1 | True | 160/160 | 3.15e-07 | 32.66 | 2.889 |
| chebyshev_sigmoid_d7 | baseline | baseline | True | 160/160 | 1.07e-07 | 122.4 | 1.0 |
| chebyshev_sigmoid_d7 | k1 | archive/run1 | True | 160/160 | 1.67e-07 | 52.68 | 2.323 |
| chebyshev_sigmoid_d7 | k2 | archive-explore/run1 | True | 160/160 | 1.22e-07 | 52.57 | 2.328 |
| chebyshev_sigmoid_d7 | k3 | independent/run1 | True | 160/160 | 1.35e-07 | 60.81 | 2.013 |
| chebyshev_sigmoid_d7 | k4 | linear/run1 | True | 160/160 | 1.35e-07 | 74.44 | 1.644 |
| chebyshev_sigmoid_d7 | k5 | linear-explore/run1 | True | 160/160 | 2.88e-07 | 48.73 | 2.512 |
| chebyshev_sigmoid_d7 | k6 | linear-minimal/run1 | True | 160/160 | 1.11e-07 | 45.15 | 2.711 |
| coyote_tree_100_50_5_1 | baseline | archive/run1 archive-explore/run1 independent/run1 linear/run1 linear-explore/run1 linear-minimal/run1 | True | 16/16 | 0.00e+00 | 323.22 | 1.0 |
| extract_sparse | baseline | baseline | True | 160/160 | 2.21e-05 | 8.18 | 1.0 |
| extract_sparse | k1 | archive/run1 | True | 160/160 | 1.14e-05 | 6.36 | 1.287 |
| extract_sparse | k2 | archive-explore/run1 | True | 160/160 | 1.29e-05 | 6.35 | 1.29 |
| extract_sparse | k3 | independent/run1 | True | 160/160 | 9.15e-06 | 6.38 | 1.283 |
| extract_sparse | k4 | linear/run1 | True | 160/160 | 1.18e-05 | 6.38 | 1.283 |
| extract_sparse | k5 | linear-explore/run1 | True | 160/160 | 1.36e-05 | 6.34 | 1.29 |
| extract_sparse | k6 | linear-minimal/run1 | True | 160/160 | 9.83e-06 | 6.36 | 1.287 |
| matmul_2x2 | baseline | baseline | True | 16/16 | 0.00e+00 | 214.6 | 1.0 |
| matmul_2x2 | k1 | archive/run1 | True | 16/16 | 0.00e+00 | 137.14 | 1.565 |
| matmul_2x2 | k2 | archive-explore/run1 | True | 16/16 | 0.00e+00 | 159.08 | 1.349 |
| matmul_2x2 | k3 | independent/run1 | True | 16/16 | 0.00e+00 | 152.55 | 1.407 |
| matmul_2x2 | k4 | linear/run1 | True | 16/16 | 0.00e+00 | 136.8 | 1.569 |
| matmul_2x2 | k5 | linear-explore/run1 | True | 16/16 | 0.00e+00 | 137.22 | 1.564 |
| matmul_2x2 | k6 | linear-minimal/run1 | True | 16/16 | 0.00e+00 | 152.63 | 1.406 |
