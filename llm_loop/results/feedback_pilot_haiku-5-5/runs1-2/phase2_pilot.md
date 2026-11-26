# Phase-2 pilot: neutral re-scoring

Commit 67c8d41bb9 (clean), Darwin arm64 arm, Apple clang version 21.0.0 (clang-2100.1.1.101), SEAL 4.1. Re-scored 2026-11-22T13:47:06.

Speedup = baseline eval-only median / kernel eval-only median, both from the same neutral session (1 warmup + 10 measured processes of ./main --repeats 10, interleaved). The final kernel of each run is the strategy's own selection; re-scoring never re-selects. E2E = CHEHAB execution_time_(ms) (parse + encode + encrypt + fhe()). Compile time: median of 3, reported only. CKKS chain: sized. Repeated correctness checks per case: 20; strict speedup counts a final kernel that fails any check as x1.000.

## Every run

| task | strategy | run | final | speedup | strict | repeats passed | eval (ms) | e2e (ms) | FHE err | depth | xdepth | levels | ct x ct | ct x pt | align | rescale | mod sw | C (ms) | correct / 8 | dup | failures | out tok | think tok | search (s) |
|---|---|---:|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---|---:|---:|---:|
| chebyshev_elu_d7 | archive | 1 | #6 (k1) | x2.578 | x2.578 | 160/160 | 53.6 | 66.8 | 2.72e-07 | 11 | 4 | 4 | 5 | 5 | 3 | 13 | 0 | 0.48 | 4 / 8 | 0 | chehab 2 | 124078 | 115447 | 751.3 |
| chebyshev_elu_d7 | archive | 2 | #5 (k2) | x2.010 | x2.010 | 160/160 | 68.8 | 84.2 | 1.96e-07 | 14 | 5 | 5 | 4 | 9 | 5 | 18 | 4 | 0.53 | 6 / 8 | 1 | chehab 1 | 136022 | 127093 | 1002.2 |
| chebyshev_elu_d7 | archive-explore | 1 | #5 (k3) | x2.580 | x2.580 | 160/160 | 53.6 | 67.1 | 3.48e-07 | 10 | 4 | 4 | 5 | 5 | 3 | 13 | 0 | 0.48 | 3 / 8 | 0 | chehab 1 | 108070 | 99138 | 640.9 |
| chebyshev_elu_d7 | archive-explore | 2 | #5 (k4) | x2.015 | x2.015 | 160/160 | 68.6 | 84.2 | 1.93e-07 | 13 | 5 | 5 | 4 | 9 | 5 | 18 | 2 | 0.51 | 4 / 8 | 1 | chehab 2 | 113031 | 105179 | 583.7 |
| chebyshev_elu_d7 | independent | 1 | #7 (k5) | x2.016 | x2.016 | 160/160 | 68.6 | 84.0 | 1.99e-07 | 13 | 5 | 5 | 4 | 9 | 5 | 18 | 3 | 0.48 | 4 / 8 | 0 | chehab 4 | 69435 | 61858 | 475.7 |
| chebyshev_elu_d7 | independent | 2 | #0 (baseline) | x1.000 | x1.000 | 160/160 | 138.2 | 160.7 | 1.59e-07 | 22 | 8 | 8 | 6 | 8 | 11 | 25 | 10 | 0.58 | 0 / 8 | 0 | chehab 7, wrong 1 | 47351 | 40053 | 223.8 |
| chebyshev_elu_d7 | linear | 1 | #3 (k6) | x2.011 | x2.011 | 160/160 | 68.7 | 84.1 | 2.07e-07 | 14 | 5 | 5 | 4 | 9 | 5 | 18 | 4 | 0.64 | 7 / 8 | 1 | - | 62750 | 53593 | 470.6 |
| chebyshev_elu_d7 | linear | 2 | #3 (k7) | x2.010 | x2.010 | 160/160 | 68.8 | 84.0 | 2.20e-07 | 13 | 5 | 5 | 4 | 9 | 5 | 18 | 2 | 0.54 | 2 / 8 | 4 | chehab 2 | 75620 | 66511 | 467.9 |
| chebyshev_elu_d7 | linear-explore | 1 | #6 (k8) | x2.010 | x2.010 | 160/160 | 68.8 | 84.3 | 2.02e-07 | 13 | 5 | 5 | 4 | 9 | 5 | 18 | 2 | 0.53 | 4 / 8 | 1 | chehab 3 | 67877 | 58291 | 528.9 |
| chebyshev_elu_d7 | linear-explore | 2 | #5 (k9) | x2.433 | x2.433 | 160/160 | 56.8 | 70.3 | 2.74e-07 | 15 | 4 | 4 | 5 | 7 | 4 | 16 | 1 | 0.54 | 5 / 8 | 0 | chehab 3 | 108816 | 100327 | 685.3 |
| chebyshev_elu_d7 | linear-minimal | 1 | #0 (baseline) | x1.000 | x1.000 | 160/160 | 138.2 | 160.7 | 1.59e-07 | 22 | 8 | 8 | 6 | 8 | 11 | 25 | 10 | 0.58 | 4 / 8 | 2 | chehab 2 | 60820 | 54839 | 464.1 |
| chebyshev_elu_d7 | linear-minimal | 2 | #4 (k10) | x1.973 | x1.973 | 160/160 | 70.1 | 85.5 | 1.91e-07 | 13 | 5 | 5 | 4 | 9 | 6 | 19 | 3 | 0.51 | 5 / 8 | 1 | chehab 2 | 76078 | 68096 | 538.5 |
| chebyshev_gelu_d7 | archive | 1 | #2 (k1) | x2.938 | x2.938 | 160/160 | 32.1 | 44.9 | 2.69e-07 | 11 | 4 | 4 | 3 | 4 | 2 | 9 | 1 | 0.44 | 6 / 8 | 2 | - | 68906 | 62825 | 508.9 |
| chebyshev_gelu_d7 | archive | 2 | #2 (k2) | x2.907 | x2.907 | 160/160 | 32.5 | 45.2 | 2.01e-07 | 10 | 4 | 4 | 3 | 4 | 2 | 9 | 0 | 0.42 | 6 / 8 | 2 | - | 54438 | 48677 | 366.0 |
| chebyshev_gelu_d7 | archive-explore | 1 | #4 (k3) | x2.885 | x2.885 | 160/160 | 32.7 | 45.5 | 2.11e-07 | 11 | 4 | 4 | 3 | 4 | 2 | 9 | 0 | 0.43 | 5 / 8 | 0 | chehab 1 | 107042 | 99931 | 736.2 |
| chebyshev_gelu_d7 | archive-explore | 2 | #6 (k4) | x2.942 | x2.942 | 160/160 | 32.1 | 44.8 | 2.36e-07 | 11 | 4 | 4 | 3 | 4 | 2 | 9 | 0 | 0.42 | 6 / 8 | 0 | chehab 1 | 94607 | 88167 | 554.5 |
| chebyshev_gelu_d7 | independent | 1 | #2 (k5) | x2.898 | x2.898 | 160/160 | 32.6 | 45.2 | 1.50e-07 | 11 | 4 | 4 | 3 | 4 | 2 | 9 | 1 | 0.46 | 2 / 8 | 0 | chehab 2 | 42478 | 37157 | 305.7 |
| chebyshev_gelu_d7 | independent | 2 | #2 (k6) | x2.908 | x2.908 | 160/160 | 32.5 | 45.1 | 2.58e-07 | 11 | 4 | 4 | 3 | 4 | 2 | 9 | 1 | 0.46 | 2 / 8 | 0 | chehab 1 | 38603 | 33673 | 220.5 |
| chebyshev_gelu_d7 | linear | 1 | #3 (k7) | x2.888 | x2.888 | 160/160 | 32.7 | 45.5 | 2.34e-07 | 9 | 4 | 4 | 3 | 4 | 2 | 9 | 0 | 0.42 | 6 / 8 | 2 | - | 48577 | 43554 | 394.8 |
| chebyshev_gelu_d7 | linear | 2 | #3 (k8) | x2.920 | x2.920 | 160/160 | 32.3 | 45.2 | 1.80e-07 | 13 | 4 | 4 | 3 | 4 | 2 | 9 | 1 | 0.42 | 5 / 8 | 3 | - | 55951 | 50822 | 421.0 |
| chebyshev_gelu_d7 | linear-explore | 1 | #3 (k9) | x2.907 | x2.907 | 160/160 | 32.5 | 45.2 | 2.88e-07 | 11 | 4 | 4 | 3 | 4 | 2 | 9 | 1 | 0.45 | 6 / 8 | 0 | compile 1, chehab 1 | 72122 | 65399 | 546.5 |
| chebyshev_gelu_d7 | linear-explore | 2 | #3 (k10) | x2.888 | x2.888 | 160/160 | 32.7 | 45.5 | 1.88e-07 | 9 | 4 | 4 | 3 | 4 | 2 | 9 | 0 | 0.46 | 5 / 8 | 0 | chehab 3 | 93733 | 85596 | 699.1 |
| chebyshev_gelu_d7 | linear-minimal | 1 | #3 (k11) | x2.888 | x2.888 | 160/160 | 32.7 | 45.5 | 2.71e-07 | 9 | 4 | 4 | 3 | 4 | 2 | 9 | 0 | 0.49 | 4 / 8 | 3 | chehab 1 | 42922 | 38208 | 351.5 |
| chebyshev_gelu_d7 | linear-minimal | 2 | #5 (k12) | x2.906 | x2.906 | 160/160 | 32.5 | 45.2 | 1.85e-07 | 10 | 4 | 4 | 3 | 4 | 2 | 9 | 0 | 0.50 | 7 / 8 | 1 | - | 46513 | 40741 | 382.9 |
| chebyshev_sigmoid_d7 | archive | 1 | #3 (k1) | x2.324 | x2.324 | 160/160 | 52.7 | 67.9 | 1.24e-07 | 15 | 5 | 5 | 4 | 5 | 1 | 10 | 5 | 0.48 | 6 / 8 | 2 | - | 198206 | 192062 | 1239.4 |
| chebyshev_sigmoid_d7 | archive | 2 | #8 (k2) | x2.576 | x2.576 | 160/160 | 47.6 | 64.3 | 1.02e-07 | 17 | 6 | 6 | 4 | 2 | 0 | 6 | 7 | 0.42 | 7 / 8 | 0 | chehab 1 | 100804 | 95087 | 720.6 |
| chebyshev_sigmoid_d7 | archive-explore | 1 | #3 (k3) | x2.329 | x2.329 | 160/160 | 52.6 | 67.6 | 9.13e-08 | 13 | 5 | 5 | 4 | 5 | 4 | 13 | 7 | 0.54 | 7 / 8 | 0 | chehab 1 | 120120 | 114034 | 983.2 |
| chebyshev_sigmoid_d7 | archive-explore | 2 | #0 (baseline) | x1.000 | x1.000 | 160/160 | 122.5 | 144.3 | 7.88e-08 | 22 | 8 | 8 | 6 | 5 | 8 | 19 | 13 | 0.55 | 0 / 8 | 1 | chehab 6, wrong 1 | 53243 | 45878 | 254.5 |
| chebyshev_sigmoid_d7 | independent | 1 | #3 (k4) | x2.013 | x2.013 | 160/160 | 60.9 | 76.1 | 1.25e-07 | 13 | 5 | 5 | 4 | 7 | 4 | 15 | 5 | 0.52 | 2 / 8 | 0 | chehab 6 | 61927 | 55443 | 345.7 |
| chebyshev_sigmoid_d7 | independent | 2 | #2 (k5) | x1.798 | x1.798 | 160/160 | 68.1 | 85.4 | 8.99e-08 | 16 | 6 | 6 | 4 | 5 | 6 | 15 | 8 | 0.45 | 1 / 8 | 0 | chehab 7 | 52407 | 45842 | 268.2 |
| chebyshev_sigmoid_d7 | linear | 1 | #4 (k6) | x1.644 | x1.644 | 160/160 | 74.5 | 92.1 | 8.25e-08 | 20 | 6 | 6 | 4 | 5 | 9 | 18 | 11 | 0.51 | 6 / 8 | 1 | chehab 1 | 43947 | 37090 | 402.2 |
| chebyshev_sigmoid_d7 | linear | 2 | #7 (k7) | x2.460 | x2.460 | 160/160 | 49.8 | 64.6 | 8.80e-08 | 15 | 5 | 5 | 4 | 4 | 1 | 9 | 3 | 0.50 | 4 / 8 | 1 | chehab 2, wrong 1 | 74457 | 67170 | 528.2 |
| chebyshev_sigmoid_d7 | linear-explore | 1 | #7 (k8) | x2.509 | x2.509 | 160/160 | 48.8 | 62.0 | 2.32e-07 | 10 | 4 | 4 | 5 | 5 | 3 | 13 | 0 | 0.48 | 7 / 8 | 0 | chehab 1 | 119888 | 110548 | 929.5 |
| chebyshev_sigmoid_d7 | linear-explore | 2 | #6 (k9) | x2.515 | x2.515 | 160/160 | 48.7 | 62.1 | 2.02e-07 | 11 | 4 | 4 | 5 | 5 | 3 | 13 | 1 | 0.50 | 5 / 8 | 0 | chehab 2 | 157119 | 148356 | 1017.2 |
| chebyshev_sigmoid_d7 | linear-minimal | 1 | #3 (k10) | x2.713 | x2.713 | 160/160 | 45.2 | 59.9 | 1.10e-07 | 14 | 5 | 5 | 4 | 3 | 1 | 8 | 3 | 0.46 | 7 / 8 | 1 | - | 112205 | 106781 | 760.1 |
| chebyshev_sigmoid_d7 | linear-minimal | 2 | #2 (k11) | x2.441 | x2.441 | 160/160 | 50.2 | 63.5 | 2.08e-07 | 13 | 4 | 4 | 5 | 5 | 4 | 14 | 1 | 0.49 | 5 / 8 | 1 | chehab 1, wrong 1 | 95466 | 87570 | 653.1 |
| coyote_tree_100_50_5_1 | archive | 1 | #0 (baseline) | x1.000 | x1.000 | 16/16 | 320.5 | 475.4 | 0.00e+00 | 5 | 3 |  | 12 | 0 | 0 | 0 | 0 | 0.32 | 5 / 8 | 3 | - | 39392 | 31263 | 351.4 |
| coyote_tree_100_50_5_1 | archive | 2 | #0 (baseline) | x1.000 | x1.000 | 16/16 | 320.5 | 475.4 | 0.00e+00 | 5 | 3 |  | 12 | 0 | 0 | 0 | 0 | 0.32 | 6 / 8 | 2 | - | 38251 | 29224 | 411.5 |
| coyote_tree_100_50_5_1 | archive-explore | 1 | #0 (baseline) | x1.000 | x1.000 | 16/16 | 320.5 | 475.4 | 0.00e+00 | 5 | 3 |  | 12 | 0 | 0 | 0 | 0 | 0.32 | 8 / 8 | 0 | - | 53503 | 43550 | 567.6 |
| coyote_tree_100_50_5_1 | archive-explore | 2 | #0 (baseline) | x1.000 | x1.000 | 16/16 | 320.5 | 475.4 | 0.00e+00 | 5 | 3 |  | 12 | 0 | 0 | 0 | 0 | 0.32 | 8 / 8 | 0 | - | 40282 | 29985 | 440.8 |
| coyote_tree_100_50_5_1 | independent | 1 | #0 (baseline) | x1.000 | x1.000 | 16/16 | 320.5 | 475.4 | 0.00e+00 | 5 | 3 |  | 12 | 0 | 0 | 0 | 0 | 0.32 | 8 / 8 | 0 | - | 49245 | 40832 | 561.0 |
| coyote_tree_100_50_5_1 | independent | 2 | #0 (baseline) | x1.000 | x1.000 | 16/16 | 320.5 | 475.4 | 0.00e+00 | 5 | 3 |  | 12 | 0 | 0 | 0 | 0 | 0.32 | 8 / 8 | 0 | - | 43276 | 34935 | 434.4 |
| coyote_tree_100_50_5_1 | linear | 1 | #0 (baseline) | x1.000 | x1.000 | 16/16 | 320.5 | 475.4 | 0.00e+00 | 5 | 3 |  | 12 | 0 | 0 | 0 | 0 | 0.32 | 3 / 8 | 5 | - | 30241 | 22305 | 270.5 |
| coyote_tree_100_50_5_1 | linear | 2 | #0 (baseline) | x1.000 | x1.000 | 16/16 | 320.5 | 475.4 | 0.00e+00 | 5 | 3 |  | 12 | 0 | 0 | 0 | 0 | 0.32 | 1 / 8 | 7 | - | 32531 | 25351 | 239.1 |
| coyote_tree_100_50_5_1 | linear-explore | 1 | #0 (baseline) | x1.000 | x1.000 | 16/16 | 320.5 | 475.4 | 0.00e+00 | 5 | 3 |  | 12 | 0 | 0 | 0 | 0 | 0.32 | 7 / 8 | 1 | - | 35659 | 27073 | 411.7 |
| coyote_tree_100_50_5_1 | linear-explore | 2 | #0 (baseline) | x1.000 | x1.000 | 16/16 | 320.5 | 475.4 | 0.00e+00 | 5 | 3 |  | 12 | 0 | 0 | 0 | 0 | 0.32 | 8 / 8 | 0 | - | 41213 | 31763 | 502.8 |
| coyote_tree_100_50_5_1 | linear-minimal | 1 | #0 (baseline) | x1.000 | x1.000 | 16/16 | 320.5 | 475.4 | 0.00e+00 | 5 | 3 |  | 12 | 0 | 0 | 0 | 0 | 0.32 | 3 / 8 | 5 | - | 42441 | 33784 | 351.9 |
| coyote_tree_100_50_5_1 | linear-minimal | 2 | #0 (baseline) | x1.000 | x1.000 | 16/16 | 320.5 | 475.4 | 0.00e+00 | 5 | 3 |  | 12 | 0 | 0 | 0 | 0 | 0.32 | 4 / 8 | 4 | - | 42703 | 34914 | 372.8 |
| extract_sparse | archive | 1 | #2 (k1) | x1.286 | x1.286 | 160/160 | 6.7 | 21.1 | 8.93e-06 | 4 | 1 | 1 | 0 | 2 | 1 | 3 | 0 | 0.28 | 7 / 8 | 1 | - | 75239 | 70705 | 488.4 |
| extract_sparse | archive | 2 | #1 (k2) | x1.300 | x1.300 | 160/160 | 6.6 | 21.0 | 8.00e-06 | 4 | 1 | 1 | 0 | 2 | 1 | 3 | 0 | 0.31 | 7 / 8 | 1 | - | 68030 | 63310 | 496.4 |
| extract_sparse | archive-explore | 1 | #2 (k3) | x1.297 | x1.297 | 160/160 | 6.7 | 20.7 | 1.02e-05 | 3 | 1 | 1 | 0 | 2 | 1 | 3 | 0 | 0.29 | 6 / 8 | 1 | chehab 1 | 69189 | 64311 | 444.0 |
| extract_sparse | archive-explore | 2 | #2 (k4) | x1.287 | x1.287 | 160/160 | 6.7 | 20.7 | 7.60e-06 | 4 | 1 | 1 | 0 | 2 | 1 | 3 | 0 | 0.28 | 7 / 8 | 0 | chehab 1 | 59674 | 54886 | 366.4 |
| extract_sparse | independent | 1 | #1 (k5) | x1.285 | x1.285 | 160/160 | 6.7 | 21.1 | 5.83e-06 | 4 | 1 | 1 | 0 | 2 | 1 | 3 | 0 | 0.27 | 8 / 8 | 0 | - | 38271 | 33890 | 314.3 |
| extract_sparse | independent | 2 | #5 (k6) | x1.277 | x1.277 | 160/160 | 6.8 | 20.9 | 7.05e-06 | 3 | 1 | 1 | 0 | 2 | 1 | 3 | 0 | 0.32 | 7 / 8 | 1 | - | 40004 | 35728 | 284.5 |
| extract_sparse | linear | 1 | #5 (k7) | x1.278 | x1.278 | 160/160 | 6.7 | 20.7 | 8.51e-06 | 4 | 1 | 1 | 0 | 2 | 1 | 3 | 0 | 0.35 | 5 / 8 | 3 | - | 35674 | 31733 | 249.5 |
| extract_sparse | linear | 2 | #1 (k8) | x1.298 | x1.298 | 160/160 | 6.6 | 21.0 | 8.06e-06 | 4 | 1 | 1 | 0 | 2 | 1 | 3 | 0 | 0.46 | 3 / 8 | 5 | - | 31443 | 27075 | 222.3 |
| extract_sparse | linear-explore | 1 | #2 (k9) | x1.285 | x1.285 | 160/160 | 6.7 | 21.3 | 5.09e-06 | 4 | 1 | 1 | 0 | 2 | 1 | 3 | 0 | 0.45 | 5 / 8 | 1 | chehab 1 | 60332 | 55882 | 426.4 |
| extract_sparse | linear-explore | 2 | #2 (k10) | x1.285 | x1.285 | 160/160 | 6.7 | 21.0 | 5.42e-06 | 4 | 1 | 1 | 0 | 2 | 1 | 3 | 0 | 0.44 | 6 / 8 | 0 | - | 60473 | 56334 | 425.0 |
| extract_sparse | linear-minimal | 1 | #2 (k11) | x1.272 | x1.272 | 160/160 | 6.8 | 21.2 | 6.38e-06 | 4 | 1 | 1 | 0 | 2 | 1 | 3 | 0 | 0.41 | 7 / 8 | 1 | - | 33538 | 29692 | 276.6 |
| extract_sparse | linear-minimal | 2 | #1 (k12) | x1.269 | x1.269 | 160/160 | 6.8 | 21.1 | 9.08e-06 | 4 | 1 | 1 | 0 | 2 | 1 | 3 | 0 | 0.43 | 3 / 8 | 5 | - | 42784 | 38965 | 257.4 |
| matmul_2x2 | archive | 1 | #3 (k1) | x1.557 | x1.557 | 16/16 | 142.9 | 190.8 | 0.00e+00 | 10 | 3 |  | 4 | 4 | 0 | 0 | 0 | 0.41 | 6 / 8 | 2 | - | 154284 | 147552 | 1032.6 |
| matmul_2x2 | archive | 2 | #5 (k2) | x1.568 | x1.568 | 16/16 | 141.9 | 187.9 | 0.00e+00 | 10 | 3 |  | 4 | 4 | 0 | 0 | 0 | 0.46 | 7 / 8 | 0 | build 1 | 179681 | 173168 | 1186.9 |
| matmul_2x2 | archive-explore | 1 | #8 (k3) | x1.325 | x1.325 | 16/16 | 167.9 | 214.7 | 0.00e+00 | 12 | 3 |  | 4 | 8 | 0 | 0 | 0 | 0.51 | 6 / 8 | 0 | wrong 2 | 113842 | 105389 | 892.3 |
| matmul_2x2 | archive-explore | 2 | #8 (k4) | x1.558 | x1.558 | 16/16 | 142.8 | 187.9 | 0.00e+00 | 10 | 3 |  | 4 | 4 | 0 | 0 | 0 | 0.42 | 7 / 8 | 0 | wrong 1 | 96708 | 88307 | 601.4 |
| matmul_2x2 | independent | 1 | #5 (k5) | x1.402 | x1.402 | 16/16 | 158.7 | 203.5 | 0.00e+00 | 6 | 2 |  | 4 | 4 | 0 | 0 | 0 | 0.42 | 5 / 8 | 0 | wrong 3 | 53602 | 46461 | 427.9 |
| matmul_2x2 | independent | 2 | #2 (k6) | x1.271 | x1.271 | 16/16 | 175.1 | 220.3 | 0.00e+00 | 5 | 2 |  | 4 | 8 | 0 | 0 | 0 | 0.41 | 5 / 8 | 0 | build 3 | 61561 | 52938 | 406.1 |
| matmul_2x2 | linear | 1 | #5 (k7) | x1.545 | x1.545 | 16/16 | 144.1 | 188.3 | 0.00e+00 | 9 | 3 |  | 4 | 4 | 0 | 0 | 0 | 0.38 | 4 / 8 | 3 | wrong 1 | 63438 | 57049 | 436.1 |
| matmul_2x2 | linear | 2 | #4 (k8) | x1.536 | x1.536 | 16/16 | 144.8 | 188.9 | 0.00e+00 | 10 | 3 |  | 4 | 4 | 0 | 0 | 0 | 0.49 | 5 / 8 | 3 | - | 116608 | 110498 | 822.7 |
| matmul_2x2 | linear-explore | 1 | #5 (k9) | x1.551 | x1.551 | 16/16 | 143.5 | 187.8 | 0.00e+00 | 10 | 3 |  | 4 | 4 | 0 | 0 | 0 | 0.31 | 8 / 8 | 0 | - | 118879 | 112018 | 828.3 |
| matmul_2x2 | linear-explore | 2 | #6 (k10) | x1.544 | x1.544 | 16/16 | 144.1 | 186.9 | 0.00e+00 | 10 | 3 |  | 4 | 4 | 0 | 0 | 0 | 0.41 | 7 / 8 | 1 | - | 97478 | 90630 | 904.5 |
| matmul_2x2 | linear-minimal | 1 | #2 (k11) | x1.391 | x1.391 | 16/16 | 159.9 | 205.3 | 0.00e+00 | 6 | 2 |  | 4 | 4 | 0 | 0 | 0 | 0.41 | 4 / 8 | 4 | - | 83623 | 75715 | 568.2 |
| matmul_2x2 | linear-minimal | 2 | #2 (k12) | x1.378 | x1.378 | 16/16 | 161.5 | 208.2 | 0.00e+00 | 6 | 2 |  | 4 | 4 | 0 | 0 | 0 | 0.44 | 2 / 8 | 6 | - | 81624 | 74964 | 508.8 |

