# Phase-2 pilot: neutral re-scoring

Commit d1eb0ef9ee (dirty), Darwin arm64 arm, Apple clang version 17.0.0 (clang-1700.3.19.1), SEAL 4.1. Re-scored 2026-11-10T12:14:09.

Speedup = baseline eval-only median / kernel eval-only median, both from the same neutral session (1 warmup + 10 measured processes of ./main --repeats 10, interleaved). The final kernel of each run is the strategy's own selection; re-scoring never re-selects. E2E = CHEHAB execution_time_(ms) (parse + encode + encrypt + fhe()). Compile time: median of 3, reported only.

## Every run

| task | strategy | run | final | speedup | eval (ms) | e2e (ms) | FHE err | depth | xdepth | levels | ct x ct | ct x pt | align | rescale | mod sw | C (ms) | correct / 8 | dup | failures | out tok | think tok | search (s) |
|---|---|---:|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---|---:|---:|---:|
| batchnorm1d | haiku | 1 | #0 (baseline) | x1.000 | 2.1 | 9.0 | 5.34e-08 | 2 | 1 | 1 | 0 | 1 | 0 | 1 | 0 | 0.36 | 1 / 8 | 7 | - | 5779 | 3656 | 79.9 |
| batchnorm1d | opus | 1 | #0 (baseline) | x1.000 | 2.1 | 9.0 | 5.34e-08 | 2 | 1 | 1 | 0 | 1 | 0 | 1 | 0 | 0.36 | 0 / 8 | 8 | - | 2921 | 1086 | 75.6 |
| batchnorm1d | sonnet | 1 | #0 (baseline) | x1.000 | 2.1 | 9.0 | 5.34e-08 | 2 | 1 | 1 | 0 | 1 | 0 | 1 | 0 | 0.36 | 3 / 8 | 5 | - | 3781 | 2163 | 92.7 |
| batchnorm2d | haiku | 1 | #5 (k1) | x0.978 | 2.3 | 9.4 | 6.07e-08 | 2 | 1 | 1 | 0 | 1 | 0 | 1 | 0 | 0.60 | 1 / 8 | 7 | - | 6489 | 4238 | 83.9 |
| batchnorm2d | opus | 1 | #0 (baseline) | x1.000 | 2.3 | 10.0 | 3.78e-08 | 2 | 1 | 1 | 0 | 1 | 0 | 1 | 0 | 0.56 | 2 / 8 | 6 | - | 2874 | 1055 | 87.1 |
| batchnorm2d | sonnet | 1 | #0 (baseline) | x1.000 | 2.3 | 10.0 | 3.78e-08 | 2 | 1 | 1 | 0 | 1 | 0 | 1 | 0 | 0.56 | 2 / 8 | 6 | - | 3764 | 2141 | 90.0 |
| chebyshev_elu_d7 | haiku | 1 | #3 (k1) | x2.030 | 69.0 | 85.9 | 1.49e-07 | 13 | 5 | 5 | 4 | 9 | 5 | 18 | 2 | 1.64 | 8 / 8 | 0 | - | 96244 | 87692 | 628.6 |
| chebyshev_elu_d7 | haiku | 2 | #2 (k2) | x2.002 | 70.0 | 86.1 | 2.00e-07 | 13 | 5 | 5 | 4 | 9 | 6 | 19 | 3 | 0.79 | 6 / 8 | 1 | chehab 1 | 83440 | 74830 | 523.8 |
| chebyshev_elu_d7 | haiku | 3 | #4 (k3) | x2.028 | 69.1 | 85.5 | 2.34e-07 | 16 | 5 | 5 | 4 | 9 | 5 | 18 | 2 | 0.86 | 6 / 8 | 0 | chehab 1, wrong 1 | 95429 | 86379 | 589.7 |
| chebyshev_elu_d7 | haiku | 4 | #7 (k4) | x2.041 | 68.6 | 85.1 | 1.95e-07 | 14 | 5 | 5 | 4 | 9 | 5 | 18 | 2 | 0.74 | 4 / 8 | 0 | chehab 4 | 75614 | 67902 | 472.1 |
| chebyshev_elu_d7 | haiku | 5 | #5 (k5) | x2.039 | 68.7 | 85.4 | 1.77e-07 | 13 | 5 | 5 | 4 | 9 | 5 | 18 | 2 | 0.81 | 6 / 8 | 0 | chehab 2 | 80758 | 73013 | 512.9 |
| chebyshev_elu_d7 | opus | 1 | #8 (k6) | x1.971 | 71.1 | 89.5 | 1.52e-07 | 20 | 6 | 6 | 4 | 7 | 5 | 16 | 4 | 0.79 | 7 / 8 | 1 | - | 79523 | 71703 | 1068.3 |
| chebyshev_elu_d7 | opus | 2 | #8 (k7) | x2.169 | 64.6 | 82.4 | 1.60e-07 | 20 | 6 | 6 | 4 | 7 | 3 | 14 | 6 | 0.81 | 8 / 8 | 0 | - | 81388 | 73464 | 1109.9 |
| chebyshev_elu_d7 | opus | 3 | #6 (k8) | x1.921 | 72.9 | 92.3 | 1.33e-07 | 20 | 6 | 6 | 4 | 7 | 6 | 17 | 6 | 0.85 | 6 / 8 | 0 | runtime 1, wrong 1 | 71845 | 63964 | 962.3 |
| chebyshev_elu_d7 | sonnet | 1 | #6 (k9) | x2.023 | 69.2 | 85.3 | 1.65e-07 | 13 | 5 | 5 | 4 | 9 | 5 | 18 | 2 | 0.74 | 7 / 8 | 1 | - | 23829 | 18675 | 477.2 |
| chebyshev_elu_d7 | sonnet | 2 | #4 (k10) | x1.577 | 88.8 | 108.7 | 1.63e-07 | 20 | 6 | 6 | 4 | 7 | 12 | 23 | 7 | 1.10 | 6 / 8 | 1 | chehab 1 | 32727 | 26635 | 521.3 |
| chebyshev_elu_d7 | sonnet | 3 | #1 (k11) | x1.988 | 70.5 | 86.6 | 2.71e-07 | 14 | 5 | 5 | 4 | 9 | 6 | 19 | 3 | 0.89 | 7 / 8 | 1 | - | 26276 | 20399 | 478.7 |
| chebyshev_gelu_d7 | haiku | 1 | #5 (k1) | x2.483 | 39.3 | 55.0 | 2.13e-07 | 18 | 5 | 5 | 3 | 3 | 1 | 7 | 4 | 0.73 | 6 / 8 | 0 | chehab 2 | 59417 | 53261 | 411.7 |
| chebyshev_gelu_d7 | haiku | 2 | #3 (k2) | x2.901 | 33.6 | 47.2 | 1.92e-07 | 9 | 4 | 4 | 3 | 4 | 2 | 9 | 0 | 0.90 | 5 / 8 | 2 | chehab 1 | 74192 | 67226 | 445.6 |
| chebyshev_gelu_d7 | haiku | 3 | #5 (k3) | x2.327 | 41.9 | 56.9 | 1.76e-07 | 15 | 5 | 5 | 3 | 4 | 2 | 9 | 4 | 0.72 | 6 / 8 | 1 | chehab 1 | 72890 | 67155 | 462.2 |
| chebyshev_gelu_d7 | haiku | 4 | #7 (k4) | x2.441 | 39.9 | 54.8 | 2.42e-07 | 15 | 5 | 5 | 3 | 3 | 1 | 7 | 4 | 0.75 | 7 / 8 | 0 | chehab 1 | 73001 | 66218 | 485.2 |
| chebyshev_gelu_d7 | haiku | 5 | #4 (k5) | x2.945 | 33.1 | 47.0 | 4.91e-07 | 11 | 4 | 4 | 3 | 4 | 2 | 9 | 1 | 0.65 | 6 / 8 | 0 | chehab 1, wrong 1 | 54817 | 49194 | 382.6 |
| chebyshev_gelu_d7 | opus | 1 | #8 (k6) | x1.449 | 67.3 | 97.3 | 4.87e-07 | 18 | 10 | 10 | 3 | 10 | 2 | 15 | 0 | 0.90 | 8 / 8 | 0 | - | 51697 | 45505 | 769.1 |
| chebyshev_gelu_d7 | opus | 2 | #4 (k7) | x2.560 | 38.1 | 54.3 | 2.46e-07 | 17 | 5 | 5 | 3 | 3 | 1 | 7 | 4 | 0.71 | 8 / 8 | 0 | - | 38777 | 33539 | 625.3 |
| chebyshev_gelu_d7 | opus | 3 | #2 (k8) | x2.592 | 37.6 | 53.5 | 2.88e-07 | 16 | 5 | 5 | 3 | 3 | 1 | 7 | 4 | 0.75 | 8 / 8 | 0 | - | 44850 | 39457 | 692.0 |
| chebyshev_gelu_d7 | sonnet | 1 | #8 (k9) | x1.920 | 50.8 | 67.6 | 1.96e-07 | 18 | 5 | 5 | 3 | 5 | 6 | 14 | 2 | 0.79 | 5 / 8 | 1 | chehab 2 | 26599 | 20868 | 418.3 |
| chebyshev_gelu_d7 | sonnet | 2 | #2 (k10) | x2.945 | 33.1 | 46.1 | 1.74e-07 | 13 | 4 | 4 | 3 | 4 | 2 | 9 | 1 | 0.70 | 8 / 8 | 0 | - | 29278 | 25021 | 488.1 |
| chebyshev_gelu_d7 | sonnet | 3 | #4 (k11) | x2.332 | 41.8 | 58.7 | 1.78e-07 | 14 | 5 | 5 | 3 | 4 | 2 | 9 | 3 | 0.73 | 5 / 8 | 2 | chehab 1 | 16398 | 12291 | 313.2 |
| chebyshev_hardshrink_d7 | haiku | 1 | #7 (k1) | x2.765 | 45.1 | 60.9 | 1.47e-07 | 13 | 5 | 5 | 4 | 3 | 1 | 8 | 3 | 0.71 | 8 / 8 | 0 | - | 113572 | 108054 | 701.0 |
| chebyshev_hardshrink_d7 | haiku | 2 | #8 (k2) | x2.429 | 51.4 | 67.6 | 1.56e-07 | 12 | 5 | 5 | 4 | 5 | 3 | 12 | 6 | 0.89 | 7 / 8 | 0 | chehab 1 | 110592 | 105001 | 670.4 |
| chebyshev_hardshrink_d7 | haiku | 3 | #2 (k3) | x1.881 | 66.3 | 85.7 | 2.34e-07 | 20 | 6 | 6 | 4 | 4 | 5 | 13 | 6 | 0.88 | 5 / 8 | 1 | chehab 1 | 120832 | 113504 | 694.5 |
| chebyshev_hardshrink_d7 | opus | 1 | #4 (k4) | x2.572 | 48.5 | 65.2 | 1.74e-07 | 15 | 6 | 6 | 4 | 2 | 1 | 7 | 8 | 0.77 | 7 / 8 | 1 | - | 91086 | 85251 | 1132.9 |
| chebyshev_hardshrink_d7 | opus | 2 | #2 (k5) | x2.561 | 48.7 | 66.1 | 1.40e-07 | 17 | 6 | 6 | 4 | 2 | 0 | 6 | 7 | 0.81 | 7 / 8 | 0 | - | 85300 | 79900 | 1096.4 |
| chebyshev_hardshrink_d7 | opus | 3 | #3 (k6) | x2.588 | 48.2 | 65.7 | 1.51e-07 | 14 | 6 | 6 | 4 | 2 | 1 | 7 | 8 | 0.69 | 4 / 8 | 0 | wrong 1 | 59202 | 53207 | 732.0 |
| chebyshev_hardshrink_d7 | sonnet | 1 | #7 (k7) | x2.253 | 55.4 | 71.9 | 4.01e-07 | 15 | 5 | 5 | 4 | 5 | 3 | 12 | 5 | 0.93 | 5 / 8 | 1 | chehab 2 | 32908 | 26527 | 490.7 |
| chebyshev_hardshrink_d7 | sonnet | 2 | #4 (k8) | x2.165 | 57.6 | 74.9 | 1.68e-07 | 17 | 6 | 6 | 4 | 3 | 3 | 10 | 6 | 0.81 | 3 / 8 | 2 | chehab 2, wrong 1 | 35235 | 30595 | 427.5 |
| chebyshev_hardshrink_d7 | sonnet | 3 | #7 (k9) | x2.314 | 53.9 | 69.5 | 5.70e-07 | 12 | 5 | 5 | 4 | 5 | 3 | 12 | 5 | 0.80 | 7 / 8 | 0 | - | 35460 | 28224 | 533.0 |
| chebyshev_mish_d7 | haiku | 1 | #5 (k1) | x2.037 | 69.0 | 85.4 | 1.72e-07 | 13 | 5 | 5 | 4 | 9 | 5 | 18 | 2 | 0.76 | 8 / 8 | 0 | - | 106024 | 98434 | 679.6 |
| chebyshev_mish_d7 | haiku | 2 | #5 (k2) | x1.986 | 70.7 | 86.6 | 1.95e-07 | 13 | 5 | 5 | 4 | 9 | 6 | 19 | 3 | 0.82 | 6 / 8 | 0 | chehab 2 | 79718 | 71049 | 529.8 |
| chebyshev_mish_d7 | haiku | 3 | #6 (k3) | x2.002 | 70.2 | 87.3 | 2.26e-07 | 13 | 5 | 5 | 4 | 9 | 6 | 19 | 3 | 0.72 | 2 / 8 | 0 | chehab 6 | 101294 | 93754 | 542.3 |
| chebyshev_mish_d7 | opus | 1 | #4 (k4) | x2.056 | 68.3 | 84.6 | 2.09e-07 | 14 | 5 | 5 | 4 | 9 | 5 | 18 | 4 | 0.86 | 7 / 8 | 0 | chehab 1 | 108447 | 100366 | 1369.3 |
| chebyshev_mish_d7 | opus | 2 | #4 (k5) | x1.802 | 77.9 | 96.2 | 1.21e-07 | 18 | 6 | 6 | 4 | 7 | 8 | 19 | 6 | 0.80 | 8 / 8 | 0 | - | 129292 | 122711 | 1563.1 |
| chebyshev_mish_d7 | opus | 3 | #3 (k6) | x1.799 | 78.1 | 96.0 | 1.64e-07 | 15 | 6 | 6 | 4 | 7 | 7 | 18 | 5 | 0.83 | 8 / 8 | 0 | - | 88773 | 81613 | 1205.8 |
| chebyshev_mish_d7 | sonnet | 1 | #2 (k7) | x2.001 | 70.2 | 87.9 | 3.05e-07 | 14 | 5 | 5 | 4 | 9 | 6 | 19 | 3 | 0.79 | 6 / 8 | 2 | - | 20831 | 15409 | 392.7 |
| chebyshev_mish_d7 | sonnet | 2 | #5 (k8) | x2.031 | 69.2 | 85.5 | 1.91e-07 | 13 | 5 | 5 | 4 | 9 | 5 | 18 | 2 | 0.84 | 6 / 8 | 1 | chehab 1 | 23826 | 18521 | 430.8 |
| chebyshev_mish_d7 | sonnet | 3 | #0 (baseline) | x1.000 | 140.5 | 168.1 | 1.58e-07 | 22 | 8 | 8 | 6 | 8 | 11 | 25 | 10 | 0.79 | 0 / 8 | 0 | chehab 8 | 17981 | 12543 | 190.7 |
| chebyshev_relu_d2 | haiku | 1 | #1 (k1) | x1.403 | 9.8 | 20.1 | 1.27e-07 | 7 | 3 | 3 | 1 | 2 | 0 | 3 | 1 | 0.49 | 3 / 8 | 5 | - | 46999 | 43277 | 326.3 |
| chebyshev_relu_d2 | opus | 1 | #1 (k2) | x1.405 | 9.8 | 20.1 | 1.64e-07 | 7 | 3 | 3 | 1 | 2 | 0 | 3 | 1 | 0.75 | 5 / 8 | 3 | - | 18641 | 15461 | 336.7 |
| chebyshev_relu_d2 | sonnet | 1 | #1 (k3) | x1.405 | 9.8 | 20.1 | 1.61e-07 | 7 | 3 | 3 | 1 | 2 | 0 | 3 | 1 | 0.48 | 3 / 8 | 3 | - | 15438 | 12864 | 234.3 |
| chebyshev_relu_d7 | haiku | 1 | #5 (k1) | x2.895 | 33.7 | 47.2 | 2.67e-07 | 9 | 4 | 4 | 3 | 4 | 2 | 9 | 0 | 0.67 | 6 / 8 | 1 | chehab 1 | 65796 | 60035 | 513.5 |
| chebyshev_relu_d7 | opus | 1 | #5 (k2) | x2.988 | 32.7 | 46.2 | 2.09e-07 | 11 | 4 | 4 | 3 | 4 | 2 | 9 | 0 | 0.74 | 7 / 8 | 1 | - | 48863 | 43523 | 743.9 |
| chebyshev_relu_d7 | sonnet | 1 | #3 (k3) | x2.918 | 33.5 | 47.0 | 2.34e-07 | 9 | 4 | 4 | 3 | 4 | 2 | 9 | 0 | 0.76 | 5 / 8 | 2 | chehab 1 | 16412 | 12182 | 302.6 |
| chebyshev_selu_d7 | haiku | 1 | #4 (k1) | x1.989 | 69.7 | 85.3 | 2.13e-07 | 13 | 5 | 5 | 4 | 9 | 6 | 19 | 4 | 0.77 | 4 / 8 | 0 | chehab 4 | 53509 | 45201 | 357.7 |
| chebyshev_selu_d7 | haiku | 2 | #1 (k2) | x1.977 | 70.2 | 86.7 | 1.68e-07 | 13 | 5 | 5 | 4 | 9 | 6 | 19 | 3 | 0.79 | 4 / 8 | 2 | chehab 1, wrong 1 | 63850 | 56219 | 424.0 |
| chebyshev_selu_d7 | haiku | 3 | #3 (k3) | x1.989 | 69.8 | 85.5 | 1.84e-07 | 14 | 5 | 5 | 4 | 9 | 6 | 19 | 5 | 0.70 | 5 / 8 | 2 | chehab 1 | 73503 | 65383 | 471.7 |
| chebyshev_selu_d7 | opus | 1 | #7 (k4) | x2.049 | 67.7 | 83.1 | 2.09e-07 | 15 | 5 | 5 | 4 | 9 | 5 | 18 | 2 | 0.83 | 7 / 8 | 0 | chehab 1 | 129985 | 121805 | 1692.6 |
| chebyshev_selu_d7 | opus | 2 | #8 (k5) | x2.424 | 57.2 | 72.5 | 2.01e-07 | 16 | 5 | 5 | 4 | 9 | 3 | 16 | 2 | 0.79 | 6 / 8 | 1 | chehab 1 | 83510 | 76568 | 1085.0 |
| chebyshev_selu_d7 | opus | 3 | #8 (k6) | x2.396 | 57.9 | 75.7 | 2.44e-07 | 12 | 5 | 5 | 4 | 5 | 3 | 12 | 5 | 0.81 | 6 / 8 | 0 | chehab 2 | 102261 | 95017 | 3921.7 |
| chebyshev_selu_d7 | sonnet | 1 | #6 (k7) | x2.035 | 68.2 | 84.0 | 1.88e-07 | 14 | 5 | 5 | 4 | 9 | 5 | 18 | 2 | 0.84 | 7 / 8 | 1 | - | 19115 | 13397 | 421.3 |
| chebyshev_selu_d7 | sonnet | 2 | #0 (baseline) | x1.000 | 138.7 | 164.6 | 1.57e-07 | 22 | 8 | 8 | 6 | 8 | 11 | 25 | 10 | 0.82 | 1 / 8 | 2 | chehab 5 | 19693 | 14274 | 236.0 |
| chebyshev_selu_d7 | sonnet | 3 | #1 (k8) | x1.984 | 69.9 | 85.6 | 2.57e-07 | 14 | 5 | 5 | 4 | 9 | 6 | 19 | 3 | 0.81 | 6 / 8 | 2 | - | 24637 | 19523 | 437.8 |
| chebyshev_sigmoid_d5 | haiku | 1 | #1 (k1) | x1.415 | 46.5 | 62.3 | 6.96e-08 | 13 | 5 | 5 | 3 | 4 | 5 | 12 | 6 | 0.75 | 6 / 8 | 0 | chehab 2 | 89182 | 80902 | 552.5 |
| chebyshev_sigmoid_d5 | haiku | 2 | #2 (k2) | x1.803 | 36.5 | 51.8 | 1.02e-07 | 22 | 5 | 5 | 3 | 2 | 0 | 5 | 4 | 0.67 | 4 / 8 | 1 | chehab 1 | 91436 | 83496 | 538.2 |
| chebyshev_sigmoid_d5 | haiku | 3 | #2 (k3) | x2.045 | 32.2 | 47.9 | 6.47e-08 | 12 | 5 | 5 | 3 | 2 | 0 | 5 | 4 | 0.62 | 2 / 8 | 3 | - | 72938 | 66812 | 409.0 |
| chebyshev_sigmoid_d5 | opus | 1 | #3 (k4) | x2.052 | 32.1 | 47.1 | 6.78e-08 | 12 | 5 | 5 | 3 | 2 | 0 | 5 | 4 | 0.66 | 6 / 8 | 1 | wrong 1 | 60443 | 56126 | 804.6 |
| chebyshev_sigmoid_d5 | opus | 2 | #2 (k5) | x2.031 | 32.4 | 47.7 | 7.33e-08 | 12 | 5 | 5 | 3 | 2 | 0 | 5 | 4 | 0.71 | 5 / 8 | 2 | - | 79087 | 73605 | 979.3 |
| chebyshev_sigmoid_d5 | opus | 3 | #2 (k6) | x2.044 | 32.2 | 47.6 | 6.82e-08 | 12 | 5 | 5 | 3 | 2 | 0 | 5 | 4 | 0.67 | 4 / 8 | 1 | - | 48537 | 43368 | 633.3 |
| chebyshev_sigmoid_d5 | sonnet | 1 | #4 (k7) | x2.055 | 32.0 | 47.4 | 5.89e-08 | 12 | 5 | 5 | 3 | 2 | 0 | 5 | 4 | 0.62 | 6 / 8 | 1 | - | 24152 | 19793 | 422.8 |
| chebyshev_sigmoid_d5 | sonnet | 2 | #1 (k8) | x1.955 | 33.7 | 46.4 | 1.72e-07 | 12 | 4 | 4 | 3 | 4 | 2 | 9 | 2 | 0.70 | 8 / 8 | 0 | - | 20911 | 15817 | 418.9 |
| chebyshev_sigmoid_d5 | sonnet | 3 | #3 (k9) | x2.038 | 32.3 | 47.6 | 4.65e-08 | 12 | 5 | 5 | 3 | 2 | 0 | 5 | 4 | 0.65 | 6 / 8 | 2 | - | 26320 | 22138 | 414.9 |
| chebyshev_sigmoid_d7 | haiku | 1 | #5 (k1) | x2.278 | 57.2 | 76.7 | 8.33e-08 | 17 | 6 | 6 | 4 | 3 | 1 | 8 | 6 | 0.79 | 5 / 8 | 2 | chehab 1 | 94314 | 88240 | 572.6 |
| chebyshev_sigmoid_d7 | haiku | 2 | #8 (k2) | x2.202 | 59.2 | 74.8 | 8.81e-08 | 15 | 5 | 5 | 4 | 5 | 2 | 11 | 6 | 0.87 | 7 / 8 | 1 | - | 80339 | 74850 | 537.7 |
| chebyshev_sigmoid_d7 | haiku | 3 | #6 (k3) | x2.422 | 53.8 | 71.6 | 8.05e-08 | 14 | 5 | 5 | 4 | 5 | 3 | 12 | 7 | 0.71 | 6 / 8 | 1 | chehab 1 | 110002 | 105011 | 638.3 |
| chebyshev_sigmoid_d7 | haiku | 4 | #7 (k4) | x2.578 | 50.6 | 66.5 | 6.54e-08 | 15 | 5 | 5 | 4 | 4 | 1 | 9 | 3 | 0.78 | 6 / 8 | 1 | chehab 1 | 114795 | 108260 | 1000.9 |
| chebyshev_sigmoid_d7 | haiku | 5 | #8 (k5) | x2.578 | 50.6 | 69.5 | 6.19e-08 | 13 | 5 | 5 | 4 | 3 | 2 | 9 | 3 | 0.79 | 7 / 8 | 0 | chehab 1 | 107298 | 101530 | 658.5 |
| chebyshev_sigmoid_d7 | opus | 1 | #8 (k6) | x1.702 | 76.6 | 112.1 | 6.22e-08 | 20 | 10 | 10 | 4 | 6 | 0 | 10 | 7 | 0.85 | 8 / 8 | 0 | - | 73406 | 67554 | 1010.3 |
| chebyshev_sigmoid_d7 | opus | 2 | #5 (k7) | x2.556 | 51.0 | 70.4 | 6.31e-08 | 17 | 6 | 6 | 4 | 2 | 0 | 6 | 7 | 0.57 | 7 / 8 | 0 | - | 65041 | 59237 | 888.4 |
| chebyshev_sigmoid_d7 | opus | 3 | #5 (k8) | x2.804 | 46.5 | 63.2 | 8.49e-08 | 13 | 5 | 5 | 4 | 3 | 1 | 8 | 3 | 0.81 | 7 / 8 | 1 | - | 66043 | 60005 | 932.1 |
| chebyshev_sigmoid_d7 | sonnet | 1 | #1 (k9) | x1.953 | 66.8 | 87.9 | 5.91e-08 | 16 | 6 | 6 | 4 | 4 | 4 | 12 | 6 | 0.83 | 6 / 8 | 0 | chehab 2 | 30208 | 25099 | 484.3 |
| chebyshev_sigmoid_d7 | sonnet | 2 | #1 (k10) | x2.037 | 64.0 | 84.0 | 8.29e-08 | 15 | 6 | 6 | 4 | 4 | 3 | 11 | 5 | 0.97 | 6 / 8 | 1 | chehab 1 | 23858 | 19266 | 427.9 |
| chebyshev_sigmoid_d7 | sonnet | 3 | #1 (k11) | x2.262 | 57.6 | 76.2 | 2.74e-07 | 16 | 5 | 5 | 4 | 5 | 3 | 12 | 5 | 0.98 | 5 / 8 | 2 | wrong 1 | 31709 | 25784 | 452.8 |
| chebyshev_silu_d7 | haiku | 1 | #2 (k1) | x2.863 | 33.8 | 47.5 | 1.57e-07 | 9 | 4 | 4 | 3 | 4 | 2 | 9 | 0 | 0.63 | 5 / 8 | 2 | chehab 1 | 100743 | 94680 | 610.6 |
| chebyshev_silu_d7 | haiku | 2 | #8 (k2) | x2.459 | 39.4 | 55.1 | 1.69e-07 | 15 | 5 | 5 | 3 | 3 | 1 | 7 | 4 | 1.61 | 7 / 8 | 0 | chehab 1 | 70096 | 63909 | 479.2 |
| chebyshev_silu_d7 | haiku | 3 | #2 (k3) | x2.915 | 33.2 | 46.1 | 1.48e-07 | 10 | 4 | 4 | 3 | 4 | 2 | 9 | 0 | 0.70 | 7 / 8 | 1 | - | 110196 | 104517 | 670.7 |
| chebyshev_silu_d7 | opus | 1 | #4 (k4) | x3.040 | 31.9 | 45.1 | 2.25e-07 | 11 | 4 | 4 | 3 | 4 | 2 | 9 | 0 | 0.96 | 7 / 8 | 1 | - | 44880 | 39925 | 708.3 |
| chebyshev_silu_d7 | opus | 2 | #3 (k5) | x2.959 | 32.7 | 46.3 | 1.87e-07 | 11 | 4 | 4 | 3 | 4 | 2 | 9 | 0 | 0.75 | 4 / 8 | 2 | chehab 2 | 46931 | 41429 | 646.2 |
| chebyshev_silu_d7 | opus | 3 | #4 (k6) | x2.568 | 37.7 | 53.9 | 2.42e-07 | 12 | 5 | 5 | 3 | 3 | 2 | 8 | 3 | 0.85 | 8 / 8 | 0 | - | 35503 | 30553 | 611.3 |
| chebyshev_silu_d7 | sonnet | 1 | #5 (k7) | x2.953 | 32.8 | 46.7 | 1.82e-07 | 13 | 4 | 4 | 3 | 4 | 2 | 9 | 1 | 0.77 | 6 / 8 | 1 | wrong 1 | 29966 | 24510 | 463.5 |
| chebyshev_silu_d7 | sonnet | 2 | #5 (k8) | x2.951 | 32.8 | 46.7 | 1.57e-07 | 10 | 4 | 4 | 3 | 4 | 2 | 9 | 0 | 0.80 | 8 / 8 | 0 | - | 23073 | 19050 | 459.1 |
| chebyshev_silu_d7 | sonnet | 3 | #3 (k9) | x2.966 | 32.7 | 46.6 | 1.61e-07 | 10 | 4 | 4 | 3 | 4 | 2 | 9 | 0 | 0.67 | 5 / 8 | 2 | chehab 1 | 15787 | 12128 | 302.2 |
| chebyshev_softplus_d7 | haiku | 1 | #6 (k1) | x2.992 | 32.7 | 46.3 | 1.28e-07 | 10 | 4 | 4 | 3 | 4 | 2 | 9 | 1 | 0.81 | 6 / 8 | 2 | - | 67068 | 61439 | 441.2 |
| chebyshev_softplus_d7 | haiku | 2 | #2 (k2) | x2.331 | 42.0 | 57.6 | 1.76e-07 | 14 | 5 | 5 | 3 | 4 | 2 | 9 | 3 | 0.71 | 5 / 8 | 2 | chehab 1 | 67930 | 62843 | 429.5 |
| chebyshev_softplus_d7 | haiku | 3 | #6 (k3) | x2.928 | 33.5 | 47.8 | 1.60e-07 | 11 | 4 | 4 | 3 | 4 | 2 | 9 | 1 | 0.76 | 6 / 8 | 0 | chehab 1, wrong 1 | 61780 | 55936 | 424.0 |
| chebyshev_softplus_d7 | opus | 1 | #8 (k4) | x2.580 | 38.0 | 53.4 | 1.54e-07 | 16 | 5 | 5 | 3 | 3 | 1 | 7 | 4 | 0.73 | 8 / 8 | 0 | - | 37434 | 32718 | 625.9 |
| chebyshev_softplus_d7 | opus | 2 | #6 (k5) | x2.992 | 32.8 | 46.4 | 1.64e-07 | 12 | 4 | 4 | 3 | 4 | 2 | 9 | 1 | 0.73 | 7 / 8 | 0 | chehab 1 | 45636 | 40596 | 698.8 |
| chebyshev_softplus_d7 | opus | 3 | #1 (k6) | x2.896 | 33.8 | 47.1 | 1.46e-07 | 10 | 4 | 4 | 3 | 4 | 2 | 9 | 1 | 0.83 | 8 / 8 | 0 | - | 43838 | 38715 | 710.7 |
| chebyshev_softplus_d7 | sonnet | 1 | #4 (k7) | x2.939 | 33.3 | 46.7 | 1.30e-07 | 10 | 4 | 4 | 3 | 4 | 2 | 9 | 0 | 0.71 | 6 / 8 | 1 | chehab 1 | 14349 | 10564 | 327.7 |
| chebyshev_softplus_d7 | sonnet | 2 | #3 (k8) | x2.894 | 33.9 | 46.9 | 1.21e-07 | 9 | 4 | 4 | 3 | 4 | 2 | 9 | 0 | 0.80 | 8 / 8 | 0 | - | 14885 | 11049 | 354.3 |
| chebyshev_softplus_d7 | sonnet | 3 | #2 (k9) | x2.350 | 41.7 | 59.0 | 1.39e-07 | 14 | 5 | 5 | 3 | 4 | 2 | 9 | 3 | 0.68 | 7 / 8 | 1 | - | 19057 | 15198 | 403.0 |
| elementwise_add | haiku | 1 | #0 (baseline) | x1.000 | 0.0 | 10.2 | 4.24e-08 | 1 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0.67 | 0 / 8 | 8 | - | 3406 | 1585 | 45.8 |
| elementwise_add | opus | 1 | #0 (baseline) | x1.000 | 0.0 | 10.2 | 4.24e-08 | 1 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0.67 | 0 / 8 | 8 | - | 2224 | 568 | 62.5 |
| elementwise_add | sonnet | 1 | #0 (baseline) | x1.000 | 0.0 | 10.2 | 4.24e-08 | 1 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0.67 | 0 / 8 | 8 | - | 1235 | 44 | 48.8 |
| elementwise_add_plain | haiku | 1 | #0 (baseline) | x1.000 | 0.4 | 6.0 | 2.90e-08 | 1 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0.65 | 0 / 8 | 8 | - | 4253 | 2382 | 55.2 |
| elementwise_add_plain | opus | 1 | #0 (baseline) | x1.000 | 0.4 | 6.0 | 2.90e-08 | 1 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0.65 | 0 / 8 | 8 | - | 2450 | 691 | 64.3 |
| elementwise_add_plain | sonnet | 1 | #0 (baseline) | x1.000 | 0.4 | 6.0 | 2.90e-08 | 1 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0.65 | 0 / 8 | 8 | - | 2509 | 1099 | 56.6 |
| elementwise_add_scalar | haiku | 1 | #0 (baseline) | x1.000 | 0.0 | 5.4 | 3.66e-08 | 1 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0.57 | 0 / 8 | 8 | - | 3991 | 2275 | 52.0 |
| elementwise_add_scalar | opus | 1 | #0 (baseline) | x1.000 | 0.0 | 5.4 | 3.66e-08 | 1 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0.57 | 0 / 8 | 8 | - | 2436 | 869 | 63.2 |
| elementwise_add_scalar | sonnet | 1 | #0 (baseline) | x1.000 | 0.0 | 5.4 | 3.66e-08 | 1 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0.57 | 0 / 8 | 8 | - | 2526 | 1216 | 56.4 |
| elementwise_mul | haiku | 1 | #0 (baseline) | x1.000 | 3.6 | 17.5 | 6.03e-08 | 1 | 1 | 1 | 1 | 0 | 0 | 1 | 0 | 0.48 | 0 / 8 | 8 | - | 3906 | 1820 | 53.1 |
| elementwise_mul | opus | 1 | #0 (baseline) | x1.000 | 3.6 | 17.5 | 6.03e-08 | 1 | 1 | 1 | 1 | 0 | 0 | 1 | 0 | 0.48 | 0 / 8 | 8 | - | 2724 | 908 | 65.6 |
| elementwise_mul | sonnet | 1 | #0 (baseline) | x1.000 | 3.6 | 17.5 | 6.03e-08 | 1 | 1 | 1 | 1 | 0 | 0 | 1 | 0 | 0.48 | 0 / 8 | 8 | - | 1671 | 202 | 54.0 |
| elementwise_mul_plain | haiku | 1 | #0 (baseline) | x1.000 | 1.4 | 8.4 | 6.42e-08 | 1 | 1 | 1 | 0 | 1 | 0 | 1 | 0 | 0.40 | 0 / 8 | 8 | - | 3998 | 2336 | 50.9 |
| elementwise_mul_plain | opus | 1 | #0 (baseline) | x1.000 | 1.4 | 8.4 | 6.42e-08 | 1 | 1 | 1 | 0 | 1 | 0 | 1 | 0 | 0.40 | 1 / 8 | 7 | - | 2257 | 559 | 83.1 |
| elementwise_mul_plain | sonnet | 1 | #0 (baseline) | x1.000 | 1.4 | 8.4 | 6.42e-08 | 1 | 1 | 1 | 0 | 1 | 0 | 1 | 0 | 0.40 | 0 / 8 | 8 | - | 1431 | 212 | 47.1 |
| elementwise_mul_scalar | haiku | 1 | #0 (baseline) | x1.000 | 0.8 | 7.7 | 5.90e-08 | 1 | 1 | 1 | 0 | 1 | 0 | 1 | 0 | 0.40 | 0 / 8 | 8 | - | 4762 | 3048 | 61.3 |
| elementwise_mul_scalar | opus | 1 | #0 (baseline) | x1.000 | 0.8 | 7.7 | 5.90e-08 | 1 | 1 | 1 | 0 | 1 | 0 | 1 | 0 | 0.40 | 1 / 8 | 7 | - | 2372 | 645 | 85.1 |
| elementwise_mul_scalar | sonnet | 1 | #0 (baseline) | x1.000 | 0.8 | 7.7 | 5.90e-08 | 1 | 1 | 1 | 0 | 1 | 0 | 1 | 0 | 0.40 | 0 / 8 | 8 | - | 2122 | 726 | 54.7 |
| embedding | haiku | 1 | #0 (baseline) | x1.000 | 75.1 | 84.3 | 5.04e-05 | 21 | 2 | 2 | 0 | 6 | 0 | 6 | 0 | 0.65 | 4 / 8 | 1 | compile 1, chehab 2 | 92876 | 88168 | 826.6 |
| embedding | opus | 1 | #0 (baseline) | x1.000 | 75.1 | 84.3 | 5.04e-05 | 21 | 2 | 2 | 0 | 6 | 0 | 6 | 0 | 0.65 | 7 / 8 | 1 | - | 70752 | 65873 | 1415.7 |
| embedding | sonnet | 1 | #0 (baseline) | x1.000 | 75.1 | 84.3 | 5.04e-05 | 21 | 2 | 2 | 0 | 6 | 0 | 6 | 0 | 0.65 | 1 / 8 | 6 | wrong 1 | 33893 | 30425 | 431.1 |
| extract | haiku | 1 | #0 (baseline) | x1.000 | 2.7 | 9.5 | 7.55e-06 | 2 | 1 | 1 | 0 | 1 | 0 | 1 | 0 | 0.43 | 1 / 8 | 6 | wrong 1 | 8611 | 6244 | 108.1 |
| extract | opus | 1 | #0 (baseline) | x1.000 | 2.7 | 9.5 | 7.55e-06 | 2 | 1 | 1 | 0 | 1 | 0 | 1 | 0 | 0.43 | 4 / 8 | 4 | - | 3824 | 1847 | 136.4 |
| extract | sonnet | 1 | #0 (baseline) | x1.000 | 2.7 | 9.5 | 7.55e-06 | 2 | 1 | 1 | 0 | 1 | 0 | 1 | 0 | 0.43 | 0 / 8 | 8 | - | 5335 | 3665 | 80.4 |
| extract_linear | haiku | 1 | #0 (baseline) | x1.000 | 2.7 | 9.5 | 9.11e-06 | 2 | 1 | 1 | 0 | 1 | 0 | 1 | 0 | 0.48 | 1 / 8 | 7 | - | 8958 | 6738 | 103.8 |
| extract_linear | opus | 1 | #0 (baseline) | x1.000 | 2.7 | 9.5 | 9.11e-06 | 2 | 1 | 1 | 0 | 1 | 0 | 1 | 0 | 0.48 | 2 / 8 | 6 | - | 4275 | 2417 | 119.2 |
| extract_linear | sonnet | 1 | #0 (baseline) | x1.000 | 2.7 | 9.5 | 9.11e-06 | 2 | 1 | 1 | 0 | 1 | 0 | 1 | 0 | 0.48 | 0 / 8 | 8 | - | 4323 | 2669 | 68.9 |
| extract_sparse | haiku | 1 | #2 (k1) | x1.277 | 6.5 | 20.1 | 9.30e-06 | 3 | 1 | 1 | 0 | 2 | 1 | 3 | 0 | 0.60 | 7 / 8 | 1 | - | 52358 | 47641 | 430.8 |
| extract_sparse | opus | 1 | #2 (k2) | x1.278 | 6.5 | 20.1 | 9.75e-06 | 4 | 1 | 1 | 0 | 2 | 1 | 3 | 0 | 0.62 | 7 / 8 | 1 | - | 17912 | 13540 | 364.4 |
| extract_sparse | sonnet | 1 | #1 (k3) | x1.283 | 6.4 | 20.0 | 6.62e-06 | 4 | 1 | 1 | 0 | 2 | 1 | 3 | 0 | 0.62 | 7 / 8 | 1 | - | 11285 | 8383 | 277.1 |
| quad | haiku | 1 | #0 (baseline) | x1.000 | 3.5 | 10.5 | 1.07e-07 | 1 | 1 | 1 | 1 | 0 | 0 | 1 | 0 | 0.59 | 1 / 8 | 7 | - | 4033 | 2015 | 74.5 |
| quad | opus | 1 | #1 (k1) | x1.021 | 3.4 | 10.4 | 9.16e-08 | 1 | 1 | 1 | 1 | 0 | 0 | 1 | 0 | 0.42 | 1 / 8 | 7 | - | 1737 | 300 | 74.7 |
| quad | sonnet | 1 | #0 (baseline) | x1.000 | 3.5 | 10.5 | 1.07e-07 | 1 | 1 | 1 | 1 | 0 | 0 | 1 | 0 | 0.59 | 1 / 8 | 7 | - | 1255 | 0 | 67.9 |
| relu_composite | haiku | 1 | #0 (baseline) | x1.000 | 368.9 | 402.4 | 8.77e-07 | 17 | 10 | 10 | 16 | 12 | 9 | 37 | 9 | 0.69 | 3 / 8 | 2 | chehab 3 | 136380 | 128844 | 838.0 |
| relu_composite | opus | 1 | #0 (baseline) | x1.000 | 368.9 | 402.4 | 8.77e-07 | 17 | 10 | 10 | 16 | 12 | 9 | 37 | 9 | 0.69 | 3 / 8 | 1 | chehab 2, held-out 1 | 120606 | 112949 | 1466.7 |
| relu_composite | sonnet | 1 | #8 (k1) | x1.013 | 364.2 | 398.2 | 6.30e-07 | 18 | 10 | 10 | 15 | 11 | 8 | 34 | 16 | 1.28 | 2 / 8 | 4 | chehab 2 | 28515 | 22155 | 364.4 |
| table_mult | haiku | 1 | #0 (baseline) | x1.000 | 37.0 | 43.9 | 8.39e-05 | 17 | 1 | 1 | 0 | 3 | 0 | 3 | 0 | 0.66 | 1 / 8 | 7 | - | 48732 | 45958 | 369.0 |
| table_mult | opus | 1 | #0 (baseline) | x1.000 | 37.0 | 43.9 | 8.39e-05 | 17 | 1 | 1 | 0 | 3 | 0 | 3 | 0 | 0.66 | 6 / 8 | 2 | - | 13870 | 10083 | 655.7 |
| table_mult | sonnet | 1 | #0 (baseline) | x1.000 | 37.0 | 43.9 | 8.39e-05 | 17 | 1 | 1 | 0 | 3 | 0 | 3 | 0 | 0.66 | 3 / 8 | 5 | - | 10253 | 7412 | 354.4 |

