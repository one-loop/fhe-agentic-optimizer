# Phase-2 pilot: neutral re-scoring

Commit d1eb0ef9ee (dirty), Darwin arm64 arm, Apple clang version 17.0.0 (clang-1700.3.19.1), SEAL 4.1. Re-scored 2026-11-08T17:59:14.

Speedup = baseline eval-only median / kernel eval-only median, both from the same neutral session (1 warmup + 10 measured processes of ./main --repeats 10, interleaved). The final kernel of each run is the strategy's own selection; re-scoring never re-selects. E2E = CHEHAB execution_time_(ms) (parse + encode + encrypt + fhe()). Compile time: median of 3, reported only.

## Every run

| task | strategy | run | final | speedup | eval (ms) | e2e (ms) | FHE err | depth | xdepth | levels | ct x ct | ct x pt | align | rescale | mod sw | C (ms) | correct / 8 | dup | failures | out tok | think tok | search (s) |
|---|---|---:|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---|---:|---:|---:|
| batchnorm1d | haiku | 1 | #0 (baseline) | x1.000 | 2.2 | 9.5 | 4.08e-08 | 2 | 1 | 1 | 0 | 1 | 0 | 1 | 0 | 0.31 | 1 / 8 | 7 | - | 5779 | 3656 | 79.9 |
| batchnorm1d | sonnet | 1 | #0 (baseline) | x1.000 | 2.2 | 9.5 | 4.08e-08 | 2 | 1 | 1 | 0 | 1 | 0 | 1 | 0 | 0.31 | 3 / 8 | 5 | - | 3781 | 2163 | 92.7 |
| batchnorm2d | haiku | 1 | #5 (k1) | x1.005 | 2.2 | 9.3 | 3.84e-08 | 2 | 1 | 1 | 0 | 1 | 0 | 1 | 0 | 0.39 | 1 / 8 | 7 | - | 6489 | 4238 | 83.9 |
| batchnorm2d | sonnet | 1 | #0 (baseline) | x1.000 | 2.2 | 9.4 | 4.19e-08 | 2 | 1 | 1 | 0 | 1 | 0 | 1 | 0 | 0.35 | 2 / 8 | 6 | - | 3764 | 2141 | 90.0 |
| chebyshev_elu_d7 | haiku | 1 | #3 (k1) | x2.020 | 70.4 | 86.5 | 2.09e-07 | 13 | 5 | 5 | 4 | 9 | 5 | 18 | 2 | 0.63 | 8 / 8 | 0 | - | 96244 | 87692 | 628.6 |
| chebyshev_elu_d7 | haiku | 2 | #2 (k2) | x1.983 | 71.7 | 87.8 | 1.87e-07 | 13 | 5 | 5 | 4 | 9 | 6 | 19 | 3 | 0.57 | 6 / 8 | 1 | chehab 1 | 83440 | 74830 | 523.8 |
| chebyshev_elu_d7 | haiku | 3 | #4 (k3) | x2.041 | 69.6 | 88.4 | 2.11e-07 | 16 | 5 | 5 | 4 | 9 | 5 | 18 | 2 | 0.70 | 6 / 8 | 0 | chehab 1, wrong 1 | 95429 | 86379 | 589.7 |
| chebyshev_elu_d7 | haiku | 4 | #7 (k4) | x2.043 | 69.6 | 86.3 | 1.77e-07 | 14 | 5 | 5 | 4 | 9 | 5 | 18 | 2 | 0.60 | 4 / 8 | 0 | chehab 4 | 75614 | 67902 | 472.1 |
| chebyshev_elu_d7 | haiku | 5 | #5 (k5) | x2.053 | 69.2 | 84.8 | 2.20e-07 | 13 | 5 | 5 | 4 | 9 | 5 | 18 | 2 | 0.64 | 6 / 8 | 0 | chehab 2 | 80758 | 73013 | 512.9 |
| chebyshev_elu_d7 | opus | 1 | #8 (k6) | x1.950 | 72.9 | 91.1 | 1.35e-07 | 20 | 6 | 6 | 4 | 7 | 5 | 16 | 4 | 0.67 | 7 / 8 | 1 | - | 79523 | 71703 | 1068.3 |
| chebyshev_elu_d7 | opus | 2 | #8 (k7) | x2.181 | 65.2 | 84.1 | 1.80e-07 | 20 | 6 | 6 | 4 | 7 | 3 | 14 | 6 | 0.63 | 8 / 8 | 0 | - | 81388 | 73464 | 1109.9 |
| chebyshev_elu_d7 | opus | 3 | #6 (k8) | x1.909 | 74.5 | 91.5 | 1.56e-07 | 20 | 6 | 6 | 4 | 7 | 6 | 17 | 6 | 0.68 | 6 / 8 | 0 | runtime 1, wrong 1 | 71845 | 63964 | 962.3 |
| chebyshev_elu_d7 | sonnet | 1 | #6 (k9) | x2.008 | 70.8 | 87.0 | 2.06e-07 | 13 | 5 | 5 | 4 | 9 | 5 | 18 | 2 | 0.62 | 7 / 8 | 1 | - | 23829 | 18675 | 477.2 |
| chebyshev_elu_d7 | sonnet | 2 | #4 (k10) | x1.584 | 89.8 | 110.2 | 1.92e-07 | 20 | 6 | 6 | 4 | 7 | 12 | 23 | 7 | 0.75 | 6 / 8 | 1 | chehab 1 | 32727 | 26635 | 521.3 |
| chebyshev_elu_d7 | sonnet | 3 | #1 (k11) | x1.971 | 72.1 | 87.4 | 2.17e-07 | 14 | 5 | 5 | 4 | 9 | 6 | 19 | 3 | 0.62 | 7 / 8 | 1 | - | 26276 | 20399 | 478.7 |
| chebyshev_gelu_d7 | haiku | 1 | #5 (k1) | x2.482 | 38.2 | 53.2 | 3.35e-07 | 18 | 5 | 5 | 3 | 3 | 1 | 7 | 4 | 0.54 | 6 / 8 | 0 | chehab 2 | 59417 | 53261 | 411.7 |
| chebyshev_gelu_d7 | haiku | 2 | #3 (k2) | x2.912 | 32.6 | 45.6 | 2.53e-07 | 9 | 4 | 4 | 3 | 4 | 2 | 9 | 0 | 0.49 | 5 / 8 | 2 | chehab 1 | 74192 | 67226 | 445.6 |
| chebyshev_gelu_d7 | haiku | 3 | #5 (k3) | x2.330 | 40.7 | 55.5 | 2.34e-07 | 15 | 5 | 5 | 3 | 4 | 2 | 9 | 4 | 0.56 | 6 / 8 | 1 | chehab 1 | 72890 | 67155 | 462.2 |
| chebyshev_gelu_d7 | haiku | 4 | #7 (k4) | x2.463 | 38.5 | 53.7 | 1.89e-07 | 15 | 5 | 5 | 3 | 3 | 1 | 7 | 4 | 0.51 | 7 / 8 | 0 | chehab 1 | 73001 | 66218 | 485.2 |
| chebyshev_gelu_d7 | haiku | 5 | #4 (k5) | x2.933 | 32.4 | 45.2 | 2.06e-07 | 11 | 4 | 4 | 3 | 4 | 2 | 9 | 1 | 0.50 | 6 / 8 | 0 | chehab 1, wrong 1 | 54817 | 49194 | 382.6 |
| chebyshev_gelu_d7 | opus | 1 | #8 (k6) | x1.440 | 65.9 | 91.7 | 7.54e-07 | 18 | 10 | 10 | 3 | 10 | 2 | 15 | 0 | 0.59 | 8 / 8 | 0 | - | 51697 | 45505 | 769.1 |
| chebyshev_gelu_d7 | opus | 2 | #4 (k7) | x2.550 | 37.2 | 53.1 | 1.93e-07 | 17 | 5 | 5 | 3 | 3 | 1 | 7 | 4 | 0.52 | 8 / 8 | 0 | - | 38777 | 33539 | 625.3 |
| chebyshev_gelu_d7 | opus | 3 | #2 (k8) | x2.580 | 36.8 | 52.5 | 2.53e-07 | 16 | 5 | 5 | 3 | 3 | 1 | 7 | 4 | 0.50 | 8 / 8 | 0 | - | 44850 | 39457 | 692.0 |
| chebyshev_gelu_d7 | sonnet | 1 | #8 (k9) | x1.920 | 49.4 | 64.5 | 2.60e-07 | 18 | 5 | 5 | 3 | 5 | 6 | 14 | 2 | 0.57 | 5 / 8 | 1 | chehab 2 | 26599 | 20868 | 418.3 |
| chebyshev_gelu_d7 | sonnet | 2 | #2 (k10) | x2.956 | 32.1 | 45.1 | 1.83e-07 | 13 | 4 | 4 | 3 | 4 | 2 | 9 | 1 | 0.59 | 8 / 8 | 0 | - | 29278 | 25021 | 488.1 |
| chebyshev_gelu_d7 | sonnet | 3 | #4 (k11) | x2.328 | 40.8 | 55.6 | 2.29e-07 | 14 | 5 | 5 | 3 | 4 | 2 | 9 | 3 | 0.52 | 5 / 8 | 2 | chehab 1 | 16398 | 12291 | 313.2 |
| chebyshev_hardshrink_d7 | haiku | 1 | #7 (k1) | x2.756 | 44.6 | 59.6 | 1.67e-07 | 13 | 5 | 5 | 4 | 3 | 1 | 8 | 3 | 0.52 | 8 / 8 | 0 | - | 113572 | 108054 | 701.0 |
| chebyshev_hardshrink_d7 | haiku | 2 | #8 (k2) | x2.422 | 50.7 | 65.6 | 1.58e-07 | 12 | 5 | 5 | 4 | 5 | 3 | 12 | 6 | 0.64 | 7 / 8 | 0 | chehab 1 | 110592 | 105001 | 670.4 |
| chebyshev_hardshrink_d7 | haiku | 3 | #2 (k3) | x1.880 | 65.3 | 82.6 | 1.75e-07 | 20 | 6 | 6 | 4 | 4 | 5 | 13 | 6 | 0.61 | 5 / 8 | 1 | chehab 1 | 120832 | 113504 | 694.5 |
| chebyshev_hardshrink_d7 | sonnet | 1 | #7 (k4) | x2.250 | 54.6 | 70.9 | 3.42e-07 | 15 | 5 | 5 | 4 | 5 | 3 | 12 | 5 | 0.58 | 5 / 8 | 1 | chehab 2 | 32908 | 26527 | 490.7 |
| chebyshev_hardshrink_d7 | sonnet | 2 | #4 (k5) | x2.165 | 56.7 | 73.9 | 1.47e-07 | 17 | 6 | 6 | 4 | 3 | 3 | 10 | 6 | 0.56 | 3 / 8 | 2 | chehab 2, wrong 1 | 35235 | 30595 | 427.5 |
| chebyshev_hardshrink_d7 | sonnet | 3 | #7 (k6) | x2.312 | 53.1 | 68.4 | 7.77e-07 | 12 | 5 | 5 | 4 | 5 | 3 | 12 | 5 | 0.68 | 7 / 8 | 0 | - | 35460 | 28224 | 533.0 |
| chebyshev_mish_d7 | haiku | 1 | #5 (k1) | x2.033 | 68.3 | 83.9 | 2.38e-07 | 13 | 5 | 5 | 4 | 9 | 5 | 18 | 2 | 0.57 | 8 / 8 | 0 | - | 106024 | 98434 | 679.6 |
| chebyshev_mish_d7 | haiku | 2 | #5 (k2) | x1.987 | 69.8 | 85.6 | 2.16e-07 | 13 | 5 | 5 | 4 | 9 | 6 | 19 | 3 | 0.58 | 6 / 8 | 0 | chehab 2 | 79718 | 71049 | 529.8 |
| chebyshev_mish_d7 | haiku | 3 | #6 (k3) | x1.993 | 69.6 | 85.5 | 2.04e-07 | 13 | 5 | 5 | 4 | 9 | 6 | 19 | 3 | 0.58 | 2 / 8 | 0 | chehab 6 | 101294 | 93754 | 542.3 |
| chebyshev_mish_d7 | sonnet | 1 | #2 (k4) | x2.001 | 69.3 | 85.0 | 2.10e-07 | 14 | 5 | 5 | 4 | 9 | 6 | 19 | 3 | 0.59 | 6 / 8 | 2 | - | 20831 | 15409 | 392.7 |
| chebyshev_mish_d7 | sonnet | 2 | #5 (k5) | x2.030 | 68.4 | 84.0 | 2.15e-07 | 13 | 5 | 5 | 4 | 9 | 5 | 18 | 2 | 0.60 | 6 / 8 | 1 | chehab 1 | 23826 | 18521 | 430.8 |
| chebyshev_mish_d7 | sonnet | 3 | #0 (baseline) | x1.000 | 138.7 | 162.2 | 2.31e-07 | 22 | 8 | 8 | 6 | 8 | 11 | 25 | 10 | 0.69 | 0 / 8 | 0 | chehab 8 | 17981 | 12543 | 190.7 |
| chebyshev_relu_d2 | haiku | 1 | #1 (k1) | x1.405 | 9.7 | 20.1 | 1.17e-07 | 7 | 3 | 3 | 1 | 2 | 0 | 3 | 1 | 0.35 | 3 / 8 | 5 | - | 46999 | 43277 | 326.3 |
| chebyshev_relu_d2 | opus | 1 | #1 (k2) | x1.401 | 9.8 | 20.2 | 1.46e-07 | 7 | 3 | 3 | 1 | 2 | 0 | 3 | 1 | 0.49 | 5 / 8 | 3 | - | 18641 | 15461 | 336.7 |
| chebyshev_relu_d2 | sonnet | 1 | #1 (k3) | x1.401 | 9.8 | 20.2 | 1.38e-07 | 7 | 3 | 3 | 1 | 2 | 0 | 3 | 1 | 0.52 | 3 / 8 | 3 | - | 15438 | 12864 | 234.3 |
| chebyshev_relu_d7 | haiku | 1 | #5 (k1) | x2.906 | 32.6 | 45.7 | 2.22e-07 | 9 | 4 | 4 | 3 | 4 | 2 | 9 | 0 | 0.77 | 6 / 8 | 1 | chehab 1 | 65796 | 60035 | 513.5 |
| chebyshev_relu_d7 | opus | 1 | #5 (k2) | x2.994 | 31.7 | 44.9 | 2.49e-07 | 11 | 4 | 4 | 3 | 4 | 2 | 9 | 0 | 0.68 | 7 / 8 | 1 | - | 48863 | 43523 | 743.9 |
| chebyshev_relu_d7 | sonnet | 1 | #3 (k3) | x2.924 | 32.4 | 45.5 | 1.93e-07 | 9 | 4 | 4 | 3 | 4 | 2 | 9 | 0 | 0.70 | 5 / 8 | 2 | chehab 1 | 16412 | 12182 | 302.6 |
| chebyshev_selu_d7 | haiku | 1 | #4 (k1) | x1.992 | 69.7 | 85.3 | 1.82e-07 | 13 | 5 | 5 | 4 | 9 | 6 | 19 | 4 | 0.81 | 4 / 8 | 0 | chehab 4 | 53509 | 45201 | 357.7 |
| chebyshev_selu_d7 | haiku | 2 | #1 (k2) | x1.981 | 70.1 | 86.1 | 2.46e-07 | 13 | 5 | 5 | 4 | 9 | 6 | 19 | 3 | 0.74 | 4 / 8 | 2 | chehab 1, wrong 1 | 63850 | 56219 | 424.0 |
| chebyshev_selu_d7 | haiku | 3 | #3 (k3) | x1.991 | 69.7 | 85.4 | 1.74e-07 | 14 | 5 | 5 | 4 | 9 | 6 | 19 | 5 | 0.79 | 5 / 8 | 2 | chehab 1 | 73503 | 65383 | 471.7 |
| chebyshev_selu_d7 | sonnet | 1 | #6 (k4) | x2.036 | 68.2 | 83.6 | 2.10e-07 | 14 | 5 | 5 | 4 | 9 | 5 | 18 | 2 | 0.75 | 7 / 8 | 1 | - | 19115 | 13397 | 421.3 |
| chebyshev_selu_d7 | sonnet | 2 | #0 (baseline) | x1.000 | 138.8 | 163.2 | 2.52e-07 | 22 | 8 | 8 | 6 | 8 | 11 | 25 | 10 | 0.83 | 1 / 8 | 2 | chehab 5 | 19693 | 14274 | 236.0 |
| chebyshev_selu_d7 | sonnet | 3 | #1 (k5) | x1.986 | 69.9 | 85.8 | 2.03e-07 | 14 | 5 | 5 | 4 | 9 | 6 | 19 | 3 | 0.99 | 6 / 8 | 2 | - | 24637 | 19523 | 437.8 |
| chebyshev_sigmoid_d5 | haiku | 1 | #1 (k1) | x1.408 | 45.9 | 60.6 | 9.07e-08 | 13 | 5 | 5 | 3 | 4 | 5 | 12 | 6 | 0.81 | 6 / 8 | 0 | chehab 2 | 89182 | 80902 | 552.5 |
| chebyshev_sigmoid_d5 | haiku | 2 | #2 (k2) | x1.784 | 36.3 | 51.2 | 1.03e-07 | 22 | 5 | 5 | 3 | 2 | 0 | 5 | 4 | 0.72 | 4 / 8 | 1 | chehab 1 | 91436 | 83496 | 538.2 |
| chebyshev_sigmoid_d5 | haiku | 3 | #2 (k3) | x2.039 | 31.7 | 46.5 | 8.50e-08 | 12 | 5 | 5 | 3 | 2 | 0 | 5 | 4 | 0.59 | 2 / 8 | 3 | - | 72938 | 66812 | 409.0 |
| chebyshev_sigmoid_d5 | sonnet | 1 | #4 (k4) | x2.035 | 31.8 | 46.5 | 5.38e-08 | 12 | 5 | 5 | 3 | 2 | 0 | 5 | 4 | 0.60 | 6 / 8 | 1 | - | 24152 | 19793 | 422.8 |
| chebyshev_sigmoid_d5 | sonnet | 2 | #1 (k5) | x1.969 | 32.8 | 45.9 | 2.13e-07 | 12 | 4 | 4 | 3 | 4 | 2 | 9 | 2 | 0.67 | 8 / 8 | 0 | - | 20911 | 15817 | 418.9 |
| chebyshev_sigmoid_d5 | sonnet | 3 | #3 (k6) | x2.037 | 31.7 | 46.6 | 4.31e-08 | 12 | 5 | 5 | 3 | 2 | 0 | 5 | 4 | 0.74 | 6 / 8 | 2 | - | 26320 | 22138 | 414.9 |
| chebyshev_sigmoid_d7 | haiku | 1 | #5 (k1) | x2.285 | 54.2 | 71.1 | 1.12e-07 | 17 | 6 | 6 | 4 | 3 | 1 | 8 | 6 | 0.76 | 5 / 8 | 2 | chehab 1 | 94314 | 88240 | 572.6 |
| chebyshev_sigmoid_d7 | haiku | 2 | #8 (k2) | x2.243 | 55.3 | 70.5 | 1.09e-07 | 15 | 5 | 5 | 4 | 5 | 2 | 11 | 6 | 0.76 | 7 / 8 | 1 | - | 80339 | 74850 | 537.7 |
| chebyshev_sigmoid_d7 | haiku | 3 | #6 (k3) | x2.425 | 51.1 | 66.5 | 1.03e-07 | 14 | 5 | 5 | 4 | 5 | 3 | 12 | 7 | 0.77 | 6 / 8 | 1 | chehab 1 | 110002 | 105011 | 638.3 |
| chebyshev_sigmoid_d7 | haiku | 4 | #7 (k4) | x2.610 | 47.5 | 62.4 | 6.20e-08 | 15 | 5 | 5 | 4 | 4 | 1 | 9 | 3 | 0.74 | 6 / 8 | 1 | chehab 1 | 114795 | 108260 | 1000.9 |
| chebyshev_sigmoid_d7 | haiku | 5 | #8 (k5) | x2.642 | 46.9 | 61.7 | 6.02e-08 | 13 | 5 | 5 | 4 | 3 | 2 | 9 | 3 | 0.77 | 7 / 8 | 0 | chehab 1 | 107298 | 101530 | 658.5 |
| chebyshev_sigmoid_d7 | opus | 1 | #8 (k6) | x1.717 | 72.2 | 100.2 | 8.24e-08 | 20 | 10 | 10 | 4 | 6 | 0 | 10 | 7 | 0.74 | 8 / 8 | 0 | - | 73406 | 67554 | 1010.3 |
| chebyshev_sigmoid_d7 | opus | 2 | #5 (k7) | x2.572 | 48.2 | 65.6 | 7.59e-08 | 17 | 6 | 6 | 4 | 2 | 0 | 6 | 7 | 0.67 | 7 / 8 | 0 | - | 65041 | 59237 | 888.4 |
| chebyshev_sigmoid_d7 | opus | 3 | #5 (k8) | x2.799 | 44.3 | 59.3 | 6.87e-08 | 13 | 5 | 5 | 4 | 3 | 1 | 8 | 3 | 0.69 | 7 / 8 | 1 | - | 66043 | 60005 | 932.1 |
| chebyshev_sigmoid_d7 | sonnet | 1 | #1 (k9) | x1.945 | 63.7 | 80.9 | 7.63e-08 | 16 | 6 | 6 | 4 | 4 | 4 | 12 | 6 | 0.78 | 6 / 8 | 0 | chehab 2 | 30208 | 25099 | 484.3 |
| chebyshev_sigmoid_d7 | sonnet | 2 | #1 (k10) | x2.027 | 61.2 | 78.1 | 6.39e-08 | 15 | 6 | 6 | 4 | 4 | 3 | 11 | 5 | 1.10 | 6 / 8 | 1 | chehab 1 | 23858 | 19266 | 427.9 |
| chebyshev_sigmoid_d7 | sonnet | 3 | #1 (k11) | x2.256 | 54.9 | 71.1 | 2.57e-07 | 16 | 5 | 5 | 4 | 5 | 3 | 12 | 5 | 0.96 | 5 / 8 | 2 | wrong 1 | 31709 | 25784 | 452.8 |
| chebyshev_silu_d7 | haiku | 1 | #2 (k1) | x2.907 | 32.6 | 45.5 | 1.77e-07 | 9 | 4 | 4 | 3 | 4 | 2 | 9 | 0 | 0.99 | 5 / 8 | 2 | chehab 1 | 100743 | 94680 | 610.6 |
| chebyshev_silu_d7 | haiku | 2 | #8 (k2) | x2.459 | 38.5 | 53.5 | 2.55e-07 | 15 | 5 | 5 | 3 | 3 | 1 | 7 | 4 | 0.74 | 7 / 8 | 0 | chehab 1 | 70096 | 63909 | 479.2 |
| chebyshev_silu_d7 | haiku | 3 | #2 (k3) | x2.944 | 32.2 | 45.1 | 1.97e-07 | 10 | 4 | 4 | 3 | 4 | 2 | 9 | 0 | 0.66 | 7 / 8 | 1 | - | 110196 | 104517 | 670.7 |
| chebyshev_silu_d7 | sonnet | 1 | #5 (k4) | x2.944 | 32.2 | 45.2 | 1.76e-07 | 13 | 4 | 4 | 3 | 4 | 2 | 9 | 1 | 0.64 | 6 / 8 | 1 | wrong 1 | 29966 | 24510 | 463.5 |
| chebyshev_silu_d7 | sonnet | 2 | #5 (k5) | x2.958 | 32.1 | 45.0 | 2.74e-07 | 10 | 4 | 4 | 3 | 4 | 2 | 9 | 0 | 0.74 | 8 / 8 | 0 | - | 23073 | 19050 | 459.1 |
| chebyshev_silu_d7 | sonnet | 3 | #3 (k6) | x2.940 | 32.2 | 45.2 | 1.61e-07 | 10 | 4 | 4 | 3 | 4 | 2 | 9 | 0 | 0.83 | 5 / 8 | 2 | chehab 1 | 15787 | 12128 | 302.2 |
| chebyshev_softplus_d7 | haiku | 1 | #6 (k1) | x2.956 | 32.1 | 45.0 | 1.23e-07 | 10 | 4 | 4 | 3 | 4 | 2 | 9 | 1 | 0.68 | 6 / 8 | 2 | - | 67068 | 61439 | 441.2 |
| chebyshev_softplus_d7 | haiku | 2 | #2 (k2) | x2.338 | 40.5 | 55.3 | 1.28e-07 | 14 | 5 | 5 | 3 | 4 | 2 | 9 | 3 | 0.72 | 5 / 8 | 2 | chehab 1 | 67930 | 62843 | 429.5 |
| chebyshev_softplus_d7 | haiku | 3 | #6 (k3) | x2.938 | 32.2 | 45.2 | 1.38e-07 | 11 | 4 | 4 | 3 | 4 | 2 | 9 | 1 | 0.66 | 6 / 8 | 0 | chehab 1, wrong 1 | 61780 | 55936 | 424.0 |
| chebyshev_softplus_d7 | sonnet | 1 | #4 (k4) | x2.938 | 32.2 | 45.2 | 1.58e-07 | 10 | 4 | 4 | 3 | 4 | 2 | 9 | 0 | 0.72 | 6 / 8 | 1 | chehab 1 | 14349 | 10564 | 327.7 |
| chebyshev_softplus_d7 | sonnet | 2 | #3 (k5) | x2.905 | 32.6 | 45.7 | 1.16e-07 | 9 | 4 | 4 | 3 | 4 | 2 | 9 | 0 | 0.73 | 8 / 8 | 0 | - | 14885 | 11049 | 354.3 |
| chebyshev_softplus_d7 | sonnet | 3 | #2 (k6) | x2.339 | 40.5 | 55.4 | 1.39e-07 | 14 | 5 | 5 | 3 | 4 | 2 | 9 | 3 | 0.81 | 7 / 8 | 1 | - | 19057 | 15198 | 403.0 |
| elementwise_add | haiku | 1 | #0 (baseline) | x1.000 | 0.0 | 10.2 | 3.73e-08 | 1 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0.61 | 0 / 8 | 8 | - | 3406 | 1585 | 45.8 |
| elementwise_add | opus | 1 | #0 (baseline) | x1.000 | 0.0 | 10.2 | 3.73e-08 | 1 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0.61 | 0 / 8 | 8 | - | 2224 | 568 | 62.5 |
| elementwise_add | sonnet | 1 | #0 (baseline) | x1.000 | 0.0 | 10.2 | 3.73e-08 | 1 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0.61 | 0 / 8 | 8 | - | 1235 | 44 | 48.8 |
| elementwise_add_plain | haiku | 1 | #0 (baseline) | x1.000 | 0.4 | 6.0 | 2.91e-08 | 1 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0.46 | 0 / 8 | 8 | - | 4253 | 2382 | 55.2 |
| elementwise_add_plain | opus | 1 | #0 (baseline) | x1.000 | 0.4 | 6.0 | 2.91e-08 | 1 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0.46 | 0 / 8 | 8 | - | 2450 | 691 | 64.3 |
| elementwise_add_plain | sonnet | 1 | #0 (baseline) | x1.000 | 0.4 | 6.0 | 2.91e-08 | 1 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0.46 | 0 / 8 | 8 | - | 2509 | 1099 | 56.6 |
| elementwise_add_scalar | haiku | 1 | #0 (baseline) | x1.000 | 0.0 | 5.3 | 3.34e-08 | 1 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0.55 | 0 / 8 | 8 | - | 3991 | 2275 | 52.0 |
| elementwise_add_scalar | opus | 1 | #0 (baseline) | x1.000 | 0.0 | 5.3 | 3.34e-08 | 1 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0.55 | 0 / 8 | 8 | - | 2436 | 869 | 63.2 |
| elementwise_add_scalar | sonnet | 1 | #0 (baseline) | x1.000 | 0.0 | 5.3 | 3.34e-08 | 1 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0.55 | 0 / 8 | 8 | - | 2526 | 1216 | 56.4 |
| elementwise_mul | haiku | 1 | #0 (baseline) | x1.000 | 3.5 | 16.6 | 5.88e-08 | 1 | 1 | 1 | 1 | 0 | 0 | 1 | 0 | 0.43 | 0 / 8 | 8 | - | 3906 | 1820 | 53.1 |
| elementwise_mul | opus | 1 | #0 (baseline) | x1.000 | 3.5 | 16.6 | 5.88e-08 | 1 | 1 | 1 | 1 | 0 | 0 | 1 | 0 | 0.43 | 0 / 8 | 8 | - | 2724 | 908 | 65.6 |
| elementwise_mul | sonnet | 1 | #0 (baseline) | x1.000 | 3.5 | 16.6 | 5.88e-08 | 1 | 1 | 1 | 1 | 0 | 0 | 1 | 0 | 0.43 | 0 / 8 | 8 | - | 1671 | 202 | 54.0 |
| elementwise_mul_plain | haiku | 1 | #0 (baseline) | x1.000 | 1.4 | 8.3 | 5.80e-08 | 1 | 1 | 1 | 0 | 1 | 0 | 1 | 0 | 0.43 | 0 / 8 | 8 | - | 3998 | 2336 | 50.9 |
| elementwise_mul_plain | opus | 1 | #0 (baseline) | x1.000 | 1.4 | 8.3 | 5.80e-08 | 1 | 1 | 1 | 0 | 1 | 0 | 1 | 0 | 0.43 | 1 / 8 | 7 | - | 2257 | 559 | 83.1 |
| elementwise_mul_plain | sonnet | 1 | #0 (baseline) | x1.000 | 1.4 | 8.3 | 5.80e-08 | 1 | 1 | 1 | 0 | 1 | 0 | 1 | 0 | 0.43 | 0 / 8 | 8 | - | 1431 | 212 | 47.1 |
| elementwise_mul_scalar | haiku | 1 | #0 (baseline) | x1.000 | 0.8 | 7.6 | 6.31e-08 | 1 | 1 | 1 | 0 | 1 | 0 | 1 | 0 | 0.42 | 0 / 8 | 8 | - | 4762 | 3048 | 61.3 |
| elementwise_mul_scalar | opus | 1 | #0 (baseline) | x1.000 | 0.8 | 7.6 | 6.31e-08 | 1 | 1 | 1 | 0 | 1 | 0 | 1 | 0 | 0.42 | 1 / 8 | 7 | - | 2372 | 645 | 85.1 |
| elementwise_mul_scalar | sonnet | 1 | #0 (baseline) | x1.000 | 0.8 | 7.6 | 6.31e-08 | 1 | 1 | 1 | 0 | 1 | 0 | 1 | 0 | 0.42 | 0 / 8 | 8 | - | 2122 | 726 | 54.7 |
| embedding | haiku | 1 | #0 (baseline) | x1.000 | 73.5 | 82.6 | 4.71e-05 | 21 | 2 | 2 | 0 | 6 | 0 | 6 | 0 | 0.63 | 4 / 8 | 1 | compile 1, chehab 2 | 92876 | 88168 | 826.6 |
| embedding | opus | 1 | #0 (baseline) | x1.000 | 73.5 | 82.6 | 4.71e-05 | 21 | 2 | 2 | 0 | 6 | 0 | 6 | 0 | 0.63 | 7 / 8 | 1 | - | 70752 | 65873 | 1415.7 |
| embedding | sonnet | 1 | #0 (baseline) | x1.000 | 73.5 | 82.6 | 4.71e-05 | 21 | 2 | 2 | 0 | 6 | 0 | 6 | 0 | 0.63 | 1 / 8 | 6 | wrong 1 | 33893 | 30425 | 431.1 |
| extract | haiku | 1 | #0 (baseline) | x1.000 | 2.7 | 9.6 | 7.35e-06 | 2 | 1 | 1 | 0 | 1 | 0 | 1 | 0 | 0.40 | 1 / 8 | 6 | wrong 1 | 8611 | 6244 | 108.1 |
| extract | opus | 1 | #0 (baseline) | x1.000 | 2.7 | 9.6 | 7.35e-06 | 2 | 1 | 1 | 0 | 1 | 0 | 1 | 0 | 0.40 | 4 / 8 | 4 | - | 3824 | 1847 | 136.4 |
| extract | sonnet | 1 | #0 (baseline) | x1.000 | 2.7 | 9.6 | 7.35e-06 | 2 | 1 | 1 | 0 | 1 | 0 | 1 | 0 | 0.40 | 0 / 8 | 8 | - | 5335 | 3665 | 80.4 |
| extract_linear | haiku | 1 | #0 (baseline) | x1.000 | 2.7 | 9.5 | 1.09e-05 | 2 | 1 | 1 | 0 | 1 | 0 | 1 | 0 | 0.39 | 1 / 8 | 7 | - | 8958 | 6738 | 103.8 |
| extract_linear | opus | 1 | #0 (baseline) | x1.000 | 2.7 | 9.5 | 1.09e-05 | 2 | 1 | 1 | 0 | 1 | 0 | 1 | 0 | 0.39 | 2 / 8 | 6 | - | 4275 | 2417 | 119.2 |
| extract_linear | sonnet | 1 | #0 (baseline) | x1.000 | 2.7 | 9.5 | 1.09e-05 | 2 | 1 | 1 | 0 | 1 | 0 | 1 | 0 | 0.39 | 0 / 8 | 8 | - | 4323 | 2669 | 68.9 |
| extract_sparse | haiku | 1 | #2 (k1) | x1.282 | 6.3 | 19.6 | 9.30e-06 | 3 | 1 | 1 | 0 | 2 | 1 | 3 | 0 | 0.56 | 7 / 8 | 1 | - | 52358 | 47641 | 430.8 |
| extract_sparse | opus | 1 | #2 (k2) | x1.283 | 6.2 | 19.5 | 5.94e-06 | 4 | 1 | 1 | 0 | 2 | 1 | 3 | 0 | 0.42 | 7 / 8 | 1 | - | 17912 | 13540 | 364.4 |
| extract_sparse | sonnet | 1 | #1 (k3) | x1.281 | 6.3 | 19.6 | 8.23e-06 | 4 | 1 | 1 | 0 | 2 | 1 | 3 | 0 | 0.48 | 7 / 8 | 1 | - | 11285 | 8383 | 277.1 |
| quad | haiku | 1 | #0 (baseline) | x1.000 | 3.4 | 10.2 | 1.10e-07 | 1 | 1 | 1 | 1 | 0 | 0 | 1 | 0 | 0.62 | 1 / 8 | 7 | - | 4033 | 2015 | 74.5 |
| quad | sonnet | 1 | #0 (baseline) | x1.000 | 3.4 | 10.2 | 1.10e-07 | 1 | 1 | 1 | 1 | 0 | 0 | 1 | 0 | 0.62 | 1 / 8 | 7 | - | 1255 | 0 | 67.9 |
| relu_composite | haiku | 1 | #0 (baseline) | x1.000 | 364.6 | 392.5 | 8.44e-07 | 17 | 10 | 10 | 16 | 12 | 9 | 37 | 9 | 0.69 | 3 / 8 | 2 | chehab 3 | 136380 | 128844 | 838.0 |
| relu_composite | opus | 1 | #0 (baseline) | x1.000 | 364.6 | 392.5 | 8.44e-07 | 17 | 10 | 10 | 16 | 12 | 9 | 37 | 9 | 0.69 | 3 / 8 | 1 | chehab 2, held-out 1 | 120606 | 112949 | 1466.7 |
| relu_composite | sonnet | 1 | #8 (k1) | x1.011 | 360.6 | 388.5 | 9.70e-07 | 18 | 10 | 10 | 15 | 11 | 8 | 34 | 16 | 0.80 | 2 / 8 | 4 | chehab 2 | 28515 | 22155 | 364.4 |
| table_mult | haiku | 1 | #0 (baseline) | x1.000 | 34.6 | 42.1 | 6.31e-05 | 17 | 1 | 1 | 0 | 3 | 0 | 3 | 0 | 0.66 | 1 / 8 | 7 | - | 48732 | 45958 | 369.0 |
| table_mult | opus | 1 | #0 (baseline) | x1.000 | 34.6 | 42.1 | 6.31e-05 | 17 | 1 | 1 | 0 | 3 | 0 | 3 | 0 | 0.66 | 6 / 8 | 2 | - | 13870 | 10083 | 655.7 |
| table_mult | sonnet | 1 | #0 (baseline) | x1.000 | 34.6 | 42.1 | 6.31e-05 | 17 | 1 | 1 | 0 | 3 | 0 | 3 | 0 | 0.66 | 3 / 8 | 5 | - | 10253 | 7412 | 354.4 |

