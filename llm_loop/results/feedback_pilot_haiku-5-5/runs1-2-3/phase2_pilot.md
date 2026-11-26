# Phase-2 pilot: neutral re-scoring

Commit 67c8d41bb9 (dirty), Darwin arm64 arm, Apple clang version 21.0.0 (clang-2100.1.1.101), SEAL 4.1. Re-scored 2026-11-23T06:03:58.

Speedup = baseline eval-only median / kernel eval-only median, both from the same neutral session (1 warmup + 10 measured processes of ./main --repeats 10, interleaved). The final kernel of each run is the strategy's own selection; re-scoring never re-selects. E2E = CHEHAB execution_time_(ms) (parse + encode + encrypt + fhe()). Compile time: median of 3, reported only. CKKS chain: sized. Repeated correctness checks per case: 20; strict speedup counts a final kernel that fails any check as x1.000.

## Every run

| task | strategy | run | final | speedup | strict | repeats passed | eval (ms) | e2e (ms) | FHE err | depth | xdepth | levels | ct x ct | ct x pt | align | rescale | mod sw | C (ms) | correct / 8 | dup | failures | out tok | think tok | search (s) |
|---|---|---:|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---|---:|---:|---:|
| chebyshev_elu_d7 | archive | 1 | #6 (k1) | x2.578 | x2.578 | 160/160 | 53.6 | 66.9 | 2.45e-07 | 11 | 4 | 4 | 5 | 5 | 3 | 13 | 0 | 0.50 | 4 / 8 | 0 | chehab 2 | 124078 | 115447 | 751.3 |
| chebyshev_elu_d7 | archive | 2 | #5 (k2) | x2.011 | x2.011 | 160/160 | 68.8 | 84.3 | 1.96e-07 | 14 | 5 | 5 | 4 | 9 | 5 | 18 | 4 | 0.48 | 6 / 8 | 1 | chehab 1 | 136022 | 127093 | 1002.2 |
| chebyshev_elu_d7 | archive | 3 | #1 (k3) | x1.975 | x1.975 | 160/160 | 70.0 | 85.5 | 1.83e-07 | 13 | 5 | 5 | 4 | 9 | 6 | 19 | 3 | 0.52 | 5 / 8 | 1 | chehab 2 | 90195 | 82805 | 513.9 |
| chebyshev_elu_d7 | archive-explore | 1 | #5 (k4) | x2.581 | x2.581 | 160/160 | 53.6 | 67.1 | 1.93e-07 | 10 | 4 | 4 | 5 | 5 | 3 | 13 | 0 | 0.51 | 3 / 8 | 0 | chehab 1 | 108070 | 99138 | 640.9 |
| chebyshev_elu_d7 | archive-explore | 2 | #5 (k5) | x2.016 | x2.016 | 160/160 | 68.6 | 84.2 | 2.16e-07 | 13 | 5 | 5 | 4 | 9 | 5 | 18 | 2 | 0.54 | 4 / 8 | 1 | chehab 2 | 113031 | 105179 | 583.7 |
| chebyshev_elu_d7 | archive-explore | 3 | #1 (k6) | x1.973 | x1.973 | 160/160 | 70.1 | 85.6 | 1.91e-07 | 13 | 5 | 5 | 4 | 9 | 6 | 19 | 3 | 0.71 | 4 / 8 | 0 | chehab 4 | 107905 | 99686 | 574.0 |
| chebyshev_elu_d7 | independent | 1 | #7 (k7) | x2.015 | x2.015 | 160/160 | 68.6 | 84.0 | 2.03e-07 | 13 | 5 | 5 | 4 | 9 | 5 | 18 | 3 | 0.46 | 4 / 8 | 0 | chehab 4 | 69435 | 61858 | 475.7 |
| chebyshev_elu_d7 | independent | 2 | #0 (baseline) | x1.000 | x1.000 | 160/160 | 138.3 | 160.9 | 1.55e-07 | 22 | 8 | 8 | 6 | 8 | 11 | 25 | 10 | 0.56 | 0 / 8 | 0 | chehab 7, wrong 1 | 47351 | 40053 | 223.8 |
| chebyshev_elu_d7 | independent | 3 | #5 (k8) | x1.972 | x1.972 | 160/160 | 70.1 | 85.6 | 1.64e-07 | 13 | 5 | 5 | 4 | 9 | 6 | 19 | 3 | 0.50 | 1 / 8 | 0 | chehab 7 | 38838 | 30956 | 204.0 |
| chebyshev_elu_d7 | linear | 1 | #3 (k9) | x2.011 | x2.011 | 160/160 | 68.8 | 84.2 | 1.91e-07 | 14 | 5 | 5 | 4 | 9 | 5 | 18 | 4 | 0.51 | 7 / 8 | 1 | - | 62750 | 53593 | 470.6 |
| chebyshev_elu_d7 | linear | 2 | #3 (k10) | x2.011 | x2.011 | 160/160 | 68.8 | 84.2 | 1.92e-07 | 13 | 5 | 5 | 4 | 9 | 5 | 18 | 2 | 0.55 | 2 / 8 | 4 | chehab 2 | 75620 | 66511 | 467.9 |
| chebyshev_elu_d7 | linear | 3 | #5 (k11) | x1.491 | x1.491 | 160/160 | 92.8 | 110.5 | 2.16e-07 | 17 | 6 | 6 | 4 | 11 | 7 | 22 | 7 | 0.53 | 2 / 8 | 4 | chehab 2 | 45381 | 36801 | 261.5 |
| chebyshev_elu_d7 | linear-explore | 1 | #6 (k12) | x2.010 | x2.010 | 160/160 | 68.8 | 84.3 | 1.67e-07 | 13 | 5 | 5 | 4 | 9 | 5 | 18 | 2 | 0.50 | 4 / 8 | 1 | chehab 3 | 67877 | 58291 | 528.9 |
| chebyshev_elu_d7 | linear-explore | 2 | #5 (k13) | x2.433 | x2.433 | 160/160 | 56.8 | 70.5 | 2.25e-07 | 15 | 4 | 4 | 5 | 7 | 4 | 16 | 1 | 0.54 | 5 / 8 | 0 | chehab 3 | 108816 | 100327 | 685.3 |
| chebyshev_elu_d7 | linear-explore | 3 | #3 (k14) | x1.975 | x1.975 | 160/160 | 70.0 | 85.5 | 1.53e-07 | 13 | 5 | 5 | 4 | 9 | 6 | 19 | 3 | 0.50 | 2 / 8 | 0 | chehab 6 | 56882 | 48558 | 309.3 |
| chebyshev_elu_d7 | linear-minimal | 1 | #0 (baseline) | x1.000 | x1.000 | 160/160 | 138.3 | 160.9 | 1.55e-07 | 22 | 8 | 8 | 6 | 8 | 11 | 25 | 10 | 0.56 | 4 / 8 | 2 | chehab 2 | 60820 | 54839 | 464.1 |
| chebyshev_elu_d7 | linear-minimal | 2 | #4 (k15) | x1.974 | x1.974 | 160/160 | 70.1 | 85.5 | 1.86e-07 | 13 | 5 | 5 | 4 | 9 | 6 | 19 | 3 | 0.51 | 5 / 8 | 1 | chehab 2 | 76078 | 68096 | 538.5 |
| chebyshev_elu_d7 | linear-minimal | 3 | #3 (k16) | x2.011 | x2.011 | 160/160 | 68.8 | 84.3 | 1.63e-07 | 14 | 5 | 5 | 4 | 9 | 5 | 18 | 4 | 0.45 | 5 / 8 | 2 | chehab 1 | 55110 | 46858 | 345.8 |
| chebyshev_gelu_d7 | archive | 1 | #2 (k1) | x2.937 | x2.937 | 160/160 | 32.1 | 44.6 | 1.68e-07 | 11 | 4 | 4 | 3 | 4 | 2 | 9 | 1 | 0.44 | 6 / 8 | 2 | - | 68906 | 62825 | 508.9 |
| chebyshev_gelu_d7 | archive | 2 | #2 (k2) | x2.907 | x2.907 | 160/160 | 32.5 | 45.0 | 1.83e-07 | 10 | 4 | 4 | 3 | 4 | 2 | 9 | 0 | 0.46 | 6 / 8 | 2 | - | 54438 | 48677 | 366.0 |
| chebyshev_gelu_d7 | archive | 3 | #4 (k3) | x2.873 | x2.873 | 160/160 | 32.8 | 45.4 | 2.32e-07 | 9 | 4 | 4 | 3 | 4 | 2 | 9 | 0 | 0.42 | 5 / 8 | 2 | chehab 1 | 53494 | 47521 | 348.4 |
| chebyshev_gelu_d7 | archive-explore | 1 | #4 (k4) | x2.883 | x2.883 | 160/160 | 32.7 | 45.2 | 1.89e-07 | 11 | 4 | 4 | 3 | 4 | 2 | 9 | 0 | 0.41 | 5 / 8 | 0 | chehab 1 | 107042 | 99931 | 736.2 |
| chebyshev_gelu_d7 | archive-explore | 2 | #6 (k5) | x2.939 | x2.939 | 160/160 | 32.1 | 44.7 | 1.62e-07 | 11 | 4 | 4 | 3 | 4 | 2 | 9 | 0 | 0.40 | 6 / 8 | 0 | chehab 1 | 94607 | 88167 | 554.5 |
| chebyshev_gelu_d7 | archive-explore | 3 | #2 (k6) | x2.015 | x2.015 | 160/160 | 46.8 | 61.4 | 1.67e-07 | 13 | 5 | 5 | 3 | 5 | 4 | 12 | 2 | 0.56 | 4 / 8 | 1 | chehab 3 | 51186 | 45149 | 328.8 |
| chebyshev_gelu_d7 | independent | 1 | #2 (k7) | x2.896 | x2.896 | 160/160 | 32.6 | 45.1 | 2.46e-07 | 11 | 4 | 4 | 3 | 4 | 2 | 9 | 1 | 0.50 | 2 / 8 | 0 | chehab 2 | 42478 | 37157 | 305.7 |
| chebyshev_gelu_d7 | independent | 2 | #2 (k8) | x2.907 | x2.907 | 160/160 | 32.5 | 44.9 | 1.76e-07 | 11 | 4 | 4 | 3 | 4 | 2 | 9 | 1 | 0.44 | 2 / 8 | 0 | chehab 1 | 38603 | 33673 | 220.5 |
| chebyshev_gelu_d7 | independent | 3 | #4 (k9) | x2.907 | x2.907 | 160/160 | 32.5 | 44.9 | 2.01e-07 | 11 | 4 | 4 | 3 | 4 | 2 | 9 | 1 | 0.45 | 4 / 8 | 0 | chehab 1 | 44449 | 39458 | 279.3 |
| chebyshev_gelu_d7 | linear | 1 | #3 (k10) | x2.889 | x2.889 | 160/160 | 32.7 | 45.2 | 1.85e-07 | 9 | 4 | 4 | 3 | 4 | 2 | 9 | 0 | 0.45 | 6 / 8 | 2 | - | 48577 | 43554 | 394.8 |
| chebyshev_gelu_d7 | linear | 2 | #3 (k11) | x2.917 | x2.917 | 160/160 | 32.4 | 44.9 | 1.85e-07 | 13 | 4 | 4 | 3 | 4 | 2 | 9 | 1 | 0.43 | 5 / 8 | 3 | - | 55951 | 50822 | 421.0 |
| chebyshev_gelu_d7 | linear | 3 | #5 (k12) | x2.909 | x2.909 | 160/160 | 32.4 | 44.9 | 2.06e-07 | 11 | 4 | 4 | 3 | 4 | 2 | 9 | 1 | 0.42 | 3 / 8 | 4 | chehab 1 | 32676 | 28056 | 217.7 |
| chebyshev_gelu_d7 | linear-explore | 1 | #3 (k13) | x2.908 | x2.908 | 160/160 | 32.5 | 44.8 | 1.53e-07 | 11 | 4 | 4 | 3 | 4 | 2 | 9 | 1 | 0.42 | 6 / 8 | 0 | compile 1, chehab 1 | 72122 | 65399 | 546.5 |
| chebyshev_gelu_d7 | linear-explore | 2 | #3 (k14) | x2.888 | x2.888 | 160/160 | 32.7 | 45.2 | 1.54e-07 | 9 | 4 | 4 | 3 | 4 | 2 | 9 | 0 | 0.45 | 5 / 8 | 0 | chehab 3 | 93733 | 85596 | 699.1 |
| chebyshev_gelu_d7 | linear-explore | 3 | #4 (k15) | x2.897 | x2.897 | 160/160 | 32.6 | 45.2 | 2.43e-07 | 11 | 4 | 4 | 3 | 4 | 2 | 9 | 1 | 0.42 | 6 / 8 | 0 | chehab 2 | 58819 | 52542 | 394.0 |
| chebyshev_gelu_d7 | linear-minimal | 1 | #3 (k16) | x2.888 | x2.888 | 160/160 | 32.7 | 45.1 | 2.62e-07 | 9 | 4 | 4 | 3 | 4 | 2 | 9 | 0 | 0.45 | 4 / 8 | 3 | chehab 1 | 42922 | 38208 | 351.5 |
| chebyshev_gelu_d7 | linear-minimal | 2 | #5 (k17) | x2.905 | x2.905 | 160/160 | 32.5 | 45.0 | 2.64e-07 | 10 | 4 | 4 | 3 | 4 | 2 | 9 | 0 | 0.40 | 7 / 8 | 1 | - | 46513 | 40741 | 382.9 |
| chebyshev_gelu_d7 | linear-minimal | 3 | #1 (k18) | x2.906 | x2.906 | 160/160 | 32.5 | 44.9 | 2.53e-07 | 11 | 4 | 4 | 3 | 4 | 2 | 9 | 1 | 0.46 | 7 / 8 | 1 | - | 33380 | 28409 | 271.5 |
| chebyshev_sigmoid_d7 | archive | 1 | #3 (k1) | x2.323 | x2.323 | 160/160 | 52.7 | 67.8 | 1.31e-07 | 15 | 5 | 5 | 4 | 5 | 1 | 10 | 5 | 0.45 | 6 / 8 | 2 | - | 198206 | 192062 | 1239.4 |
| chebyshev_sigmoid_d7 | archive | 2 | #8 (k2) | x2.575 | x2.575 | 160/160 | 47.5 | 64.1 | 1.06e-07 | 17 | 6 | 6 | 4 | 2 | 0 | 6 | 7 | 0.52 | 7 / 8 | 0 | chehab 1 | 100804 | 95087 | 720.6 |
| chebyshev_sigmoid_d7 | archive | 3 | #4 (k3) | x2.704 | x2.704 | 160/160 | 45.3 | 60.2 | 7.47e-08 | 13 | 5 | 5 | 4 | 3 | 1 | 8 | 3 | 0.44 | 7 / 8 | 1 | - | 111925 | 106063 | 650.5 |
| chebyshev_sigmoid_d7 | archive-explore | 1 | #3 (k4) | x2.327 | x2.327 | 160/160 | 52.6 | 67.4 | 8.33e-08 | 13 | 5 | 5 | 4 | 5 | 4 | 13 | 7 | 0.52 | 7 / 8 | 0 | chehab 1 | 120120 | 114034 | 983.2 |
| chebyshev_sigmoid_d7 | archive-explore | 2 | #0 (baseline) | x1.000 | x1.000 | 160/160 | 122.4 | 144.1 | 7.58e-08 | 22 | 8 | 8 | 6 | 5 | 8 | 19 | 13 | 0.53 | 0 / 8 | 1 | chehab 6, wrong 1 | 53243 | 45878 | 254.5 |
| chebyshev_sigmoid_d7 | archive-explore | 3 | #8 (k5) | x2.495 | x2.495 | 160/160 | 49.0 | 62.3 | 2.95e-07 | 10 | 4 | 4 | 5 | 5 | 3 | 13 | 0 | 0.46 | 4 / 8 | 0 | chehab 3 | 116263 | 108985 | 600.9 |
| chebyshev_sigmoid_d7 | independent | 1 | #3 (k6) | x2.011 | x2.011 | 160/160 | 60.9 | 76.1 | 1.23e-07 | 13 | 5 | 5 | 4 | 7 | 4 | 15 | 5 | 0.52 | 2 / 8 | 0 | chehab 6 | 61927 | 55443 | 345.7 |
| chebyshev_sigmoid_d7 | independent | 2 | #2 (k7) | x1.797 | x1.797 | 160/160 | 68.1 | 85.4 | 7.63e-08 | 16 | 6 | 6 | 4 | 5 | 6 | 15 | 8 | 0.45 | 1 / 8 | 0 | chehab 7 | 52407 | 45842 | 268.2 |
| chebyshev_sigmoid_d7 | independent | 3 | #4 (k8) | x1.841 | x1.841 | 160/160 | 66.5 | 83.7 | 9.18e-08 | 17 | 6 | 6 | 4 | 5 | 5 | 14 | 9 | 0.51 | 2 / 8 | 0 | chehab 6 | 62530 | 55930 | 332.1 |
| chebyshev_sigmoid_d7 | linear | 1 | #4 (k9) | x1.642 | x1.642 | 160/160 | 74.5 | 92.2 | 9.08e-08 | 20 | 6 | 6 | 4 | 5 | 9 | 18 | 11 | 0.56 | 6 / 8 | 1 | chehab 1 | 43947 | 37090 | 402.2 |
| chebyshev_sigmoid_d7 | linear | 2 | #7 (k10) | x2.457 | x2.457 | 160/160 | 49.8 | 64.6 | 1.01e-07 | 15 | 5 | 5 | 4 | 4 | 1 | 9 | 3 | 0.55 | 4 / 8 | 1 | chehab 2, wrong 1 | 74457 | 67170 | 528.2 |
| chebyshev_sigmoid_d7 | linear | 3 | #7 (k11) | x2.110 | x2.110 | 160/160 | 58.0 | 73.1 | 1.28e-07 | 13 | 5 | 5 | 4 | 6 | 2 | 12 | 4 | 0.45 | 6 / 8 | 0 | chehab 2 | 69581 | 62282 | 450.6 |
| chebyshev_sigmoid_d7 | linear-explore | 1 | #7 (k12) | x2.509 | x2.509 | 160/160 | 48.8 | 62.0 | 2.09e-07 | 10 | 4 | 4 | 5 | 5 | 3 | 13 | 0 | 0.46 | 7 / 8 | 0 | chehab 1 | 119888 | 110548 | 929.5 |
| chebyshev_sigmoid_d7 | linear-explore | 2 | #6 (k13) | x2.512 | x2.512 | 160/160 | 48.7 | 61.9 | 2.05e-07 | 11 | 4 | 4 | 5 | 5 | 3 | 13 | 1 | 0.51 | 5 / 8 | 0 | chehab 2 | 157119 | 148356 | 1017.2 |
| chebyshev_sigmoid_d7 | linear-explore | 3 | #7 (k14) | x2.320 | x2.320 | 160/160 | 52.7 | 67.9 | 1.06e-07 | 14 | 5 | 5 | 4 | 5 | 4 | 13 | 8 | 0.49 | 5 / 8 | 0 | chehab 3 | 101936 | 93630 | 555.4 |
| chebyshev_sigmoid_d7 | linear-minimal | 1 | #3 (k15) | x2.710 | x2.710 | 160/160 | 45.2 | 60.0 | 9.53e-08 | 14 | 5 | 5 | 4 | 3 | 1 | 8 | 3 | 0.43 | 7 / 8 | 1 | - | 112205 | 106781 | 760.1 |
| chebyshev_sigmoid_d7 | linear-minimal | 2 | #2 (k16) | x2.439 | x2.439 | 160/160 | 50.2 | 63.5 | 2.30e-07 | 13 | 4 | 4 | 5 | 5 | 4 | 14 | 1 | 0.45 | 5 / 8 | 1 | chehab 1, wrong 1 | 95466 | 87570 | 653.1 |
| chebyshev_sigmoid_d7 | linear-minimal | 3 | #3 (k17) | x2.113 | x2.113 | 160/160 | 57.9 | 72.9 | 9.97e-08 | 13 | 5 | 5 | 4 | 6 | 2 | 12 | 4 | 0.48 | 3 / 8 | 4 | chehab 1 | 45655 | 39360 | 273.6 |
| coyote_tree_100_50_5_1 | archive | 1 | #0 (baseline) | x1.000 | x1.000 | 16/16 | 323.3 | 480.6 | 0.00e+00 | 5 | 3 |  | 12 | 0 | 0 | 0 | 0 | 0.29 | 5 / 8 | 3 | - | 39392 | 31263 | 351.4 |
| coyote_tree_100_50_5_1 | archive | 2 | #0 (baseline) | x1.000 | x1.000 | 16/16 | 323.3 | 480.6 | 0.00e+00 | 5 | 3 |  | 12 | 0 | 0 | 0 | 0 | 0.29 | 6 / 8 | 2 | - | 38251 | 29224 | 411.5 |
| coyote_tree_100_50_5_1 | archive | 3 | #0 (baseline) | x1.000 | x1.000 | 16/16 | 323.3 | 480.6 | 0.00e+00 | 5 | 3 |  | 12 | 0 | 0 | 0 | 0 | 0.29 | 6 / 8 | 2 | - | 42624 | 34212 | 379.8 |
| coyote_tree_100_50_5_1 | archive-explore | 1 | #0 (baseline) | x1.000 | x1.000 | 16/16 | 323.3 | 480.6 | 0.00e+00 | 5 | 3 |  | 12 | 0 | 0 | 0 | 0 | 0.29 | 8 / 8 | 0 | - | 53503 | 43550 | 567.6 |
| coyote_tree_100_50_5_1 | archive-explore | 2 | #0 (baseline) | x1.000 | x1.000 | 16/16 | 323.3 | 480.6 | 0.00e+00 | 5 | 3 |  | 12 | 0 | 0 | 0 | 0 | 0.29 | 8 / 8 | 0 | - | 40282 | 29985 | 440.8 |
| coyote_tree_100_50_5_1 | archive-explore | 3 | #0 (baseline) | x1.000 | x1.000 | 16/16 | 323.3 | 480.6 | 0.00e+00 | 5 | 3 |  | 12 | 0 | 0 | 0 | 0 | 0.29 | 7 / 8 | 0 | compile 1 | 38988 | 29347 | 396.0 |
| coyote_tree_100_50_5_1 | independent | 1 | #0 (baseline) | x1.000 | x1.000 | 16/16 | 323.3 | 480.6 | 0.00e+00 | 5 | 3 |  | 12 | 0 | 0 | 0 | 0 | 0.29 | 8 / 8 | 0 | - | 49245 | 40832 | 561.0 |
| coyote_tree_100_50_5_1 | independent | 2 | #0 (baseline) | x1.000 | x1.000 | 16/16 | 323.3 | 480.6 | 0.00e+00 | 5 | 3 |  | 12 | 0 | 0 | 0 | 0 | 0.29 | 8 / 8 | 0 | - | 43276 | 34935 | 434.4 |
| coyote_tree_100_50_5_1 | independent | 3 | #0 (baseline) | x1.000 | x1.000 | 16/16 | 323.3 | 480.6 | 0.00e+00 | 5 | 3 |  | 12 | 0 | 0 | 0 | 0 | 0.29 | 8 / 8 | 0 | - | 45718 | 37065 | 443.1 |
| coyote_tree_100_50_5_1 | linear | 1 | #0 (baseline) | x1.000 | x1.000 | 16/16 | 323.3 | 480.6 | 0.00e+00 | 5 | 3 |  | 12 | 0 | 0 | 0 | 0 | 0.29 | 3 / 8 | 5 | - | 30241 | 22305 | 270.5 |
| coyote_tree_100_50_5_1 | linear | 2 | #0 (baseline) | x1.000 | x1.000 | 16/16 | 323.3 | 480.6 | 0.00e+00 | 5 | 3 |  | 12 | 0 | 0 | 0 | 0 | 0.29 | 1 / 8 | 7 | - | 32531 | 25351 | 239.1 |
| coyote_tree_100_50_5_1 | linear | 3 | #0 (baseline) | x1.000 | x1.000 | 16/16 | 323.3 | 480.6 | 0.00e+00 | 5 | 3 |  | 12 | 0 | 0 | 0 | 0 | 0.29 | 3 / 8 | 5 | - | 24649 | 17362 | 206.0 |
| coyote_tree_100_50_5_1 | linear-explore | 1 | #0 (baseline) | x1.000 | x1.000 | 16/16 | 323.3 | 480.6 | 0.00e+00 | 5 | 3 |  | 12 | 0 | 0 | 0 | 0 | 0.29 | 7 / 8 | 1 | - | 35659 | 27073 | 411.7 |
| coyote_tree_100_50_5_1 | linear-explore | 2 | #0 (baseline) | x1.000 | x1.000 | 16/16 | 323.3 | 480.6 | 0.00e+00 | 5 | 3 |  | 12 | 0 | 0 | 0 | 0 | 0.29 | 8 / 8 | 0 | - | 41213 | 31763 | 502.8 |
| coyote_tree_100_50_5_1 | linear-explore | 3 | #0 (baseline) | x1.000 | x1.000 | 16/16 | 323.3 | 480.6 | 0.00e+00 | 5 | 3 |  | 12 | 0 | 0 | 0 | 0 | 0.29 | 7 / 8 | 1 | - | 36897 | 28007 | 382.7 |
| coyote_tree_100_50_5_1 | linear-minimal | 1 | #0 (baseline) | x1.000 | x1.000 | 16/16 | 323.3 | 480.6 | 0.00e+00 | 5 | 3 |  | 12 | 0 | 0 | 0 | 0 | 0.29 | 3 / 8 | 5 | - | 42441 | 33784 | 351.9 |
| coyote_tree_100_50_5_1 | linear-minimal | 2 | #0 (baseline) | x1.000 | x1.000 | 16/16 | 323.3 | 480.6 | 0.00e+00 | 5 | 3 |  | 12 | 0 | 0 | 0 | 0 | 0.29 | 4 / 8 | 4 | - | 42703 | 34914 | 372.8 |
| coyote_tree_100_50_5_1 | linear-minimal | 3 | #0 (baseline) | x1.000 | x1.000 | 16/16 | 323.3 | 480.6 | 0.00e+00 | 5 | 3 |  | 12 | 0 | 0 | 0 | 0 | 0.29 | 6 / 8 | 2 | - | 33582 | 25215 | 331.7 |
| extract_sparse | archive | 1 | #2 (k1) | x1.289 | x1.289 | 160/160 | 6.3 | 19.6 | 4.91e-06 | 4 | 1 | 1 | 0 | 2 | 1 | 3 | 0 | 0.30 | 7 / 8 | 1 | - | 75239 | 70705 | 488.4 |
| extract_sparse | archive | 2 | #1 (k2) | x1.290 | x1.290 | 160/160 | 6.3 | 19.6 | 8.87e-06 | 4 | 1 | 1 | 0 | 2 | 1 | 3 | 0 | 0.32 | 7 / 8 | 1 | - | 68030 | 63310 | 496.4 |
| extract_sparse | archive | 3 | #2 (k3) | x1.291 | x1.291 | 160/160 | 6.3 | 19.6 | 5.00e-06 | 4 | 1 | 1 | 0 | 2 | 1 | 3 | 0 | 0.27 | 6 / 8 | 1 | wrong 1 | 70880 | 65634 | 399.8 |
| extract_sparse | archive-explore | 1 | #2 (k4) | x1.290 | x1.290 | 160/160 | 6.3 | 19.6 | 7.39e-06 | 3 | 1 | 1 | 0 | 2 | 1 | 3 | 0 | 0.31 | 6 / 8 | 1 | chehab 1 | 69189 | 64311 | 444.0 |
| extract_sparse | archive-explore | 2 | #2 (k5) | x1.286 | x1.286 | 160/160 | 6.4 | 19.6 | 1.07e-05 | 4 | 1 | 1 | 0 | 2 | 1 | 3 | 0 | 0.30 | 7 / 8 | 0 | chehab 1 | 59674 | 54886 | 366.4 |
| extract_sparse | archive-explore | 3 | #2 (k6) | x1.286 | x1.286 | 160/160 | 6.4 | 19.6 | 8.67e-06 | 4 | 1 | 1 | 0 | 2 | 1 | 3 | 0 | 0.34 | 6 / 8 | 2 | - | 57069 | 52896 | 345.5 |
| extract_sparse | independent | 1 | #1 (k7) | x1.286 | x1.286 | 160/160 | 6.4 | 19.7 | 9.41e-06 | 4 | 1 | 1 | 0 | 2 | 1 | 3 | 0 | 0.31 | 8 / 8 | 0 | - | 38271 | 33890 | 314.3 |
| extract_sparse | independent | 2 | #5 (k8) | x1.290 | x1.290 | 160/160 | 6.3 | 19.6 | 8.22e-06 | 3 | 1 | 1 | 0 | 2 | 1 | 3 | 0 | 0.31 | 7 / 8 | 1 | - | 40004 | 35728 | 284.5 |
| extract_sparse | independent | 3 | #5 (k9) | x1.286 | x1.286 | 160/160 | 6.4 | 19.5 | 6.49e-06 | 4 | 1 | 1 | 0 | 2 | 1 | 3 | 0 | 0.29 | 8 / 8 | 0 | - | 45064 | 40951 | 276.5 |
| extract_sparse | linear | 1 | #5 (k10) | x1.287 | x1.287 | 160/160 | 6.4 | 19.5 | 9.79e-06 | 4 | 1 | 1 | 0 | 2 | 1 | 3 | 0 | 0.29 | 5 / 8 | 3 | - | 35674 | 31733 | 249.5 |
| extract_sparse | linear | 2 | #1 (k11) | x1.288 | x1.288 | 160/160 | 6.4 | 19.7 | 7.59e-06 | 4 | 1 | 1 | 0 | 2 | 1 | 3 | 0 | 0.31 | 3 / 8 | 5 | - | 31443 | 27075 | 222.3 |
| extract_sparse | linear | 3 | #1 (k12) | x1.290 | x1.290 | 160/160 | 6.3 | 19.6 | 6.08e-06 | 3 | 1 | 1 | 0 | 2 | 1 | 3 | 0 | 0.29 | 2 / 8 | 6 | - | 24705 | 21410 | 146.0 |
| extract_sparse | linear-explore | 1 | #2 (k13) | x1.291 | x1.291 | 160/160 | 6.3 | 19.6 | 5.76e-06 | 4 | 1 | 1 | 0 | 2 | 1 | 3 | 0 | 0.32 | 5 / 8 | 1 | chehab 1 | 60332 | 55882 | 426.4 |
| extract_sparse | linear-explore | 2 | #2 (k14) | x1.284 | x1.284 | 160/160 | 6.4 | 19.6 | 7.94e-06 | 4 | 1 | 1 | 0 | 2 | 1 | 3 | 0 | 0.38 | 6 / 8 | 0 | - | 60473 | 56334 | 425.0 |
| extract_sparse | linear-explore | 3 | #1 (k15) | x1.289 | x1.289 | 160/160 | 6.4 | 19.6 | 8.92e-06 | 4 | 1 | 1 | 0 | 2 | 1 | 3 | 0 | 0.31 | 3 / 8 | 4 | - | 44083 | 39905 | 250.5 |
| extract_sparse | linear-minimal | 1 | #2 (k16) | x1.287 | x1.287 | 160/160 | 6.4 | 19.8 | 8.68e-06 | 4 | 1 | 1 | 0 | 2 | 1 | 3 | 0 | 0.29 | 7 / 8 | 1 | - | 33538 | 29692 | 276.6 |
| extract_sparse | linear-minimal | 2 | #1 (k17) | x1.284 | x1.284 | 160/160 | 6.4 | 19.6 | 8.40e-06 | 4 | 1 | 1 | 0 | 2 | 1 | 3 | 0 | 0.28 | 3 / 8 | 5 | - | 42784 | 38965 | 257.4 |
| extract_sparse | linear-minimal | 3 | #1 (k18) | x1.287 | x1.287 | 160/160 | 6.4 | 19.6 | 9.77e-06 | 4 | 1 | 1 | 0 | 2 | 1 | 3 | 0 | 0.31 | 6 / 8 | 2 | - | 45933 | 41487 | 277.5 |
| matmul_2x2 | archive | 1 | #3 (k1) | x1.563 | x1.563 | 16/16 | 137.2 | 178.5 | 0.00e+00 | 10 | 3 |  | 4 | 4 | 0 | 0 | 0 | 0.26 | 6 / 8 | 2 | - | 154284 | 147552 | 1032.6 |
| matmul_2x2 | archive | 2 | #5 (k2) | x1.561 | x1.561 | 16/16 | 137.3 | 178.3 | 0.00e+00 | 10 | 3 |  | 4 | 4 | 0 | 0 | 0 | 0.27 | 7 / 8 | 0 | build 1 | 179681 | 173168 | 1186.9 |
| matmul_2x2 | archive | 3 | #5 (k3) | x1.563 | x1.563 | 16/16 | 137.2 | 178.1 | 0.00e+00 | 10 | 3 |  | 4 | 4 | 0 | 0 | 0 | 0.34 | 5 / 8 | 3 | - | 144442 | 138169 | 746.2 |
| matmul_2x2 | archive-explore | 1 | #8 (k4) | x1.349 | x1.349 | 16/16 | 158.9 | 200.4 | 0.00e+00 | 12 | 3 |  | 4 | 8 | 0 | 0 | 0 | 0.32 | 6 / 8 | 0 | wrong 2 | 113842 | 105389 | 892.3 |
| matmul_2x2 | archive-explore | 2 | #8 (k5) | x1.561 | x1.561 | 16/16 | 137.3 | 178.6 | 0.00e+00 | 10 | 3 |  | 4 | 4 | 0 | 0 | 0 | 0.31 | 7 / 8 | 0 | wrong 1 | 96708 | 88307 | 601.4 |
| matmul_2x2 | archive-explore | 3 | #6 (k6) | x1.563 | x1.563 | 16/16 | 137.2 | 178.2 | 0.00e+00 | 9 | 3 |  | 4 | 4 | 0 | 0 | 0 | 0.34 | 8 / 8 | 0 | - | 105946 | 97882 | 661.2 |
| matmul_2x2 | independent | 1 | #5 (k7) | x1.406 | x1.406 | 16/16 | 152.4 | 193.2 | 0.00e+00 | 6 | 2 |  | 4 | 4 | 0 | 0 | 0 | 0.28 | 5 / 8 | 0 | wrong 3 | 53602 | 46461 | 427.9 |
| matmul_2x2 | independent | 2 | #2 (k8) | x1.297 | x1.297 | 16/16 | 165.3 | 206.4 | 0.00e+00 | 5 | 2 |  | 4 | 8 | 0 | 0 | 0 | 0.27 | 5 / 8 | 0 | build 3 | 61561 | 52938 | 406.1 |
| matmul_2x2 | independent | 3 | #2 (k9) | x1.297 | x1.297 | 16/16 | 165.2 | 206.5 | 0.00e+00 | 5 | 2 |  | 4 | 8 | 0 | 0 | 0 | 0.28 | 6 / 8 | 0 | wrong 2 | 52038 | 45449 | 393.5 |
| matmul_2x2 | linear | 1 | #5 (k10) | x1.567 | x1.567 | 16/16 | 136.8 | 178.0 | 0.00e+00 | 9 | 3 |  | 4 | 4 | 0 | 0 | 0 | 0.33 | 4 / 8 | 3 | wrong 1 | 63438 | 57049 | 436.1 |
| matmul_2x2 | linear | 2 | #4 (k11) | x1.562 | x1.562 | 16/16 | 137.2 | 178.3 | 0.00e+00 | 10 | 3 |  | 4 | 4 | 0 | 0 | 0 | 0.31 | 5 / 8 | 3 | - | 116608 | 110498 | 822.7 |
| matmul_2x2 | linear | 3 | #3 (k12) | x1.563 | x1.563 | 16/16 | 137.2 | 178.5 | 0.00e+00 | 10 | 3 |  | 4 | 4 | 0 | 0 | 0 | 0.28 | 4 / 8 | 4 | - | 85394 | 79041 | 478.2 |
| matmul_2x2 | linear-explore | 1 | #5 (k13) | x1.562 | x1.562 | 16/16 | 137.2 | 178.0 | 0.00e+00 | 10 | 3 |  | 4 | 4 | 0 | 0 | 0 | 0.29 | 8 / 8 | 0 | - | 118879 | 112018 | 828.3 |
| matmul_2x2 | linear-explore | 2 | #6 (k14) | x1.562 | x1.562 | 16/16 | 137.3 | 178.3 | 0.00e+00 | 10 | 3 |  | 4 | 4 | 0 | 0 | 0 | 0.28 | 7 / 8 | 1 | - | 97478 | 90630 | 904.5 |
| matmul_2x2 | linear-explore | 3 | #5 (k15) | x1.561 | x1.561 | 16/16 | 137.3 | 178.3 | 0.00e+00 | 10 | 3 |  | 4 | 4 | 0 | 0 | 0 | 0.29 | 6 / 8 | 1 | wrong 1 | 168815 | 159801 | 875.7 |
| matmul_2x2 | linear-minimal | 1 | #2 (k16) | x1.404 | x1.404 | 16/16 | 152.6 | 193.6 | 0.00e+00 | 6 | 2 |  | 4 | 4 | 0 | 0 | 0 | 0.33 | 4 / 8 | 4 | - | 83623 | 75715 | 568.2 |
| matmul_2x2 | linear-minimal | 2 | #2 (k17) | x1.405 | x1.405 | 16/16 | 152.6 | 194.0 | 0.00e+00 | 6 | 2 |  | 4 | 4 | 0 | 0 | 0 | 0.34 | 2 / 8 | 6 | - | 81624 | 74964 | 508.8 |
| matmul_2x2 | linear-minimal | 3 | #2 (k18) | x1.406 | x1.406 | 16/16 | 152.5 | 193.7 | 0.00e+00 | 6 | 2 |  | 4 | 4 | 0 | 0 | 0 | 0.29 | 6 / 8 | 2 | - | 95922 | 88516 | 564.6 |