## Per task and strategy (3 runs each)

| task | strategy | runs | speedup |
|---|---|---:|---|
| batchnorm1d | haiku | 1 | median x1.000, mean x1.000, min x1.000, max x1.000, sd 0.000 |
| batchnorm2d | haiku | 1 | median x0.978, mean x0.978, min x0.978, max x0.978, sd 0.000 |
| chebyshev_elu_d7 | haiku | 5 | median x2.030, mean x2.028, min x2.002, max x2.041, sd 0.016 |
| chebyshev_gelu_d7 | haiku | 5 | median x2.483, mean x2.619, min x2.327, max x2.945, sd 0.284 |
| chebyshev_hardshrink_d7 | haiku | 3 | median x2.429, mean x2.358, min x1.881, max x2.765, sd 0.446 |
| chebyshev_mish_d7 | haiku | 3 | median x2.002, mean x2.008, min x1.986, max x2.037, sd 0.026 |
| chebyshev_relu_d2 | haiku | 1 | median x1.403, mean x1.403, min x1.403, max x1.403, sd 0.000 |
| chebyshev_relu_d7 | haiku | 1 | median x2.895, mean x2.895, min x2.895, max x2.895, sd 0.000 |
| chebyshev_selu_d7 | haiku | 3 | median x1.989, mean x1.985, min x1.977, max x1.989, sd 0.007 |
| chebyshev_sigmoid_d5 | haiku | 3 | median x1.803, mean x1.754, min x1.415, max x2.045, sd 0.318 |
| chebyshev_sigmoid_d7 | haiku | 5 | median x2.422, mean x2.411, min x2.202, max x2.578, sd 0.171 |
| chebyshev_silu_d7 | haiku | 3 | median x2.863, mean x2.745, min x2.459, max x2.915, sd 0.250 |
| chebyshev_softplus_d7 | haiku | 3 | median x2.928, mean x2.750, min x2.331, max x2.992, sd 0.365 |
| elementwise_add | haiku | 1 | median x1.000, mean x1.000, min x1.000, max x1.000, sd 0.000 |
| elementwise_add_plain | haiku | 1 | median x1.000, mean x1.000, min x1.000, max x1.000, sd 0.000 |
| elementwise_add_scalar | haiku | 1 | median x1.000, mean x1.000, min x1.000, max x1.000, sd 0.000 |
| elementwise_mul | haiku | 1 | median x1.000, mean x1.000, min x1.000, max x1.000, sd 0.000 |
| elementwise_mul_plain | haiku | 1 | median x1.000, mean x1.000, min x1.000, max x1.000, sd 0.000 |
| elementwise_mul_scalar | haiku | 1 | median x1.000, mean x1.000, min x1.000, max x1.000, sd 0.000 |
| embedding | haiku | 1 | median x1.000, mean x1.000, min x1.000, max x1.000, sd 0.000 |
| extract | haiku | 1 | median x1.000, mean x1.000, min x1.000, max x1.000, sd 0.000 |
| extract_linear | haiku | 1 | median x1.000, mean x1.000, min x1.000, max x1.000, sd 0.000 |
| extract_sparse | haiku | 1 | median x1.277, mean x1.277, min x1.277, max x1.277, sd 0.000 |
| quad | haiku | 1 | median x1.000, mean x1.000, min x1.000, max x1.000, sd 0.000 |
| relu_composite | haiku | 1 | median x1.000, mean x1.000, min x1.000, max x1.000, sd 0.000 |
| table_mult | haiku | 1 | median x1.000, mean x1.000, min x1.000, max x1.000, sd 0.000 |
| batchnorm1d | opus | 1 | median x1.000, mean x1.000, min x1.000, max x1.000, sd 0.000 |
| batchnorm2d | opus | 1 | median x1.000, mean x1.000, min x1.000, max x1.000, sd 0.000 |
| chebyshev_elu_d7 | opus | 3 | median x1.971, mean x2.021, min x1.921, max x2.169, sd 0.131 |
| chebyshev_gelu_d7 | opus | 3 | median x2.560, mean x2.200, min x1.449, max x2.592, sd 0.651 |
| chebyshev_hardshrink_d7 | opus | 3 | median x2.572, mean x2.574, min x2.561, max x2.588, sd 0.014 |
| chebyshev_mish_d7 | opus | 3 | median x1.802, mean x1.886, min x1.799, max x2.056, sd 0.147 |
| chebyshev_relu_d2 | opus | 1 | median x1.405, mean x1.405, min x1.405, max x1.405, sd 0.000 |
| chebyshev_relu_d7 | opus | 1 | median x2.988, mean x2.988, min x2.988, max x2.988, sd 0.000 |
| chebyshev_selu_d7 | opus | 3 | median x2.396, mean x2.290, min x2.049, max x2.424, sd 0.209 |
| chebyshev_sigmoid_d5 | opus | 3 | median x2.044, mean x2.043, min x2.031, max x2.052, sd 0.011 |
| chebyshev_sigmoid_d7 | opus | 3 | median x2.556, mean x2.354, min x1.702, max x2.804, sd 0.578 |
| chebyshev_silu_d7 | opus | 3 | median x2.959, mean x2.856, min x2.568, max x3.040, sd 0.252 |
| chebyshev_softplus_d7 | opus | 3 | median x2.896, mean x2.823, min x2.580, max x2.992, sd 0.215 |
| elementwise_add | opus | 1 | median x1.000, mean x1.000, min x1.000, max x1.000, sd 0.000 |
| elementwise_add_plain | opus | 1 | median x1.000, mean x1.000, min x1.000, max x1.000, sd 0.000 |
| elementwise_add_scalar | opus | 1 | median x1.000, mean x1.000, min x1.000, max x1.000, sd 0.000 |
| elementwise_mul | opus | 1 | median x1.000, mean x1.000, min x1.000, max x1.000, sd 0.000 |
| elementwise_mul_plain | opus | 1 | median x1.000, mean x1.000, min x1.000, max x1.000, sd 0.000 |
| elementwise_mul_scalar | opus | 1 | median x1.000, mean x1.000, min x1.000, max x1.000, sd 0.000 |
| embedding | opus | 1 | median x1.000, mean x1.000, min x1.000, max x1.000, sd 0.000 |
| extract | opus | 1 | median x1.000, mean x1.000, min x1.000, max x1.000, sd 0.000 |
| extract_linear | opus | 1 | median x1.000, mean x1.000, min x1.000, max x1.000, sd 0.000 |
| extract_sparse | opus | 1 | median x1.278, mean x1.278, min x1.278, max x1.278, sd 0.000 |
| quad | opus | 1 | median x1.021, mean x1.021, min x1.021, max x1.021, sd 0.000 |
| relu_composite | opus | 1 | median x1.000, mean x1.000, min x1.000, max x1.000, sd 0.000 |
| table_mult | opus | 1 | median x1.000, mean x1.000, min x1.000, max x1.000, sd 0.000 |
| batchnorm1d | sonnet | 1 | median x1.000, mean x1.000, min x1.000, max x1.000, sd 0.000 |
| batchnorm2d | sonnet | 1 | median x1.000, mean x1.000, min x1.000, max x1.000, sd 0.000 |
| chebyshev_elu_d7 | sonnet | 3 | median x1.988, mean x1.863, min x1.577, max x2.023, sd 0.248 |
| chebyshev_gelu_d7 | sonnet | 3 | median x2.332, mean x2.399, min x1.920, max x2.945, sd 0.516 |
| chebyshev_hardshrink_d7 | sonnet | 3 | median x2.253, mean x2.244, min x2.165, max x2.314, sd 0.075 |
| chebyshev_mish_d7 | sonnet | 3 | median x2.001, mean x1.677, min x1.000, max x2.031, sd 0.587 |
| chebyshev_relu_d2 | sonnet | 1 | median x1.405, mean x1.405, min x1.405, max x1.405, sd 0.000 |
| chebyshev_relu_d7 | sonnet | 1 | median x2.918, mean x2.918, min x2.918, max x2.918, sd 0.000 |
| chebyshev_selu_d7 | sonnet | 3 | median x1.984, mean x1.673, min x1.000, max x2.035, sd 0.583 |
| chebyshev_sigmoid_d5 | sonnet | 3 | median x2.038, mean x2.016, min x1.955, max x2.055, sd 0.053 |
| chebyshev_sigmoid_d7 | sonnet | 3 | median x2.037, mean x2.084, min x1.953, max x2.262, sd 0.160 |
| chebyshev_silu_d7 | sonnet | 3 | median x2.953, mean x2.956, min x2.951, max x2.966, sd 0.008 |
| chebyshev_softplus_d7 | sonnet | 3 | median x2.894, mean x2.728, min x2.350, max x2.939, sd 0.328 |
| elementwise_add | sonnet | 1 | median x1.000, mean x1.000, min x1.000, max x1.000, sd 0.000 |
| elementwise_add_plain | sonnet | 1 | median x1.000, mean x1.000, min x1.000, max x1.000, sd 0.000 |
| elementwise_add_scalar | sonnet | 1 | median x1.000, mean x1.000, min x1.000, max x1.000, sd 0.000 |
| elementwise_mul | sonnet | 1 | median x1.000, mean x1.000, min x1.000, max x1.000, sd 0.000 |
| elementwise_mul_plain | sonnet | 1 | median x1.000, mean x1.000, min x1.000, max x1.000, sd 0.000 |
| elementwise_mul_scalar | sonnet | 1 | median x1.000, mean x1.000, min x1.000, max x1.000, sd 0.000 |
| embedding | sonnet | 1 | median x1.000, mean x1.000, min x1.000, max x1.000, sd 0.000 |
| extract | sonnet | 1 | median x1.000, mean x1.000, min x1.000, max x1.000, sd 0.000 |
| extract_linear | sonnet | 1 | median x1.000, mean x1.000, min x1.000, max x1.000, sd 0.000 |
| extract_sparse | sonnet | 1 | median x1.283, mean x1.283, min x1.283, max x1.283, sd 0.000 |
| quad | sonnet | 1 | median x1.000, mean x1.000, min x1.000, max x1.000, sd 0.000 |
| relu_composite | sonnet | 1 | median x1.013, mean x1.013, min x1.013, max x1.013, sd 0.000 |
| table_mult | sonnet | 1 | median x1.000, mean x1.000, min x1.000, max x1.000, sd 0.000 |