## Per task and strategy (strict speedup)

| task | strategy | runs | speedup |
|---|---|---:|---|
| chebyshev_elu_d7 | archive | 2 | median x2.294, mean x2.294, min x2.010, max x2.578, sd 0.401 |
| chebyshev_gelu_d7 | archive | 2 | median x2.923, mean x2.923, min x2.907, max x2.938, sd 0.022 |
| chebyshev_sigmoid_d7 | archive | 2 | median x2.450, mean x2.450, min x2.324, max x2.576, sd 0.178 |
| coyote_tree_100_50_5_1 | archive | 2 | median x1.000, mean x1.000, min x1.000, max x1.000, sd 0.000 |
| extract_sparse | archive | 2 | median x1.293, mean x1.293, min x1.286, max x1.300, sd 0.009 |
| matmul_2x2 | archive | 2 | median x1.563, mean x1.563, min x1.557, max x1.568, sd 0.008 |
| chebyshev_elu_d7 | archive-explore | 2 | median x2.298, mean x2.298, min x2.015, max x2.580, sd 0.400 |
| chebyshev_gelu_d7 | archive-explore | 2 | median x2.913, mean x2.913, min x2.885, max x2.942, sd 0.041 |
| chebyshev_sigmoid_d7 | archive-explore | 2 | median x1.665, mean x1.665, min x1.000, max x2.329, sd 0.940 |
| coyote_tree_100_50_5_1 | archive-explore | 2 | median x1.000, mean x1.000, min x1.000, max x1.000, sd 0.000 |
| extract_sparse | archive-explore | 2 | median x1.292, mean x1.292, min x1.287, max x1.297, sd 0.007 |
| matmul_2x2 | archive-explore | 2 | median x1.442, mean x1.442, min x1.325, max x1.558, sd 0.164 |
| chebyshev_elu_d7 | independent | 2 | median x1.508, mean x1.508, min x1.000, max x2.016, sd 0.718 |
| chebyshev_gelu_d7 | independent | 2 | median x2.903, mean x2.903, min x2.898, max x2.908, sd 0.007 |
| chebyshev_sigmoid_d7 | independent | 2 | median x1.905, mean x1.905, min x1.798, max x2.013, sd 0.152 |
| coyote_tree_100_50_5_1 | independent | 2 | median x1.000, mean x1.000, min x1.000, max x1.000, sd 0.000 |
| extract_sparse | independent | 2 | median x1.281, mean x1.281, min x1.277, max x1.285, sd 0.006 |
| matmul_2x2 | independent | 2 | median x1.337, mean x1.337, min x1.271, max x1.402, sd 0.093 |
| chebyshev_elu_d7 | linear | 2 | median x2.010, mean x2.010, min x2.010, max x2.011, sd 0.001 |
| chebyshev_gelu_d7 | linear | 2 | median x2.904, mean x2.904, min x2.888, max x2.920, sd 0.023 |
| chebyshev_sigmoid_d7 | linear | 2 | median x2.052, mean x2.052, min x1.644, max x2.460, sd 0.577 |
| coyote_tree_100_50_5_1 | linear | 2 | median x1.000, mean x1.000, min x1.000, max x1.000, sd 0.000 |
| extract_sparse | linear | 2 | median x1.288, mean x1.288, min x1.278, max x1.298, sd 0.014 |
| matmul_2x2 | linear | 2 | median x1.540, mean x1.540, min x1.536, max x1.545, sd 0.006 |
| chebyshev_elu_d7 | linear-explore | 2 | median x2.221, mean x2.221, min x2.010, max x2.433, sd 0.299 |
| chebyshev_gelu_d7 | linear-explore | 2 | median x2.897, mean x2.897, min x2.888, max x2.907, sd 0.014 |
| chebyshev_sigmoid_d7 | linear-explore | 2 | median x2.512, mean x2.512, min x2.509, max x2.515, sd 0.004 |
| coyote_tree_100_50_5_1 | linear-explore | 2 | median x1.000, mean x1.000, min x1.000, max x1.000, sd 0.000 |
| extract_sparse | linear-explore | 2 | median x1.285, mean x1.285, min x1.285, max x1.285, sd 0.000 |
| matmul_2x2 | linear-explore | 2 | median x1.548, mean x1.548, min x1.544, max x1.551, sd 0.005 |
| chebyshev_elu_d7 | linear-minimal | 2 | median x1.486, mean x1.486, min x1.000, max x1.973, sd 0.688 |
| chebyshev_gelu_d7 | linear-minimal | 2 | median x2.897, mean x2.897, min x2.888, max x2.906, sd 0.012 |
| chebyshev_sigmoid_d7 | linear-minimal | 2 | median x2.577, mean x2.577, min x2.441, max x2.713, sd 0.192 |
| coyote_tree_100_50_5_1 | linear-minimal | 2 | median x1.000, mean x1.000, min x1.000, max x1.000, sd 0.000 |
| extract_sparse | linear-minimal | 2 | median x1.270, mean x1.270, min x1.269, max x1.272, sd 0.002 |
| matmul_2x2 | linear-minimal | 2 | median x1.384, mean x1.384, min x1.378, max x1.391, sd 0.010 |