## Per task and strategy (strict speedup)

| task | strategy | runs | speedup |
|---|---|---:|---|
| chebyshev_elu_d7 | archive | 3 | median x2.011, mean x2.188, min x1.975, max x2.578, sd 0.338 |
| chebyshev_gelu_d7 | archive | 3 | median x2.907, mean x2.906, min x2.873, max x2.937, sd 0.032 |
| chebyshev_sigmoid_d7 | archive | 3 | median x2.575, mean x2.534, min x2.323, max x2.704, sd 0.194 |
| coyote_tree_100_50_5_1 | archive | 3 | median x1.000, mean x1.000, min x1.000, max x1.000, sd 0.000 |
| extract_sparse | archive | 3 | median x1.290, mean x1.290, min x1.289, max x1.291, sd 0.001 |
| matmul_2x2 | archive | 3 | median x1.563, mean x1.562, min x1.561, max x1.563, sd 0.001 |
| chebyshev_elu_d7 | archive-explore | 3 | median x2.016, mean x2.190, min x1.973, max x2.581, sd 0.339 |
| chebyshev_gelu_d7 | archive-explore | 3 | median x2.883, mean x2.612, min x2.015, max x2.939, sd 0.518 |
| chebyshev_sigmoid_d7 | archive-explore | 3 | median x2.327, mean x1.941, min x1.000, max x2.495, sd 0.819 |
| coyote_tree_100_50_5_1 | archive-explore | 3 | median x1.000, mean x1.000, min x1.000, max x1.000, sd 0.000 |
| extract_sparse | archive-explore | 3 | median x1.286, mean x1.287, min x1.286, max x1.290, sd 0.002 |
| matmul_2x2 | archive-explore | 3 | median x1.561, mean x1.491, min x1.349, max x1.563, sd 0.123 |
| chebyshev_elu_d7 | independent | 3 | median x1.972, mean x1.662, min x1.000, max x2.015, sd 0.574 |
| chebyshev_gelu_d7 | independent | 3 | median x2.907, mean x2.903, min x2.896, max x2.907, sd 0.007 |
| chebyshev_sigmoid_d7 | independent | 3 | median x1.841, mean x1.883, min x1.797, max x2.011, sd 0.113 |
| coyote_tree_100_50_5_1 | independent | 3 | median x1.000, mean x1.000, min x1.000, max x1.000, sd 0.000 |
| extract_sparse | independent | 3 | median x1.286, mean x1.287, min x1.286, max x1.290, sd 0.002 |
| matmul_2x2 | independent | 3 | median x1.297, mean x1.334, min x1.297, max x1.406, sd 0.063 |
| chebyshev_elu_d7 | linear | 3 | median x2.011, mean x1.837, min x1.491, max x2.011, sd 0.300 |
| chebyshev_gelu_d7 | linear | 3 | median x2.909, mean x2.905, min x2.889, max x2.917, sd 0.014 |
| chebyshev_sigmoid_d7 | linear | 3 | median x2.110, mean x2.070, min x1.642, max x2.457, sd 0.409 |
| coyote_tree_100_50_5_1 | linear | 3 | median x1.000, mean x1.000, min x1.000, max x1.000, sd 0.000 |
| extract_sparse | linear | 3 | median x1.288, mean x1.288, min x1.287, max x1.290, sd 0.002 |
| matmul_2x2 | linear | 3 | median x1.563, mean x1.564, min x1.562, max x1.567, sd 0.003 |
| chebyshev_elu_d7 | linear-explore | 3 | median x2.010, mean x2.140, min x1.975, max x2.433, sd 0.255 |
| chebyshev_gelu_d7 | linear-explore | 3 | median x2.897, mean x2.898, min x2.888, max x2.908, sd 0.010 |
| chebyshev_sigmoid_d7 | linear-explore | 3 | median x2.509, mean x2.447, min x2.320, max x2.512, sd 0.110 |
| coyote_tree_100_50_5_1 | linear-explore | 3 | median x1.000, mean x1.000, min x1.000, max x1.000, sd 0.000 |
| extract_sparse | linear-explore | 3 | median x1.289, mean x1.288, min x1.284, max x1.291, sd 0.003 |
| matmul_2x2 | linear-explore | 3 | median x1.562, mean x1.562, min x1.561, max x1.562, sd 0.001 |
| chebyshev_elu_d7 | linear-minimal | 3 | median x1.974, mean x1.661, min x1.000, max x2.011, sd 0.573 |
| chebyshev_gelu_d7 | linear-minimal | 3 | median x2.905, mean x2.899, min x2.888, max x2.906, sd 0.010 |
| chebyshev_sigmoid_d7 | linear-minimal | 3 | median x2.439, mean x2.420, min x2.113, max x2.710, sd 0.299 |
| coyote_tree_100_50_5_1 | linear-minimal | 3 | median x1.000, mean x1.000, min x1.000, max x1.000, sd 0.000 |
| extract_sparse | linear-minimal | 3 | median x1.287, mean x1.286, min x1.284, max x1.287, sd 0.002 |
| matmul_2x2 | linear-minimal | 3 | median x1.405, mean x1.405, min x1.404, max x1.406, sd 0.001 |