## Across tasks (geometric mean of the per-task mean speedups)

| strategy | tasks | geometric mean |
|---|---:|---:|
| haiku | 26 | x1.414 |
| opus | 26 | x1.428 |
| sonnet | 26 | x1.388 |

## Approximation error (task property, separate from FHE error)

| task | max_fit_error (fitting grid) | approx. error, fixed points | approx. error, dense points |
|---|---:|---:|---:|
| batchnorm1d | - | - | - |
| batchnorm2d | - | - | - |
| chebyshev_elu_d7 | 0.0427493 | 0.0427493 | 0.0330832 |
| chebyshev_gelu_d7 | 0.0581538 | 0.0581538 | 0.0470933 |
| chebyshev_hardshrink_d7 | 0.464224 | 0.050663 | 0.455449 |
| chebyshev_mish_d7 | 0.03818 | 0.03818 | 0.0258338 |
| chebyshev_relu_d2 | 0.375091 | 0.375091 | 0.359784 |
| chebyshev_relu_d7 | 0.17094 | 0.17094 | 0.155796 |
| chebyshev_selu_d7 | 0.0947603 | 0.0947603 | 0.0892641 |
| chebyshev_sigmoid_d5 | 0.0105848 | 0.0105848 | 0.00897247 |
| chebyshev_sigmoid_d7 | 0.002824 | 0.002824 | 0.0021268 |
| chebyshev_silu_d7 | 0.0172606 | 0.0172606 | 0.013855 |
| chebyshev_softplus_d7 | 0.00357233 | 0.00357233 | 0.00285445 |
| elementwise_add | - | - | - |
| elementwise_add_plain | - | - | - |
| elementwise_add_scalar | - | - | - |
| elementwise_mul | - | - | - |
| elementwise_mul_plain | - | - | - |
| elementwise_mul_scalar | - | - | - |
| embedding | - | - | - |
| extract | - | - | - |
| extract_linear | - | - | - |
| extract_sparse | - | - | - |
| quad | - | - | - |
| relu_composite | 0.0118026 | 2.30792e-06 | 0.0118007 |
| table_mult | - | - | - |