## Per task and strategy (3 runs each)

| task | strategy | runs | speedup |
|---|---|---:|---|
| batchnorm1d | haiku | 1 | median x1.000, mean x1.000, min x1.000, max x1.000, sd 0.000 |
| batchnorm2d | haiku | 1 | median x1.005, mean x1.005, min x1.005, max x1.005, sd 0.000 |
| chebyshev_elu_d7 | haiku | 5 | median x2.041, mean x2.028, min x1.983, max x2.053, sd 0.028 |
| chebyshev_gelu_d7 | haiku | 5 | median x2.482, mean x2.624, min x2.330, max x2.933, sd 0.279 |
| chebyshev_hardshrink_d7 | haiku | 3 | median x2.422, mean x2.353, min x1.880, max x2.756, sd 0.442 |
| chebyshev_mish_d7 | haiku | 3 | median x1.993, mean x2.004, min x1.987, max x2.033, sd 0.025 |
| chebyshev_relu_d2 | haiku | 1 | median x1.405, mean x1.405, min x1.405, max x1.405, sd 0.000 |
| chebyshev_relu_d7 | haiku | 1 | median x2.906, mean x2.906, min x2.906, max x2.906, sd 0.000 |
| chebyshev_selu_d7 | haiku | 3 | median x1.991, mean x1.988, min x1.981, max x1.992, sd 0.006 |
| chebyshev_sigmoid_d5 | haiku | 3 | median x1.784, mean x1.743, min x1.408, max x2.039, sd 0.317 |
| chebyshev_sigmoid_d7 | haiku | 5 | median x2.425, mean x2.441, min x2.243, max x2.642, sd 0.182 |
| chebyshev_silu_d7 | haiku | 3 | median x2.907, mean x2.770, min x2.459, max x2.944, sd 0.270 |
| chebyshev_softplus_d7 | haiku | 3 | median x2.938, mean x2.744, min x2.338, max x2.956, sd 0.352 |
| elementwise_add | haiku | 1 | median x1.000, mean x1.000, min x1.000, max x1.000, sd 0.000 |
| elementwise_add_plain | haiku | 1 | median x1.000, mean x1.000, min x1.000, max x1.000, sd 0.000 |
| elementwise_add_scalar | haiku | 1 | median x1.000, mean x1.000, min x1.000, max x1.000, sd 0.000 |
| elementwise_mul | haiku | 1 | median x1.000, mean x1.000, min x1.000, max x1.000, sd 0.000 |
| elementwise_mul_plain | haiku | 1 | median x1.000, mean x1.000, min x1.000, max x1.000, sd 0.000 |
| elementwise_mul_scalar | haiku | 1 | median x1.000, mean x1.000, min x1.000, max x1.000, sd 0.000 |
| embedding | haiku | 1 | median x1.000, mean x1.000, min x1.000, max x1.000, sd 0.000 |
| extract | haiku | 1 | median x1.000, mean x1.000, min x1.000, max x1.000, sd 0.000 |
| extract_linear | haiku | 1 | median x1.000, mean x1.000, min x1.000, max x1.000, sd 0.000 |
| extract_sparse | haiku | 1 | median x1.282, mean x1.282, min x1.282, max x1.282, sd 0.000 |
| quad | haiku | 1 | median x1.000, mean x1.000, min x1.000, max x1.000, sd 0.000 |
| relu_composite | haiku | 1 | median x1.000, mean x1.000, min x1.000, max x1.000, sd 0.000 |
| table_mult | haiku | 1 | median x1.000, mean x1.000, min x1.000, max x1.000, sd 0.000 |
| chebyshev_elu_d7 | opus | 3 | median x1.950, mean x2.013, min x1.909, max x2.181, sd 0.147 |
| chebyshev_gelu_d7 | opus | 3 | median x2.550, mean x2.190, min x1.440, max x2.580, sd 0.650 |
| chebyshev_relu_d2 | opus | 1 | median x1.401, mean x1.401, min x1.401, max x1.401, sd 0.000 |
| chebyshev_relu_d7 | opus | 1 | median x2.994, mean x2.994, min x2.994, max x2.994, sd 0.000 |
| chebyshev_sigmoid_d7 | opus | 3 | median x2.572, mean x2.363, min x1.717, max x2.799, sd 0.571 |
| elementwise_add | opus | 1 | median x1.000, mean x1.000, min x1.000, max x1.000, sd 0.000 |
| elementwise_add_plain | opus | 1 | median x1.000, mean x1.000, min x1.000, max x1.000, sd 0.000 |
| elementwise_add_scalar | opus | 1 | median x1.000, mean x1.000, min x1.000, max x1.000, sd 0.000 |
| elementwise_mul | opus | 1 | median x1.000, mean x1.000, min x1.000, max x1.000, sd 0.000 |
| elementwise_mul_plain | opus | 1 | median x1.000, mean x1.000, min x1.000, max x1.000, sd 0.000 |
| elementwise_mul_scalar | opus | 1 | median x1.000, mean x1.000, min x1.000, max x1.000, sd 0.000 |
| embedding | opus | 1 | median x1.000, mean x1.000, min x1.000, max x1.000, sd 0.000 |
| extract | opus | 1 | median x1.000, mean x1.000, min x1.000, max x1.000, sd 0.000 |
| extract_linear | opus | 1 | median x1.000, mean x1.000, min x1.000, max x1.000, sd 0.000 |
| extract_sparse | opus | 1 | median x1.283, mean x1.283, min x1.283, max x1.283, sd 0.000 |
| relu_composite | opus | 1 | median x1.000, mean x1.000, min x1.000, max x1.000, sd 0.000 |
| table_mult | opus | 1 | median x1.000, mean x1.000, min x1.000, max x1.000, sd 0.000 |
| batchnorm1d | sonnet | 1 | median x1.000, mean x1.000, min x1.000, max x1.000, sd 0.000 |
| batchnorm2d | sonnet | 1 | median x1.000, mean x1.000, min x1.000, max x1.000, sd 0.000 |
| chebyshev_elu_d7 | sonnet | 3 | median x1.971, mean x1.854, min x1.584, max x2.008, sd 0.235 |
| chebyshev_gelu_d7 | sonnet | 3 | median x2.328, mean x2.401, min x1.920, max x2.956, sd 0.522 |
| chebyshev_hardshrink_d7 | sonnet | 3 | median x2.250, mean x2.242, min x2.165, max x2.312, sd 0.074 |
| chebyshev_mish_d7 | sonnet | 3 | median x2.001, mean x1.677, min x1.000, max x2.030, sd 0.586 |
| chebyshev_relu_d2 | sonnet | 1 | median x1.401, mean x1.401, min x1.401, max x1.401, sd 0.000 |
| chebyshev_relu_d7 | sonnet | 1 | median x2.924, mean x2.924, min x2.924, max x2.924, sd 0.000 |
| chebyshev_selu_d7 | sonnet | 3 | median x1.986, mean x1.674, min x1.000, max x2.036, sd 0.584 |
| chebyshev_sigmoid_d5 | sonnet | 3 | median x2.035, mean x2.014, min x1.969, max x2.037, sd 0.039 |
| chebyshev_sigmoid_d7 | sonnet | 3 | median x2.027, mean x2.076, min x1.945, max x2.256, sd 0.161 |
| chebyshev_silu_d7 | sonnet | 3 | median x2.944, mean x2.948, min x2.940, max x2.958, sd 0.009 |
| chebyshev_softplus_d7 | sonnet | 3 | median x2.905, mean x2.727, min x2.339, max x2.938, sd 0.337 |
| elementwise_add | sonnet | 1 | median x1.000, mean x1.000, min x1.000, max x1.000, sd 0.000 |
| elementwise_add_plain | sonnet | 1 | median x1.000, mean x1.000, min x1.000, max x1.000, sd 0.000 |
| elementwise_add_scalar | sonnet | 1 | median x1.000, mean x1.000, min x1.000, max x1.000, sd 0.000 |
| elementwise_mul | sonnet | 1 | median x1.000, mean x1.000, min x1.000, max x1.000, sd 0.000 |
| elementwise_mul_plain | sonnet | 1 | median x1.000, mean x1.000, min x1.000, max x1.000, sd 0.000 |
| elementwise_mul_scalar | sonnet | 1 | median x1.000, mean x1.000, min x1.000, max x1.000, sd 0.000 |
| embedding | sonnet | 1 | median x1.000, mean x1.000, min x1.000, max x1.000, sd 0.000 |
| extract | sonnet | 1 | median x1.000, mean x1.000, min x1.000, max x1.000, sd 0.000 |
| extract_linear | sonnet | 1 | median x1.000, mean x1.000, min x1.000, max x1.000, sd 0.000 |
| extract_sparse | sonnet | 1 | median x1.281, mean x1.281, min x1.281, max x1.281, sd 0.000 |
| quad | sonnet | 1 | median x1.000, mean x1.000, min x1.000, max x1.000, sd 0.000 |
| relu_composite | sonnet | 1 | median x1.011, mean x1.011, min x1.011, max x1.011, sd 0.000 |
| table_mult | sonnet | 1 | median x1.000, mean x1.000, min x1.000, max x1.000, sd 0.000 |