## Across tasks (geometric mean of the per-task mean strict speedups)

| strategy | tasks | geometric mean |
|---|---:|---:|
| archive | 6 | x1.793 |
| archive-explore | 6 | x1.658 |
| independent | 6 | x1.558 |
| linear | 6 | x1.696 |
| linear-explore | 6 | x1.783 |
| linear-minimal | 6 | x1.641 |

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
| chebyshev_elu_d7 | baseline | independent/run2 linear-minimal/run1 | True | 160/160 | 2.68e-07 | 138.23 | 1.0 |
| chebyshev_elu_d7 | k1 | archive/run1 | True | 160/160 | 4.23e-07 | 53.63 | 2.578 |
| chebyshev_elu_d7 | k2 | archive/run2 | True | 160/160 | 2.15e-07 | 68.77 | 2.01 |
| chebyshev_elu_d7 | k3 | archive-explore/run1 | True | 160/160 | 2.95e-07 | 53.57 | 2.58 |
| chebyshev_elu_d7 | k4 | archive-explore/run2 | True | 160/160 | 2.68e-07 | 68.6 | 2.015 |
| chebyshev_elu_d7 | k5 | independent/run1 | True | 160/160 | 2.36e-07 | 68.57 | 2.016 |
| chebyshev_elu_d7 | k6 | linear/run1 | True | 160/160 | 2.93e-07 | 68.73 | 2.011 |
| chebyshev_elu_d7 | k7 | linear/run2 | True | 160/160 | 2.55e-07 | 68.77 | 2.01 |
| chebyshev_elu_d7 | k8 | linear-explore/run1 | True | 160/160 | 2.30e-07 | 68.78 | 2.01 |
| chebyshev_elu_d7 | k9 | linear-explore/run2 | True | 160/160 | 3.49e-07 | 56.82 | 2.433 |
| chebyshev_elu_d7 | k10 | linear-minimal/run2 | True | 160/160 | 3.05e-07 | 70.07 | 1.973 |
| chebyshev_gelu_d7 | baseline | baseline | True | 160/160 | 3.21e-07 | 94.41 | 1.0 |
| chebyshev_gelu_d7 | k1 | archive/run1 | True | 160/160 | 3.19e-07 | 32.13 | 2.938 |
| chebyshev_gelu_d7 | k2 | archive/run2 | True | 160/160 | 2.82e-07 | 32.48 | 2.907 |
| chebyshev_gelu_d7 | k3 | archive-explore/run1 | True | 160/160 | 3.50e-07 | 32.73 | 2.885 |
| chebyshev_gelu_d7 | k4 | archive-explore/run2 | True | 160/160 | 2.90e-07 | 32.09 | 2.942 |
| chebyshev_gelu_d7 | k5 | independent/run1 | True | 160/160 | 3.52e-07 | 32.58 | 2.898 |
| chebyshev_gelu_d7 | k6 | independent/run2 | True | 160/160 | 5.00e-07 | 32.46 | 2.908 |
| chebyshev_gelu_d7 | k7 | linear/run1 | True | 160/160 | 3.88e-07 | 32.69 | 2.888 |
| chebyshev_gelu_d7 | k8 | linear/run2 | True | 160/160 | 2.75e-07 | 32.33 | 2.92 |
| chebyshev_gelu_d7 | k9 | linear-explore/run1 | True | 160/160 | 4.26e-07 | 32.48 | 2.907 |
| chebyshev_gelu_d7 | k10 | linear-explore/run2 | True | 160/160 | 3.28e-07 | 32.7 | 2.888 |
| chebyshev_gelu_d7 | k11 | linear-minimal/run1 | True | 160/160 | 3.27e-07 | 32.69 | 2.888 |
| chebyshev_gelu_d7 | k12 | linear-minimal/run2 | True | 160/160 | 4.51e-07 | 32.49 | 2.906 |
| chebyshev_sigmoid_d7 | baseline | archive-explore/run2 | True | 160/160 | 1.07e-07 | 122.51 | 1.0 |
| chebyshev_sigmoid_d7 | k1 | archive/run1 | True | 160/160 | 1.33e-07 | 52.72 | 2.324 |
| chebyshev_sigmoid_d7 | k2 | archive/run2 | True | 160/160 | 1.58e-07 | 47.57 | 2.576 |
| chebyshev_sigmoid_d7 | k3 | archive-explore/run1 | True | 160/160 | 1.73e-07 | 52.6 | 2.329 |
| chebyshev_sigmoid_d7 | k4 | independent/run1 | True | 160/160 | 1.36e-07 | 60.87 | 2.013 |
| chebyshev_sigmoid_d7 | k5 | independent/run2 | True | 160/160 | 1.23e-07 | 68.15 | 1.798 |
| chebyshev_sigmoid_d7 | k6 | linear/run1 | True | 160/160 | 1.17e-07 | 74.52 | 1.644 |
| chebyshev_sigmoid_d7 | k7 | linear/run2 | True | 160/160 | 1.32e-07 | 49.81 | 2.46 |
| chebyshev_sigmoid_d7 | k8 | linear-explore/run1 | True | 160/160 | 2.62e-07 | 48.82 | 2.509 |
| chebyshev_sigmoid_d7 | k9 | linear-explore/run2 | True | 160/160 | 3.22e-07 | 48.71 | 2.515 |
| chebyshev_sigmoid_d7 | k10 | linear-minimal/run1 | True | 160/160 | 1.43e-07 | 45.16 | 2.713 |
| chebyshev_sigmoid_d7 | k11 | linear-minimal/run2 | True | 160/160 | 4.18e-07 | 50.19 | 2.441 |
| coyote_tree_100_50_5_1 | baseline | archive/run1 archive/run2 archive-explore/run1 archive-explore/run2 independent/run1 independent/run2 linear/run1 linear/run2 linear-explore/run1 linear-explore/run2 linear-minimal/run1 linear-minimal/run2 | True | 16/16 | 0.00e+00 | 320.45 | 1.0 |
| extract_sparse | baseline | baseline | True | 160/160 | 2.62e-05 | 8.63 | 1.0 |
| extract_sparse | k1 | archive/run1 | True | 160/160 | 9.18e-06 | 6.71 | 1.286 |
| extract_sparse | k2 | archive/run2 | True | 160/160 | 1.28e-05 | 6.64 | 1.3 |
| extract_sparse | k3 | archive-explore/run1 | True | 160/160 | 1.02e-05 | 6.65 | 1.297 |
| extract_sparse | k4 | archive-explore/run2 | True | 160/160 | 9.55e-06 | 6.7 | 1.287 |
| extract_sparse | k5 | independent/run1 | True | 160/160 | 1.11e-05 | 6.71 | 1.285 |
| extract_sparse | k6 | independent/run2 | True | 160/160 | 1.01e-05 | 6.76 | 1.277 |
| extract_sparse | k7 | linear/run1 | True | 160/160 | 1.11e-05 | 6.75 | 1.278 |
| extract_sparse | k8 | linear/run2 | True | 160/160 | 1.00e-05 | 6.64 | 1.298 |
| extract_sparse | k9 | linear-explore/run1 | True | 160/160 | 1.25e-05 | 6.71 | 1.285 |
| extract_sparse | k10 | linear-explore/run2 | True | 160/160 | 1.02e-05 | 6.71 | 1.285 |
| extract_sparse | k11 | linear-minimal/run1 | True | 160/160 | 1.26e-05 | 6.78 | 1.272 |
| extract_sparse | k12 | linear-minimal/run2 | True | 160/160 | 1.44e-05 | 6.8 | 1.269 |
| matmul_2x2 | baseline | baseline | True | 16/16 | 0.00e+00 | 222.52 | 1.0 |
| matmul_2x2 | k1 | archive/run1 | True | 16/16 | 0.00e+00 | 142.94 | 1.557 |
| matmul_2x2 | k2 | archive/run2 | True | 16/16 | 0.00e+00 | 141.87 | 1.568 |
| matmul_2x2 | k3 | archive-explore/run1 | True | 16/16 | 0.00e+00 | 167.88 | 1.325 |
| matmul_2x2 | k4 | archive-explore/run2 | True | 16/16 | 0.00e+00 | 142.83 | 1.558 |
| matmul_2x2 | k5 | independent/run1 | True | 16/16 | 0.00e+00 | 158.67 | 1.402 |
| matmul_2x2 | k6 | independent/run2 | True | 16/16 | 0.00e+00 | 175.09 | 1.271 |
| matmul_2x2 | k7 | linear/run1 | True | 16/16 | 0.00e+00 | 144.06 | 1.545 |
| matmul_2x2 | k8 | linear/run2 | True | 16/16 | 0.00e+00 | 144.84 | 1.536 |
| matmul_2x2 | k9 | linear-explore/run1 | True | 16/16 | 0.00e+00 | 143.45 | 1.551 |
| matmul_2x2 | k10 | linear-explore/run2 | True | 16/16 | 0.00e+00 | 144.1 | 1.544 |
| matmul_2x2 | k11 | linear-minimal/run1 | True | 16/16 | 0.00e+00 | 159.94 | 1.391 |
| matmul_2x2 | k12 | linear-minimal/run2 | True | 16/16 | 0.00e+00 | 161.51 | 1.378 |