## Across tasks (geometric mean of the per-task mean strict speedups)

| strategy | tasks | geometric mean |
|---|---:|---:|
| archive | 6 | x1.786 |
| archive-explore | 6 | x1.665 |
| independent | 6 | x1.581 |
| linear | 6 | x1.677 |
| linear-explore | 6 | x1.768 |
| linear-minimal | 6 | x1.662 |

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
| chebyshev_elu_d7 | baseline | independent/run2 linear-minimal/run1 | True | 160/160 | 2.17e-07 | 138.28 | 1.0 |
| chebyshev_elu_d7 | k1 | archive/run1 | True | 160/160 | 3.58e-07 | 53.65 | 2.578 |
| chebyshev_elu_d7 | k2 | archive/run2 | True | 160/160 | 2.52e-07 | 68.76 | 2.011 |
| chebyshev_elu_d7 | k3 | archive/run3 | True | 160/160 | 2.25e-07 | 70.0 | 1.975 |
| chebyshev_elu_d7 | k4 | archive-explore/run1 | True | 160/160 | 3.90e-07 | 53.58 | 2.581 |
| chebyshev_elu_d7 | k5 | archive-explore/run2 | True | 160/160 | 1.93e-07 | 68.58 | 2.016 |
| chebyshev_elu_d7 | k6 | archive-explore/run3 | True | 160/160 | 2.18e-07 | 70.07 | 1.973 |
| chebyshev_elu_d7 | k7 | independent/run1 | True | 160/160 | 2.35e-07 | 68.61 | 2.015 |
| chebyshev_elu_d7 | k8 | independent/run3 | True | 160/160 | 2.54e-07 | 70.12 | 1.972 |
| chebyshev_elu_d7 | k9 | linear/run1 | True | 160/160 | 2.12e-07 | 68.76 | 2.011 |
| chebyshev_elu_d7 | k10 | linear/run2 | True | 160/160 | 2.50e-07 | 68.77 | 2.011 |
| chebyshev_elu_d7 | k11 | linear/run3 | True | 160/160 | 2.00e-07 | 92.76 | 1.491 |
| chebyshev_elu_d7 | k12 | linear-explore/run1 | True | 160/160 | 2.37e-07 | 68.78 | 2.01 |
| chebyshev_elu_d7 | k13 | linear-explore/run2 | True | 160/160 | 3.88e-07 | 56.83 | 2.433 |
| chebyshev_elu_d7 | k14 | linear-explore/run3 | True | 160/160 | 3.18e-07 | 70.01 | 1.975 |
| chebyshev_elu_d7 | k15 | linear-minimal/run2 | True | 160/160 | 2.15e-07 | 70.06 | 1.974 |
| chebyshev_elu_d7 | k16 | linear-minimal/run3 | True | 160/160 | 2.23e-07 | 68.77 | 2.011 |
| chebyshev_gelu_d7 | baseline | baseline | True | 160/160 | 3.14e-07 | 94.37 | 1.0 |
| chebyshev_gelu_d7 | k1 | archive/run1 | True | 160/160 | 3.62e-07 | 32.13 | 2.937 |
| chebyshev_gelu_d7 | k2 | archive/run2 | True | 160/160 | 3.21e-07 | 32.47 | 2.907 |
| chebyshev_gelu_d7 | k3 | archive/run3 | True | 160/160 | 3.23e-07 | 32.85 | 2.873 |
| chebyshev_gelu_d7 | k4 | archive-explore/run1 | True | 160/160 | 3.12e-07 | 32.74 | 2.883 |
| chebyshev_gelu_d7 | k5 | archive-explore/run2 | True | 160/160 | 4.29e-07 | 32.11 | 2.939 |
| chebyshev_gelu_d7 | k6 | archive-explore/run3 | True | 160/160 | 3.20e-07 | 46.83 | 2.015 |
| chebyshev_gelu_d7 | k7 | independent/run1 | True | 160/160 | 3.35e-07 | 32.59 | 2.896 |
| chebyshev_gelu_d7 | k8 | independent/run2 | True | 160/160 | 3.11e-07 | 32.46 | 2.907 |
| chebyshev_gelu_d7 | k9 | independent/run3 | True | 160/160 | 3.34e-07 | 32.46 | 2.907 |
| chebyshev_gelu_d7 | k10 | linear/run1 | True | 160/160 | 2.88e-07 | 32.66 | 2.889 |
| chebyshev_gelu_d7 | k11 | linear/run2 | True | 160/160 | 2.92e-07 | 32.35 | 2.917 |
| chebyshev_gelu_d7 | k12 | linear/run3 | True | 160/160 | 3.28e-07 | 32.44 | 2.909 |
| chebyshev_gelu_d7 | k13 | linear-explore/run1 | True | 160/160 | 3.64e-07 | 32.45 | 2.908 |
| chebyshev_gelu_d7 | k14 | linear-explore/run2 | True | 160/160 | 3.15e-07 | 32.68 | 2.888 |
| chebyshev_gelu_d7 | k15 | linear-explore/run3 | True | 160/160 | 3.61e-07 | 32.57 | 2.897 |
| chebyshev_gelu_d7 | k16 | linear-minimal/run1 | True | 160/160 | 2.86e-07 | 32.67 | 2.888 |
| chebyshev_gelu_d7 | k17 | linear-minimal/run2 | True | 160/160 | 3.16e-07 | 32.49 | 2.905 |
| chebyshev_gelu_d7 | k18 | linear-minimal/run3 | True | 160/160 | 3.12e-07 | 32.48 | 2.906 |
| chebyshev_sigmoid_d7 | baseline | archive-explore/run2 | True | 160/160 | 1.00e-07 | 122.37 | 1.0 |
| chebyshev_sigmoid_d7 | k1 | archive/run1 | True | 160/160 | 1.58e-07 | 52.68 | 2.323 |
| chebyshev_sigmoid_d7 | k2 | archive/run2 | True | 160/160 | 1.32e-07 | 47.51 | 2.575 |
| chebyshev_sigmoid_d7 | k3 | archive/run3 | True | 160/160 | 8.10e-08 | 45.25 | 2.704 |
| chebyshev_sigmoid_d7 | k4 | archive-explore/run1 | True | 160/160 | 1.08e-07 | 52.59 | 2.327 |
| chebyshev_sigmoid_d7 | k5 | archive-explore/run3 | True | 160/160 | 3.51e-07 | 49.04 | 2.495 |
| chebyshev_sigmoid_d7 | k6 | independent/run1 | True | 160/160 | 1.39e-07 | 60.86 | 2.011 |
| chebyshev_sigmoid_d7 | k7 | independent/run2 | True | 160/160 | 1.21e-07 | 68.1 | 1.797 |
| chebyshev_sigmoid_d7 | k8 | independent/run3 | True | 160/160 | 1.16e-07 | 66.47 | 1.841 |
| chebyshev_sigmoid_d7 | k9 | linear/run1 | True | 160/160 | 1.15e-07 | 74.52 | 1.642 |
| chebyshev_sigmoid_d7 | k10 | linear/run2 | True | 160/160 | 1.87e-07 | 49.8 | 2.457 |
| chebyshev_sigmoid_d7 | k11 | linear/run3 | True | 160/160 | 1.29e-07 | 57.99 | 2.11 |
| chebyshev_sigmoid_d7 | k12 | linear-explore/run1 | True | 160/160 | 3.70e-07 | 48.78 | 2.509 |
| chebyshev_sigmoid_d7 | k13 | linear-explore/run2 | True | 160/160 | 3.03e-07 | 48.72 | 2.512 |
| chebyshev_sigmoid_d7 | k14 | linear-explore/run3 | True | 160/160 | 1.24e-07 | 52.75 | 2.32 |
| chebyshev_sigmoid_d7 | k15 | linear-minimal/run1 | True | 160/160 | 1.61e-07 | 45.16 | 2.71 |
| chebyshev_sigmoid_d7 | k16 | linear-minimal/run2 | True | 160/160 | 2.80e-07 | 50.18 | 2.439 |
| chebyshev_sigmoid_d7 | k17 | linear-minimal/run3 | True | 160/160 | 1.44e-07 | 57.92 | 2.113 |
| coyote_tree_100_50_5_1 | baseline | archive/run1 archive/run2 archive/run3 archive-explore/run1 archive-explore/run2 archive-explore/run3 independent/run1 independent/run2 independent/run3 linear/run1 linear/run2 linear/run3 linear-explore/run1 linear-explore/run2 linear-explore/run3 linear-minimal/run1 linear-minimal/run2 linear-minimal/run3 | True | 16/16 | 0.00e+00 | 323.28 | 1.0 |
| extract_sparse | baseline | baseline | True | 160/160 | 2.51e-05 | 8.19 | 1.0 |
| extract_sparse | k1 | archive/run1 | True | 160/160 | 1.28e-05 | 6.35 | 1.289 |
| extract_sparse | k2 | archive/run2 | True | 160/160 | 1.05e-05 | 6.35 | 1.29 |
| extract_sparse | k3 | archive/run3 | True | 160/160 | 1.21e-05 | 6.34 | 1.291 |
| extract_sparse | k4 | archive-explore/run1 | True | 160/160 | 1.11e-05 | 6.35 | 1.29 |
| extract_sparse | k5 | archive-explore/run2 | True | 160/160 | 1.12e-05 | 6.37 | 1.286 |
| extract_sparse | k6 | archive-explore/run3 | True | 160/160 | 1.04e-05 | 6.37 | 1.286 |
| extract_sparse | k7 | independent/run1 | True | 160/160 | 1.48e-05 | 6.37 | 1.286 |
| extract_sparse | k8 | independent/run2 | True | 160/160 | 1.19e-05 | 6.35 | 1.29 |
| extract_sparse | k9 | independent/run3 | True | 160/160 | 9.86e-06 | 6.37 | 1.286 |
| extract_sparse | k10 | linear/run1 | True | 160/160 | 1.11e-05 | 6.36 | 1.287 |
| extract_sparse | k11 | linear/run2 | True | 160/160 | 1.11e-05 | 6.36 | 1.288 |
| extract_sparse | k12 | linear/run3 | True | 160/160 | 9.73e-06 | 6.35 | 1.29 |
| extract_sparse | k13 | linear-explore/run1 | True | 160/160 | 1.08e-05 | 6.34 | 1.291 |
| extract_sparse | k14 | linear-explore/run2 | True | 160/160 | 1.07e-05 | 6.38 | 1.284 |
| extract_sparse | k15 | linear-explore/run3 | True | 160/160 | 1.03e-05 | 6.35 | 1.289 |
| extract_sparse | k16 | linear-minimal/run1 | True | 160/160 | 1.01e-05 | 6.36 | 1.287 |
| extract_sparse | k17 | linear-minimal/run2 | True | 160/160 | 1.15e-05 | 6.38 | 1.284 |
| extract_sparse | k18 | linear-minimal/run3 | True | 160/160 | 1.10e-05 | 6.36 | 1.287 |
| matmul_2x2 | baseline | baseline | True | 16/16 | 0.00e+00 | 214.37 | 1.0 |
| matmul_2x2 | k1 | archive/run1 | True | 16/16 | 0.00e+00 | 137.19 | 1.563 |
| matmul_2x2 | k2 | archive/run2 | True | 16/16 | 0.00e+00 | 137.31 | 1.561 |
| matmul_2x2 | k3 | archive/run3 | True | 16/16 | 0.00e+00 | 137.16 | 1.563 |
| matmul_2x2 | k4 | archive-explore/run1 | True | 16/16 | 0.00e+00 | 158.92 | 1.349 |
| matmul_2x2 | k5 | archive-explore/run2 | True | 16/16 | 0.00e+00 | 137.31 | 1.561 |
| matmul_2x2 | k6 | archive-explore/run3 | True | 16/16 | 0.00e+00 | 137.17 | 1.563 |
| matmul_2x2 | k7 | independent/run1 | True | 16/16 | 0.00e+00 | 152.44 | 1.406 |
| matmul_2x2 | k8 | independent/run2 | True | 16/16 | 0.00e+00 | 165.26 | 1.297 |
| matmul_2x2 | k9 | independent/run3 | True | 16/16 | 0.00e+00 | 165.24 | 1.297 |
| matmul_2x2 | k10 | linear/run1 | True | 16/16 | 0.00e+00 | 136.79 | 1.567 |
| matmul_2x2 | k11 | linear/run2 | True | 16/16 | 0.00e+00 | 137.24 | 1.562 |
| matmul_2x2 | k12 | linear/run3 | True | 16/16 | 0.00e+00 | 137.15 | 1.563 |
| matmul_2x2 | k13 | linear-explore/run1 | True | 16/16 | 0.00e+00 | 137.2 | 1.562 |
| matmul_2x2 | k14 | linear-explore/run2 | True | 16/16 | 0.00e+00 | 137.28 | 1.562 |
| matmul_2x2 | k15 | linear-explore/run3 | True | 16/16 | 0.00e+00 | 137.35 | 1.561 |
| matmul_2x2 | k16 | linear-minimal/run1 | True | 16/16 | 0.00e+00 | 152.63 | 1.404 |
| matmul_2x2 | k17 | linear-minimal/run2 | True | 16/16 | 0.00e+00 | 152.57 | 1.405 |
| matmul_2x2 | k18 | linear-minimal/run3 | True | 16/16 | 0.00e+00 | 152.48 | 1.406 |