## Across tasks (geometric mean of the per-task mean speedups)

| strategy | tasks | geometric mean |
|---|---:|---:|
| haiku | 26 | x1.416 |
| opus | 17 | x1.267 |
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
| batchnorm1d | baseline | haiku/run1 sonnet/run1 | True | 2.24 | 1.0 |
| batchnorm2d | baseline | sonnet/run1 | True | 2.2 | 1.0 |
| batchnorm2d | k1 | haiku/run1 | True | 2.19 | 1.005 |
| chebyshev_elu_d7 | baseline | baseline | True | 142.18 | 1.0 |
| chebyshev_elu_d7 | k1 | haiku/run1 | True | 70.37 | 2.02 |
| chebyshev_elu_d7 | k2 | haiku/run2 | True | 71.71 | 1.983 |
| chebyshev_elu_d7 | k3 | haiku/run3 | True | 69.65 | 2.041 |
| chebyshev_elu_d7 | k4 | haiku/run4 | True | 69.59 | 2.043 |
| chebyshev_elu_d7 | k5 | haiku/run5 | True | 69.25 | 2.053 |
| chebyshev_elu_d7 | k6 | opus/run1 | True | 72.91 | 1.95 |
| chebyshev_elu_d7 | k7 | opus/run2 | True | 65.2 | 2.181 |
| chebyshev_elu_d7 | k8 | opus/run3 | True | 74.49 | 1.909 |
| chebyshev_elu_d7 | k9 | sonnet/run1 | True | 70.81 | 2.008 |
| chebyshev_elu_d7 | k10 | sonnet/run2 | True | 89.77 | 1.584 |
| chebyshev_elu_d7 | k11 | sonnet/run3 | True | 72.14 | 1.971 |
| chebyshev_gelu_d7 | baseline | baseline | True | 94.9 | 1.0 |
| chebyshev_gelu_d7 | k1 | haiku/run1 | True | 38.23 | 2.482 |
| chebyshev_gelu_d7 | k2 | haiku/run2 | True | 32.58 | 2.912 |
| chebyshev_gelu_d7 | k3 | haiku/run3 | True | 40.73 | 2.33 |
| chebyshev_gelu_d7 | k4 | haiku/run4 | True | 38.54 | 2.463 |
| chebyshev_gelu_d7 | k5 | haiku/run5 | True | 32.35 | 2.933 |
| chebyshev_gelu_d7 | k6 | opus/run1 | True | 65.91 | 1.44 |
| chebyshev_gelu_d7 | k7 | opus/run2 | True | 37.22 | 2.55 |
| chebyshev_gelu_d7 | k8 | opus/run3 | True | 36.78 | 2.58 |
| chebyshev_gelu_d7 | k9 | sonnet/run1 | True | 49.43 | 1.92 |
| chebyshev_gelu_d7 | k10 | sonnet/run2 | True | 32.11 | 2.956 |
| chebyshev_gelu_d7 | k11 | sonnet/run3 | True | 40.76 | 2.328 |
| chebyshev_hardshrink_d7 | baseline | baseline | True | 122.78 | 1.0 |
| chebyshev_hardshrink_d7 | k1 | haiku/run1 | True | 44.56 | 2.756 |
| chebyshev_hardshrink_d7 | k2 | haiku/run2 | True | 50.69 | 2.422 |
| chebyshev_hardshrink_d7 | k3 | haiku/run3 | True | 65.3 | 1.88 |
| chebyshev_hardshrink_d7 | k4 | sonnet/run1 | True | 54.58 | 2.25 |
| chebyshev_hardshrink_d7 | k5 | sonnet/run2 | True | 56.71 | 2.165 |
| chebyshev_hardshrink_d7 | k6 | sonnet/run3 | True | 53.1 | 2.312 |
| chebyshev_mish_d7 | baseline | sonnet/run3 | True | 138.73 | 1.0 |
| chebyshev_mish_d7 | k1 | haiku/run1 | True | 68.25 | 2.033 |
| chebyshev_mish_d7 | k2 | haiku/run2 | True | 69.81 | 1.987 |
| chebyshev_mish_d7 | k3 | haiku/run3 | True | 69.61 | 1.993 |
| chebyshev_mish_d7 | k4 | sonnet/run1 | True | 69.32 | 2.001 |
| chebyshev_mish_d7 | k5 | sonnet/run2 | True | 68.36 | 2.03 |
| chebyshev_relu_d2 | baseline | baseline | True | 13.69 | 1.0 |
| chebyshev_relu_d2 | k1 | haiku/run1 | True | 9.75 | 1.405 |
| chebyshev_relu_d2 | k2 | opus/run1 | True | 9.78 | 1.401 |
| chebyshev_relu_d2 | k3 | sonnet/run1 | True | 9.77 | 1.401 |
| chebyshev_relu_d7 | baseline | baseline | True | 94.82 | 1.0 |
| chebyshev_relu_d7 | k1 | haiku/run1 | True | 32.62 | 2.906 |
| chebyshev_relu_d7 | k2 | opus/run1 | True | 31.67 | 2.994 |
| chebyshev_relu_d7 | k3 | sonnet/run1 | True | 32.42 | 2.924 |
| chebyshev_selu_d7 | baseline | sonnet/run2 | True | 138.78 | 1.0 |
| chebyshev_selu_d7 | k1 | haiku/run1 | True | 69.66 | 1.992 |
| chebyshev_selu_d7 | k2 | haiku/run2 | True | 70.06 | 1.981 |
| chebyshev_selu_d7 | k3 | haiku/run3 | True | 69.7 | 1.991 |
| chebyshev_selu_d7 | k4 | sonnet/run1 | True | 68.16 | 2.036 |
| chebyshev_selu_d7 | k5 | sonnet/run3 | True | 69.89 | 1.986 |
| chebyshev_sigmoid_d5 | baseline | baseline | True | 64.66 | 1.0 |
| chebyshev_sigmoid_d5 | k1 | haiku/run1 | True | 45.92 | 1.408 |
| chebyshev_sigmoid_d5 | k2 | haiku/run2 | True | 36.25 | 1.784 |
| chebyshev_sigmoid_d5 | k3 | haiku/run3 | True | 31.72 | 2.039 |
| chebyshev_sigmoid_d5 | k4 | sonnet/run1 | True | 31.76 | 2.035 |
| chebyshev_sigmoid_d5 | k5 | sonnet/run2 | True | 32.83 | 1.969 |
| chebyshev_sigmoid_d5 | k6 | sonnet/run3 | True | 31.74 | 2.037 |
| chebyshev_sigmoid_d7 | baseline | baseline | True | 123.95 | 1.0 |
| chebyshev_sigmoid_d7 | k1 | haiku/run1 | True | 54.24 | 2.285 |
| chebyshev_sigmoid_d7 | k2 | haiku/run2 | True | 55.26 | 2.243 |
| chebyshev_sigmoid_d7 | k3 | haiku/run3 | True | 51.11 | 2.425 |
| chebyshev_sigmoid_d7 | k4 | haiku/run4 | True | 47.49 | 2.61 |
| chebyshev_sigmoid_d7 | k5 | haiku/run5 | True | 46.92 | 2.642 |
| chebyshev_sigmoid_d7 | k6 | opus/run1 | True | 72.19 | 1.717 |
| chebyshev_sigmoid_d7 | k7 | opus/run2 | True | 48.19 | 2.572 |
| chebyshev_sigmoid_d7 | k8 | opus/run3 | True | 44.28 | 2.799 |
| chebyshev_sigmoid_d7 | k9 | sonnet/run1 | True | 63.73 | 1.945 |
| chebyshev_sigmoid_d7 | k10 | sonnet/run2 | True | 61.16 | 2.027 |
| chebyshev_sigmoid_d7 | k11 | sonnet/run3 | True | 54.94 | 2.256 |
| chebyshev_silu_d7 | baseline | baseline | True | 94.79 | 1.0 |
| chebyshev_silu_d7 | k1 | haiku/run1 | True | 32.61 | 2.907 |
| chebyshev_silu_d7 | k2 | haiku/run2 | True | 38.55 | 2.459 |
| chebyshev_silu_d7 | k3 | haiku/run3 | True | 32.2 | 2.944 |
| chebyshev_silu_d7 | k4 | sonnet/run1 | True | 32.19 | 2.944 |
| chebyshev_silu_d7 | k5 | sonnet/run2 | True | 32.05 | 2.958 |
| chebyshev_silu_d7 | k6 | sonnet/run3 | True | 32.24 | 2.94 |
| chebyshev_softplus_d7 | baseline | baseline | True | 94.74 | 1.0 |
| chebyshev_softplus_d7 | k1 | haiku/run1 | True | 32.05 | 2.956 |
| chebyshev_softplus_d7 | k2 | haiku/run2 | True | 40.53 | 2.338 |
| chebyshev_softplus_d7 | k3 | haiku/run3 | True | 32.24 | 2.938 |
| chebyshev_softplus_d7 | k4 | sonnet/run1 | True | 32.24 | 2.938 |
| chebyshev_softplus_d7 | k5 | sonnet/run2 | True | 32.61 | 2.905 |
| chebyshev_softplus_d7 | k6 | sonnet/run3 | True | 40.5 | 2.339 |
| elementwise_add | baseline | haiku/run1 opus/run1 sonnet/run1 | True | 0.04 | 1.0 |
| elementwise_add_plain | baseline | haiku/run1 opus/run1 sonnet/run1 | True | 0.45 | 1.0 |
| elementwise_add_scalar | baseline | haiku/run1 opus/run1 sonnet/run1 | True | 0.02 | 1.0 |
| elementwise_mul | baseline | haiku/run1 opus/run1 sonnet/run1 | True | 3.47 | 1.0 |
| elementwise_mul_plain | baseline | haiku/run1 opus/run1 sonnet/run1 | True | 1.45 | 1.0 |
| elementwise_mul_scalar | baseline | haiku/run1 opus/run1 sonnet/run1 | True | 0.84 | 1.0 |
| embedding | baseline | haiku/run1 opus/run1 sonnet/run1 | True | 73.5 | 1.0 |
| extract | baseline | haiku/run1 opus/run1 sonnet/run1 | True | 2.67 | 1.0 |
| extract_linear | baseline | haiku/run1 opus/run1 sonnet/run1 | True | 2.66 | 1.0 |
| extract_sparse | baseline | baseline | True | 8.02 | 1.0 |
| extract_sparse | k1 | haiku/run1 | True | 6.25 | 1.282 |
| extract_sparse | k2 | opus/run1 | True | 6.25 | 1.283 |
| extract_sparse | k3 | sonnet/run1 | True | 6.26 | 1.281 |
| quad | baseline | haiku/run1 sonnet/run1 | True | 3.43 | 1.0 |
| relu_composite | baseline | haiku/run1 opus/run1 | True | 364.6 | 1.0 |
| relu_composite | k1 | sonnet/run1 | True | 360.56 | 1.011 |
| table_mult | baseline | haiku/run1 opus/run1 sonnet/run1 | True | 34.63 | 1.0 |