## Re-scored kernels

| task | kernel | used by | correct | eval (ms) | speedup |
|---|---|---|---|---:|---:|
| batchnorm1d | baseline | haiku/run1 opus/run1 sonnet/run1 | True | 2.09 | 1.0 |
| batchnorm2d | baseline | opus/run1 sonnet/run1 | True | 2.26 | 1.0 |
| batchnorm2d | k1 | haiku/run1 | True | 2.31 | 0.978 |
| chebyshev_elu_d7 | baseline | baseline | True | 140.07 | 1.0 |
| chebyshev_elu_d7 | k1 | haiku/run1 | True | 69.0 | 2.03 |
| chebyshev_elu_d7 | k2 | haiku/run2 | True | 69.97 | 2.002 |
| chebyshev_elu_d7 | k3 | haiku/run3 | True | 69.08 | 2.028 |
| chebyshev_elu_d7 | k4 | haiku/run4 | True | 68.63 | 2.041 |
| chebyshev_elu_d7 | k5 | haiku/run5 | True | 68.68 | 2.039 |
| chebyshev_elu_d7 | k6 | opus/run1 | True | 71.05 | 1.971 |
| chebyshev_elu_d7 | k7 | opus/run2 | True | 64.58 | 2.169 |
| chebyshev_elu_d7 | k8 | opus/run3 | True | 72.91 | 1.921 |
| chebyshev_elu_d7 | k9 | sonnet/run1 | True | 69.24 | 2.023 |
| chebyshev_elu_d7 | k10 | sonnet/run2 | True | 88.81 | 1.577 |
| chebyshev_elu_d7 | k11 | sonnet/run3 | True | 70.48 | 1.988 |
| chebyshev_gelu_d7 | baseline | baseline | True | 97.46 | 1.0 |
| chebyshev_gelu_d7 | k1 | haiku/run1 | True | 39.25 | 2.483 |
| chebyshev_gelu_d7 | k2 | haiku/run2 | True | 33.59 | 2.901 |
| chebyshev_gelu_d7 | k3 | haiku/run3 | True | 41.88 | 2.327 |
| chebyshev_gelu_d7 | k4 | haiku/run4 | True | 39.93 | 2.441 |
| chebyshev_gelu_d7 | k5 | haiku/run5 | True | 33.09 | 2.945 |
| chebyshev_gelu_d7 | k6 | opus/run1 | True | 67.28 | 1.449 |
| chebyshev_gelu_d7 | k7 | opus/run2 | True | 38.07 | 2.56 |
| chebyshev_gelu_d7 | k8 | opus/run3 | True | 37.59 | 2.592 |
| chebyshev_gelu_d7 | k9 | sonnet/run1 | True | 50.76 | 1.92 |
| chebyshev_gelu_d7 | k10 | sonnet/run2 | True | 33.09 | 2.945 |
| chebyshev_gelu_d7 | k11 | sonnet/run3 | True | 41.79 | 2.332 |
| chebyshev_hardshrink_d7 | baseline | baseline | True | 124.76 | 1.0 |
| chebyshev_hardshrink_d7 | k1 | haiku/run1 | True | 45.12 | 2.765 |
| chebyshev_hardshrink_d7 | k2 | haiku/run2 | True | 51.36 | 2.429 |
| chebyshev_hardshrink_d7 | k3 | haiku/run3 | True | 66.31 | 1.881 |
| chebyshev_hardshrink_d7 | k4 | opus/run1 | True | 48.51 | 2.572 |
| chebyshev_hardshrink_d7 | k5 | opus/run2 | True | 48.71 | 2.561 |
| chebyshev_hardshrink_d7 | k6 | opus/run3 | True | 48.2 | 2.588 |
| chebyshev_hardshrink_d7 | k7 | sonnet/run1 | True | 55.37 | 2.253 |
| chebyshev_hardshrink_d7 | k8 | sonnet/run2 | True | 57.62 | 2.165 |
| chebyshev_hardshrink_d7 | k9 | sonnet/run3 | True | 53.9 | 2.314 |
| chebyshev_mish_d7 | baseline | sonnet/run3 | True | 140.46 | 1.0 |
| chebyshev_mish_d7 | k1 | haiku/run1 | True | 68.95 | 2.037 |
| chebyshev_mish_d7 | k2 | haiku/run2 | True | 70.72 | 1.986 |
| chebyshev_mish_d7 | k3 | haiku/run3 | True | 70.16 | 2.002 |
| chebyshev_mish_d7 | k4 | opus/run1 | True | 68.33 | 2.056 |
| chebyshev_mish_d7 | k5 | opus/run2 | True | 77.94 | 1.802 |
| chebyshev_mish_d7 | k6 | opus/run3 | True | 78.06 | 1.799 |
| chebyshev_mish_d7 | k7 | sonnet/run1 | True | 70.19 | 2.001 |
| chebyshev_mish_d7 | k8 | sonnet/run2 | True | 69.16 | 2.031 |
| chebyshev_relu_d2 | baseline | baseline | True | 13.72 | 1.0 |
| chebyshev_relu_d2 | k1 | haiku/run1 | True | 9.78 | 1.403 |
| chebyshev_relu_d2 | k2 | opus/run1 | True | 9.76 | 1.405 |
| chebyshev_relu_d2 | k3 | sonnet/run1 | True | 9.77 | 1.405 |
| chebyshev_relu_d7 | baseline | baseline | True | 97.62 | 1.0 |
| chebyshev_relu_d7 | k1 | haiku/run1 | True | 33.72 | 2.895 |
| chebyshev_relu_d7 | k2 | opus/run1 | True | 32.67 | 2.988 |
| chebyshev_relu_d7 | k3 | sonnet/run1 | True | 33.46 | 2.918 |
| chebyshev_selu_d7 | baseline | sonnet/run2 | True | 138.74 | 1.0 |
| chebyshev_selu_d7 | k1 | haiku/run1 | True | 69.74 | 1.989 |
| chebyshev_selu_d7 | k2 | haiku/run2 | True | 70.19 | 1.977 |
| chebyshev_selu_d7 | k3 | haiku/run3 | True | 69.77 | 1.989 |
| chebyshev_selu_d7 | k4 | opus/run1 | True | 67.71 | 2.049 |
| chebyshev_selu_d7 | k5 | opus/run2 | True | 57.24 | 2.424 |
| chebyshev_selu_d7 | k6 | opus/run3 | True | 57.91 | 2.396 |
| chebyshev_selu_d7 | k7 | sonnet/run1 | True | 68.17 | 2.035 |
| chebyshev_selu_d7 | k8 | sonnet/run3 | True | 69.94 | 1.984 |
| chebyshev_sigmoid_d5 | baseline | baseline | True | 65.82 | 1.0 |
| chebyshev_sigmoid_d5 | k1 | haiku/run1 | True | 46.52 | 1.415 |
| chebyshev_sigmoid_d5 | k2 | haiku/run2 | True | 36.51 | 1.803 |
| chebyshev_sigmoid_d5 | k3 | haiku/run3 | True | 32.19 | 2.045 |
| chebyshev_sigmoid_d5 | k4 | opus/run1 | True | 32.07 | 2.052 |
| chebyshev_sigmoid_d5 | k5 | opus/run2 | True | 32.4 | 2.031 |
| chebyshev_sigmoid_d5 | k6 | opus/run3 | True | 32.2 | 2.044 |
| chebyshev_sigmoid_d5 | k7 | sonnet/run1 | True | 32.03 | 2.055 |
| chebyshev_sigmoid_d5 | k8 | sonnet/run2 | True | 33.66 | 1.955 |
| chebyshev_sigmoid_d5 | k9 | sonnet/run3 | True | 32.3 | 2.038 |
| chebyshev_sigmoid_d7 | baseline | baseline | True | 130.39 | 1.0 |
| chebyshev_sigmoid_d7 | k1 | haiku/run1 | True | 57.23 | 2.278 |
| chebyshev_sigmoid_d7 | k2 | haiku/run2 | True | 59.23 | 2.202 |
| chebyshev_sigmoid_d7 | k3 | haiku/run3 | True | 53.85 | 2.422 |
| chebyshev_sigmoid_d7 | k4 | haiku/run4 | True | 50.58 | 2.578 |
| chebyshev_sigmoid_d7 | k5 | haiku/run5 | True | 50.59 | 2.578 |
| chebyshev_sigmoid_d7 | k6 | opus/run1 | True | 76.6 | 1.702 |
| chebyshev_sigmoid_d7 | k7 | opus/run2 | True | 51.02 | 2.556 |
| chebyshev_sigmoid_d7 | k8 | opus/run3 | True | 46.5 | 2.804 |
| chebyshev_sigmoid_d7 | k9 | sonnet/run1 | True | 66.76 | 1.953 |
| chebyshev_sigmoid_d7 | k10 | sonnet/run2 | True | 64.01 | 2.037 |
| chebyshev_sigmoid_d7 | k11 | sonnet/run3 | True | 57.64 | 2.262 |
| chebyshev_silu_d7 | baseline | baseline | True | 96.85 | 1.0 |
| chebyshev_silu_d7 | k1 | haiku/run1 | True | 33.83 | 2.863 |
| chebyshev_silu_d7 | k2 | haiku/run2 | True | 39.39 | 2.459 |
| chebyshev_silu_d7 | k3 | haiku/run3 | True | 33.22 | 2.915 |
| chebyshev_silu_d7 | k4 | opus/run1 | True | 31.86 | 3.04 |
| chebyshev_silu_d7 | k5 | opus/run2 | True | 32.73 | 2.959 |
| chebyshev_silu_d7 | k6 | opus/run3 | True | 37.71 | 2.568 |
| chebyshev_silu_d7 | k7 | sonnet/run1 | True | 32.8 | 2.953 |
| chebyshev_silu_d7 | k8 | sonnet/run2 | True | 32.82 | 2.951 |
| chebyshev_silu_d7 | k9 | sonnet/run3 | True | 32.66 | 2.966 |
| chebyshev_softplus_d7 | baseline | baseline | True | 97.98 | 1.0 |
| chebyshev_softplus_d7 | k1 | haiku/run1 | True | 32.74 | 2.992 |
| chebyshev_softplus_d7 | k2 | haiku/run2 | True | 42.04 | 2.331 |
| chebyshev_softplus_d7 | k3 | haiku/run3 | True | 33.46 | 2.928 |
| chebyshev_softplus_d7 | k4 | opus/run1 | True | 37.98 | 2.58 |
| chebyshev_softplus_d7 | k5 | opus/run2 | True | 32.75 | 2.992 |
| chebyshev_softplus_d7 | k6 | opus/run3 | True | 33.83 | 2.896 |
| chebyshev_softplus_d7 | k7 | sonnet/run1 | True | 33.33 | 2.939 |
| chebyshev_softplus_d7 | k8 | sonnet/run2 | True | 33.85 | 2.894 |
| chebyshev_softplus_d7 | k9 | sonnet/run3 | True | 41.69 | 2.35 |
| elementwise_add | baseline | haiku/run1 opus/run1 sonnet/run1 | True | 0.04 | 1.0 |
| elementwise_add_plain | baseline | haiku/run1 opus/run1 sonnet/run1 | True | 0.44 | 1.0 |
| elementwise_add_scalar | baseline | haiku/run1 opus/run1 sonnet/run1 | True | 0.02 | 1.0 |
| elementwise_mul | baseline | haiku/run1 opus/run1 sonnet/run1 | True | 3.57 | 1.0 |
| elementwise_mul_plain | baseline | haiku/run1 opus/run1 sonnet/run1 | True | 1.45 | 1.0 |
| elementwise_mul_scalar | baseline | haiku/run1 opus/run1 sonnet/run1 | True | 0.85 | 1.0 |
| embedding | baseline | haiku/run1 opus/run1 sonnet/run1 | True | 75.1 | 1.0 |
| extract | baseline | haiku/run1 opus/run1 sonnet/run1 | True | 2.67 | 1.0 |
| extract_linear | baseline | haiku/run1 opus/run1 sonnet/run1 | True | 2.68 | 1.0 |
| extract_sparse | baseline | baseline | True | 8.25 | 1.0 |
| extract_sparse | k1 | haiku/run1 | True | 6.46 | 1.277 |
| extract_sparse | k2 | opus/run1 | True | 6.46 | 1.278 |
| extract_sparse | k3 | sonnet/run1 | True | 6.43 | 1.283 |
| quad | baseline | haiku/run1 sonnet/run1 | True | 3.52 | 1.0 |
| quad | k1 | opus/run1 | True | 3.44 | 1.021 |
| relu_composite | baseline | haiku/run1 opus/run1 | True | 368.95 | 1.0 |
| relu_composite | k1 | sonnet/run1 | True | 364.23 | 1.013 |
| table_mult | baseline | haiku/run1 opus/run1 sonnet/run1 | True | 36.99 | 1.0 |
