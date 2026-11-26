# Compiler ladder: unoptimized vs CHEHAB rulesets vs the LLM loop

Commit 67c8d41bb9 (dirty), Darwin arm64 arm, Apple clang version 21.0.0 (clang-2100.1.1.101), SEAL 4.1, CKKS chain sized. 2026-11-22T16:52:19 to 2026-11-22T21:36:27.

Each task's programs were measured in one interleaved session (1 warmup + 10 measured processes of ./main --repeats 10); speedups are ratios of eval-only medians from that session. Every program was checked on the fixed, dense and 6 held-out cases; programs faster than the unoptimized baseline were re-checked 20 times per case (BFV: 2). A program that is not reliably correct is shown as `fail` and not counted. `CHEHAB best` = the fastest reliably correct ruleset for that task. `llm` = the LLM loop's best final kernel (selected in an earlier neutral session, re-measured here); `-` = no LLM kernel for the task on this machine.

## Speedup over the unoptimized baseline

| task | scheme | unopt. (ms) | simplification | depth | ops_cost | joined | CHEHAB best | llm | llm / CHEHAB default | llm / CHEHAB best |
|---|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|
| batchnorm1d | ckks | 2.01 | x0.998 | x0.998 | x0.996 | x0.998 | x0.998 | - | - | - |
| batchnorm2d | ckks | 2.01 | x1.001 | x1.001 | x1.000 | x0.999 | x1.001 | - | - | - |
| chebyshev_elu_d7 | ckks | 138.42 | x1.008 | x1.007 | x0.891 | rejected | x1.008 | x2.606 | x2.585 | x2.585 |
| chebyshev_gelu_d7 | ckks | 94.17 | x1.006 | x1.006 | x0.888 | rejected | x1.006 | x2.955 | x2.938 | x2.938 |
| chebyshev_hardshrink_d7 | ckks | 121.53 | x1.004 | x1.004 | x0.873 | rejected | x1.004 | - | - | - |
| chebyshev_mish_d7 | ckks | 137.79 | x1.005 | x1.006 | x0.888 | rejected | x1.006 | - | - | - |
| chebyshev_relu_d2 | ckks | 13.78 | x1.001 | x1.001 | x1.009 | x0.834 | x1.009 | - | - | - |
| chebyshev_relu_d7 | ckks | 94.07 | x1.005 | x1.005 | x0.888 | rejected | x1.005 | - | - | - |
| chebyshev_selu_d7 | ckks | 140.08 | x1.006 | x1.007 | x0.888 | rejected | x1.007 | - | - | - |
| chebyshev_sigmoid_d5 | ckks | 65.14 | x1.001 | x1.003 | x0.894 | rejected | x1.003 | - | - | - |
| chebyshev_sigmoid_d7 | ckks | 122.37 | x1.008 | x1.007 | x0.877 | rejected | x1.008 | x2.734 | x2.714 | x2.714 |
| chebyshev_silu_d7 | ckks | 95.41 | x1.005 | x1.004 | x0.886 | rejected | x1.005 | - | - | - |
| chebyshev_softplus_d7 | ckks | 95.54 | x1.006 | x1.005 | x0.887 | rejected | x1.006 | - | - | - |
| coyote_tree_100_100_5_1 | bfv | 806.81 | x0.999 | x0.999 | x1.000 | x0.999 | x1.000 | - | - | - |
| coyote_tree_100_50_5_1 | bfv | 320.38 | x0.999 | x1.000 | x1.000 | x1.000 | x1.000 | - | - | - |
| coyote_tree_50_50_10_2 | bfv | 425.87 | x1.001 | x1.001 | x1.001 | x1.001 | x1.001 | - | - | - |
| elementwise_add | ckks | 0.04 | x0.998 | x0.998 | x0.997 | x0.982 | x0.998 | - | - | - |
| elementwise_add_plain | ckks | 0.44 | x1.000 | x1.000 | x0.998 | x0.998 | x1.000 | - | - | - |
| elementwise_add_scalar | ckks | 0.02 | x1.001 | x1.002 | x1.002 | x1.001 | x1.002 | - | - | - |
| elementwise_mul | ckks | 3.48 | x1.000 | x1.001 | x1.000 | x1.000 | x1.001 | - | - | - |
| elementwise_mul_plain | ckks | 1.46 | x1.000 | x1.000 | x1.001 | x1.000 | x1.001 | - | - | - |
| elementwise_mul_scalar | ckks | 0.86 | x0.995 | x0.995 | x0.997 | x0.997 | x0.997 | - | - | - |
| embedding | ckks | 74.86 | x0.999 | x0.706 | x0.998 | x0.707 | x0.999 | - | - | - |
| extract | ckks | 2.73 | x1.002 | x0.998 | x1.001 | x0.999 | x1.002 | - | - | - |
| extract_linear | ckks | 2.72 | x0.996 | x0.993 | x1.000 | x0.996 | x1.000 | - | - | - |
| extract_sparse | ckks | 8.16 | x0.999 | x1.000 | x1.178 | x1.177 | x1.178 | x1.288 | x1.289 | x1.093 |
| matmul_2x2 | bfv | 212.33 | x0.999 | x0.999 | x0.998 | x0.998 | x0.999 | x1.559 | x1.561 | x1.561 |
| matmul_3x3 | bfv | 717.90 | x1.001 | x1.000 | x0.999 | x0.999 | x1.001 | - | - | - |
| nn_avgpool2d | bfv | 3.86 | x0.992 | x0.995 | x0.992 | x0.996 | x0.996 | - | - | - |
| nn_conv2d | bfv | 3849.82 | x1.002 | x1.000 | x0.997 | x0.997 | x1.002 | - | - | - |
| nn_linear | bfv | 958.37 | x0.999 | x1.000 | x1.001 | x1.000 | x1.001 | - | - | - |
| quad | ckks | 3.52 | x1.001 | x1.002 | x1.028 | x0.999 | x1.028 | - | - | - |
| relu_composite | ckks | 359.17 | x0.999 | x1.000 | rejected | x1.000 | x1.000 | - | - | - |
| table_mult | ckks | 35.18 | x1.001 | x0.691 | x0.999 | x0.690 | x1.001 | - | - | - |

## Across tasks (geometric means)

| comparison | geometric mean |
|---|---:|
| CHEHAB simplification vs unoptimized, every task | x1.001 (34 tasks) |
| CHEHAB best ruleset vs unoptimized, every task | x1.008 (34 tasks) |
| CHEHAB simplification vs unoptimized, tasks with an LLM kernel | x1.004 (5 tasks) |
| CHEHAB best ruleset vs unoptimized, tasks with an LLM kernel | x1.038 (5 tasks) |
| LLM kernel vs unoptimized | x2.115 (5 tasks) |
| LLM kernel vs CHEHAB simplification (the loops' reference) | x2.107 (5 tasks) |
| LLM kernel vs CHEHAB best ruleset | x2.038 (5 tasks) |

Identical generated code measured under two configurations shows the session's noise floor; differences of a few percent between such configurations are noise.

## Programs

| task | config | status | reliably correct | repeats | eval (ms) | levels | ct x ct | ct x pt | rot | align | C (ms) | note |
|---|---|---|---|---:|---:|---:|---:|---:|---:|---:|---:|---|
| chebyshev_gelu_d7 | base-none | ok | True | 160/160 | 94.17 | 7 | 5 | 5 | 0 | 7 | 0.54 |  |
| chebyshev_gelu_d7 | base-simplification | ok | True | 160/160 | 93.62 | 7 | 5 | 5 | 0 | 7 | 0.55 |  |
| chebyshev_gelu_d7 | base-depth | ok | True | 160/160 | 93.65 | 7 | 5 | 5 | 0 | 7 | 0.93 |  |
| chebyshev_gelu_d7 | base-ops_cost | ok | True | 0/0 | 106.06 | 7 | 5 | 6 | 0 | 9 | 1.47 |  |
| chebyshev_gelu_d7 | base-joined | chehab_error | False | 0/0 | - | None | None | None | None | None | - | CHEHAB rejected the kernel (case dense): |
| chebyshev_gelu_d7 | llm | ok | True | 160/160 | 31.86 | 4 | 3 | 4 | 0 | 2 | 0.46 | feedback-pilot-haiku-5-5/archive-explore/run2 (prior x2.942) |
| chebyshev_sigmoid_d7 | base-none | ok | True | 160/160 | 122.37 | 8 | 6 | 5 | 0 | 8 | 0.47 |  |
| chebyshev_sigmoid_d7 | base-simplification | ok | True | 160/160 | 121.44 | 8 | 6 | 5 | 0 | 8 | 0.51 |  |
| chebyshev_sigmoid_d7 | base-depth | ok | True | 160/160 | 121.52 | 8 | 6 | 5 | 0 | 8 | 0.99 |  |
| chebyshev_sigmoid_d7 | base-ops_cost | ok | True | 0/0 | 139.52 | 8 | 6 | 6 | 0 | 11 | 1.5 |  |
| chebyshev_sigmoid_d7 | base-joined | chehab_error | False | 0/0 | - | None | None | None | None | None | - | CHEHAB rejected the kernel (case dense): |
| chebyshev_sigmoid_d7 | llm | ok | True | 160/160 | 44.75 | 5 | 4 | 3 | 0 | 1 | 0.49 | feedback-pilot-haiku-5-5/linear-minimal/run1 (prior x2.713) |
| chebyshev_elu_d7 | base-none | ok | True | 160/160 | 138.42 | 8 | 6 | 8 | 0 | 11 | 0.48 |  |
| chebyshev_elu_d7 | base-simplification | ok | True | 160/160 | 137.32 | 8 | 6 | 8 | 0 | 11 | 0.54 |  |
| chebyshev_elu_d7 | base-depth | ok | True | 160/160 | 137.42 | 8 | 6 | 8 | 0 | 11 | 1.03 |  |
| chebyshev_elu_d7 | base-ops_cost | ok | True | 0/0 | 155.33 | 8 | 6 | 9 | 0 | 14 | 1.8 |  |
| chebyshev_elu_d7 | base-joined | chehab_error | False | 0/0 | - | None | None | None | None | None | - | CHEHAB rejected the kernel (case dense): |
| chebyshev_elu_d7 | llm | ok | True | 160/160 | 53.12 | 4 | 5 | 5 | 0 | 3 | 0.51 | feedback-pilot-haiku-5-5/archive-explore/run1 (prior x2.580) |
| extract_sparse | base-none | ok | True | 160/160 | 8.16 | 1 | 0 | 3 | 3 | 0 | 0.3 |  |
| extract_sparse | base-simplification | ok | True | 160/160 | 8.17 | 1 | 0 | 3 | 3 | 0 | 0.29 | same SEAL code as base-none |
| extract_sparse | base-depth | ok | True | 160/160 | 8.16 | 1 | 0 | 3 | 3 | 0 | 0.66 | same SEAL code as base-none |
| extract_sparse | base-ops_cost | ok | True | 160/160 | 6.93 | 1 | 0 | 3 | 2 | 0 | 0.95 |  |
| extract_sparse | base-joined | ok | True | 160/160 | 6.93 | 1 | 0 | 3 | 2 | 0 | 0.84 | same SEAL code as base-ops_cost |
| extract_sparse | llm | ok | True | 160/160 | 6.34 | 1 | 0 | 2 | 2 | 1 | 0.35 | feedback-pilot-haiku-5-5/archive/run2 (prior x1.300) |
| matmul_2x2 | base-none | ok | True | 16/16 | 212.33 | 0 | 8 | 0 | 0 | 0 | 0.25 |  |
| matmul_2x2 | base-simplification | ok | True | 16/16 | 212.61 | 0 | 8 | 0 | 0 | 0 | 0.25 |  |
| matmul_2x2 | base-depth | ok | True | 0/0 | 212.57 | 0 | 8 | 0 | 0 | 0 | 0.6 | same SEAL code as base-simplification |
| matmul_2x2 | base-ops_cost | ok | True | 0/0 | 212.73 | 0 | 8 | 0 | 0 | 0 | 0.98 | same SEAL code as base-simplification |
| matmul_2x2 | base-joined | ok | True | 0/0 | 212.78 | 0 | 8 | 0 | 0 | 0 | 0.77 | same SEAL code as base-simplification |
| matmul_2x2 | llm | ok | True | 16/16 | 136.19 | 0 | 4 | 4 | 2 | 0 | 0.29 | feedback-pilot-haiku-5-5/archive/run2 (prior x1.568) |
| coyote_tree_100_50_5_1 | base-none | ok | True | 16/16 | 320.38 | 0 | 12 | 0 | 0 | 0 | 0.26 |  |
| coyote_tree_100_50_5_1 | base-simplification | ok | True | 16/16 | 320.77 | 0 | 12 | 0 | 0 | 0 | 0.27 | same SEAL code as base-none |
| coyote_tree_100_50_5_1 | base-depth | ok | True | 16/16 | 320.38 | 0 | 12 | 0 | 0 | 0 | 0.73 | same SEAL code as base-none |
| coyote_tree_100_50_5_1 | base-ops_cost | ok | True | 0/0 | 320.46 | 0 | 12 | 0 | 0 | 0 | 1.17 | same SEAL code as base-none |
| coyote_tree_100_50_5_1 | base-joined | ok | True | 0/0 | 320.4 | 0 | 12 | 0 | 0 | 0 | 0.98 | same SEAL code as base-none |
| batchnorm1d | base-none | ok | True | 160/160 | 2.01 | 1 | 0 | 1 | 0 | 0 | 0.21 |  |
| batchnorm1d | base-simplification | ok | True | 160/160 | 2.01 | 1 | 0 | 1 | 0 | 0 | 0.28 |  |
| batchnorm1d | base-depth | ok | True | 0/0 | 2.01 | 1 | 0 | 1 | 0 | 0 | 0.55 | same SEAL code as base-simplification |
| batchnorm1d | base-ops_cost | ok | True | 0/0 | 2.02 | 1 | 0 | 1 | 0 | 0 | 0.97 | same SEAL code as base-simplification |
| batchnorm1d | base-joined | ok | True | 0/0 | 2.01 | 1 | 0 | 1 | 0 | 0 | 0.81 | same SEAL code as base-simplification |
| batchnorm2d | base-none | ok | True | 160/160 | 2.01 | 1 | 0 | 1 | 0 | 0 | 0.37 |  |
| batchnorm2d | base-simplification | ok | True | 160/160 | 2.01 | 1 | 0 | 1 | 0 | 0 | 0.29 |  |
| batchnorm2d | base-depth | ok | True | 160/160 | 2.01 | 1 | 0 | 1 | 0 | 0 | 0.61 | same SEAL code as base-simplification |
| batchnorm2d | base-ops_cost | ok | True | 160/160 | 2.01 | 1 | 0 | 1 | 0 | 0 | 0.92 | same SEAL code as base-simplification |
| batchnorm2d | base-joined | ok | True | 0/0 | 2.02 | 1 | 0 | 1 | 0 | 0 | 0.84 | same SEAL code as base-simplification |
| chebyshev_hardshrink_d7 | base-none | ok | True | 160/160 | 121.53 | 8 | 6 | 5 | 0 | 8 | 0.47 |  |
| chebyshev_hardshrink_d7 | base-simplification | ok | True | 160/160 | 121.0 | 8 | 6 | 5 | 0 | 8 | 0.54 |  |
| chebyshev_hardshrink_d7 | base-depth | ok | True | 160/160 | 120.99 | 8 | 6 | 5 | 0 | 8 | 0.96 | same SEAL code as base-simplification |
| chebyshev_hardshrink_d7 | base-ops_cost | ok | True | 0/0 | 139.17 | 8 | 6 | 6 | 0 | 11 | 1.57 |  |
| chebyshev_hardshrink_d7 | base-joined | chehab_error | False | 0/0 | - | None | None | None | None | None | - | CHEHAB rejected the kernel (case dense): |
| chebyshev_mish_d7 | base-none | ok | True | 160/160 | 137.79 | 8 | 6 | 8 | 0 | 11 | 0.48 |  |
| chebyshev_mish_d7 | base-simplification | ok | True | 160/160 | 137.07 | 8 | 6 | 8 | 0 | 11 | 0.5 |  |
| chebyshev_mish_d7 | base-depth | ok | True | 160/160 | 136.94 | 8 | 6 | 8 | 0 | 11 | 1.0 |  |
| chebyshev_mish_d7 | base-ops_cost | ok | True | 0/0 | 155.2 | 8 | 6 | 9 | 0 | 14 | 1.56 |  |
| chebyshev_mish_d7 | base-joined | chehab_error | False | 0/0 | - | None | None | None | None | None | - | CHEHAB rejected the kernel (case dense): |
| chebyshev_relu_d2 | base-none | ok | True | 160/160 | 13.78 | 3 | 1 | 3 | 0 | 1 | 0.34 |  |
| chebyshev_relu_d2 | base-simplification | ok | True | 160/160 | 13.76 | 3 | 1 | 3 | 0 | 1 | 0.33 |  |
| chebyshev_relu_d2 | base-depth | ok | True | 160/160 | 13.76 | 3 | 1 | 3 | 0 | 1 | 0.63 | same SEAL code as base-simplification |
| chebyshev_relu_d2 | base-ops_cost | ok | True | 160/160 | 13.65 | 3 | 1 | 3 | 0 | 1 | 1.06 |  |
| chebyshev_relu_d2 | base-joined | ok | True | 0/0 | 16.53 | 3 | 1 | 4 | 0 | 1 | 1.0 |  |
| chebyshev_relu_d7 | base-none | ok | True | 160/160 | 94.07 | 7 | 5 | 5 | 0 | 7 | 0.47 |  |
| chebyshev_relu_d7 | base-simplification | ok | True | 160/160 | 93.57 | 7 | 5 | 5 | 0 | 7 | 0.47 |  |
| chebyshev_relu_d7 | base-depth | ok | True | 160/160 | 93.63 | 7 | 5 | 5 | 0 | 7 | 0.88 |  |
| chebyshev_relu_d7 | base-ops_cost | ok | True | 0/0 | 105.91 | 7 | 5 | 6 | 0 | 9 | 1.44 |  |
| chebyshev_relu_d7 | base-joined | chehab_error | False | 0/0 | - | None | None | None | None | None | - | CHEHAB rejected the kernel (case dense): |
| chebyshev_selu_d7 | base-none | ok | True | 160/160 | 140.08 | 8 | 6 | 8 | 0 | 11 | 0.48 |  |
| chebyshev_selu_d7 | base-simplification | ok | True | 160/160 | 139.17 | 8 | 6 | 8 | 0 | 11 | 0.53 |  |
| chebyshev_selu_d7 | base-depth | ok | True | 160/160 | 139.06 | 8 | 6 | 8 | 0 | 11 | 1.05 |  |
| chebyshev_selu_d7 | base-ops_cost | ok | True | 0/0 | 157.71 | 8 | 6 | 9 | 0 | 14 | 1.66 |  |
| chebyshev_selu_d7 | base-joined | chehab_error | False | 0/0 | - | None | None | None | None | None | - | CHEHAB rejected the kernel (case dense): |
| chebyshev_sigmoid_d5 | base-none | ok | True | 160/160 | 65.14 | 6 | 4 | 4 | 0 | 5 | 0.47 |  |
| chebyshev_sigmoid_d5 | base-simplification | ok | True | 160/160 | 65.07 | 6 | 4 | 4 | 0 | 5 | 0.47 |  |
| chebyshev_sigmoid_d5 | base-depth | ok | True | 160/160 | 64.94 | 6 | 4 | 4 | 0 | 5 | 0.93 |  |
| chebyshev_sigmoid_d5 | base-ops_cost | ok | True | 0/0 | 72.83 | 6 | 4 | 5 | 0 | 6 | 1.39 |  |
| chebyshev_sigmoid_d5 | base-joined | chehab_error | False | 0/0 | - | None | None | None | None | None | - | CHEHAB rejected the kernel (case dense): |
| chebyshev_silu_d7 | base-none | ok | True | 160/160 | 95.41 | 7 | 5 | 5 | 0 | 7 | 0.5 |  |
| chebyshev_silu_d7 | base-simplification | ok | True | 160/160 | 94.96 | 7 | 5 | 5 | 0 | 7 | 0.52 |  |
| chebyshev_silu_d7 | base-depth | ok | True | 160/160 | 95.02 | 7 | 5 | 5 | 0 | 7 | 0.96 |  |
| chebyshev_silu_d7 | base-ops_cost | ok | True | 0/0 | 107.68 | 7 | 5 | 6 | 0 | 9 | 1.47 |  |
| chebyshev_silu_d7 | base-joined | chehab_error | False | 0/0 | - | None | None | None | None | None | - | CHEHAB rejected the kernel (case dense): |
| chebyshev_softplus_d7 | base-none | ok | True | 160/160 | 95.54 | 7 | 5 | 5 | 0 | 7 | 0.45 |  |
| chebyshev_softplus_d7 | base-simplification | ok | True | 160/160 | 94.93 | 7 | 5 | 5 | 0 | 7 | 0.5 |  |
| chebyshev_softplus_d7 | base-depth | ok | True | 160/160 | 95.09 | 7 | 5 | 5 | 0 | 7 | 0.93 |  |
| chebyshev_softplus_d7 | base-ops_cost | ok | True | 0/0 | 107.7 | 7 | 5 | 6 | 0 | 9 | 1.5 |  |
| chebyshev_softplus_d7 | base-joined | chehab_error | False | 0/0 | - | None | None | None | None | None | - | CHEHAB rejected the kernel (case dense): |
| coyote_tree_100_100_5_1 | base-none | ok | True | 16/16 | 806.81 | 0 | 30 | 0 | 0 | 0 | 0.29 |  |
| coyote_tree_100_100_5_1 | base-simplification | ok | True | 16/16 | 807.57 | 0 | 30 | 0 | 0 | 0 | 0.33 | same SEAL code as base-none |
| coyote_tree_100_100_5_1 | base-depth | ok | True | 0/0 | 807.57 | 0 | 30 | 0 | 0 | 0 | 0.74 | same SEAL code as base-none |
| coyote_tree_100_100_5_1 | base-ops_cost | ok | True | 0/0 | 807.02 | 0 | 30 | 0 | 0 | 0 | 1.11 | same SEAL code as base-none |
| coyote_tree_100_100_5_1 | base-joined | ok | True | 0/0 | 807.75 | 0 | 30 | 0 | 0 | 0 | 1.09 | same SEAL code as base-none |
| coyote_tree_50_50_10_2 | base-none | ok | True | 16/16 | 425.87 | 0 | 16 | 0 | 0 | 0 | 0.28 |  |
| coyote_tree_50_50_10_2 | base-simplification | ok | True | 16/16 | 425.65 | 0 | 16 | 0 | 0 | 0 | 0.34 |  |
| coyote_tree_50_50_10_2 | base-depth | ok | True | 16/16 | 425.52 | 0 | 16 | 0 | 0 | 0 | 0.87 |  |
| coyote_tree_50_50_10_2 | base-ops_cost | ok | True | 16/16 | 425.38 | 0 | 16 | 0 | 0 | 0 | 1.18 | same SEAL code as base-simplification |
| coyote_tree_50_50_10_2 | base-joined | ok | True | 16/16 | 425.58 | 0 | 16 | 0 | 0 | 0 | 1.07 | same SEAL code as base-depth |
| elementwise_add | base-none | ok | True | 160/160 | 0.04 | 0 | 0 | 0 | 0 | 0 | 0.33 |  |
| elementwise_add | base-simplification | ok | True | 160/160 | 0.04 | 0 | 0 | 0 | 0 | 0 | 0.33 | same SEAL code as base-none |
| elementwise_add | base-depth | ok | True | 0/0 | 0.04 | 0 | 0 | 0 | 0 | 0 | 0.74 | same SEAL code as base-none |
| elementwise_add | base-ops_cost | ok | True | 0/0 | 0.04 | 0 | 0 | 0 | 0 | 0 | 1.16 | same SEAL code as base-none |
| elementwise_add | base-joined | ok | True | 0/0 | 0.04 | 0 | 0 | 0 | 0 | 0 | 1.02 | same SEAL code as base-none |
| elementwise_add_plain | base-none | ok | True | 160/160 | 0.44 | 0 | 0 | 0 | 0 | 0 | 0.35 |  |
| elementwise_add_plain | base-simplification | ok | True | 160/160 | 0.44 | 0 | 0 | 0 | 0 | 0 | 0.35 | same SEAL code as base-none |
| elementwise_add_plain | base-depth | ok | True | 0/0 | 0.44 | 0 | 0 | 0 | 0 | 0 | 0.71 | same SEAL code as base-none |
| elementwise_add_plain | base-ops_cost | ok | True | 0/0 | 0.44 | 0 | 0 | 0 | 0 | 0 | 1.16 | same SEAL code as base-none |
| elementwise_add_plain | base-joined | ok | True | 0/0 | 0.44 | 0 | 0 | 0 | 0 | 0 | 0.96 | same SEAL code as base-none |
| elementwise_add_scalar | base-none | ok | True | 160/160 | 0.02 | 0 | 0 | 0 | 0 | 0 | 0.33 |  |
| elementwise_add_scalar | base-simplification | ok | True | 160/160 | 0.02 | 0 | 0 | 0 | 0 | 0 | 0.36 | same SEAL code as base-none |
| elementwise_add_scalar | base-depth | ok | True | 160/160 | 0.02 | 0 | 0 | 0 | 0 | 0 | 0.73 | same SEAL code as base-none |
| elementwise_add_scalar | base-ops_cost | ok | True | 160/160 | 0.02 | 0 | 0 | 0 | 0 | 0 | 1.2 | same SEAL code as base-none |
| elementwise_add_scalar | base-joined | ok | True | 160/160 | 0.02 | 0 | 0 | 0 | 0 | 0 | 1.06 | same SEAL code as base-none |
| elementwise_mul | base-none | ok | True | 160/160 | 3.48 | 1 | 1 | 0 | 0 | 0 | 0.28 |  |
| elementwise_mul | base-simplification | ok | True | 160/160 | 3.48 | 1 | 1 | 0 | 0 | 0 | 0.33 | same SEAL code as base-none |
| elementwise_mul | base-depth | ok | True | 160/160 | 3.48 | 1 | 1 | 0 | 0 | 0 | 0.6 | same SEAL code as base-none |
| elementwise_mul | base-ops_cost | ok | True | 160/160 | 3.48 | 1 | 1 | 0 | 0 | 0 | 0.94 | same SEAL code as base-none |
| elementwise_mul | base-joined | ok | True | 160/160 | 3.48 | 1 | 1 | 0 | 0 | 0 | 0.87 | same SEAL code as base-none |
| elementwise_mul_plain | base-none | ok | True | 160/160 | 1.46 | 1 | 0 | 1 | 0 | 0 | 0.3 |  |
| elementwise_mul_plain | base-simplification | ok | True | 160/160 | 1.46 | 1 | 0 | 1 | 0 | 0 | 0.3 | same SEAL code as base-none |
| elementwise_mul_plain | base-depth | ok | True | 160/160 | 1.46 | 1 | 0 | 1 | 0 | 0 | 0.63 | same SEAL code as base-none |
| elementwise_mul_plain | base-ops_cost | ok | True | 160/160 | 1.46 | 1 | 0 | 1 | 0 | 0 | 0.99 | same SEAL code as base-none |
| elementwise_mul_plain | base-joined | ok | True | 160/160 | 1.46 | 1 | 0 | 1 | 0 | 0 | 0.81 | same SEAL code as base-none |
| elementwise_mul_scalar | base-none | ok | True | 160/160 | 0.86 | 1 | 0 | 1 | 0 | 0 | 0.25 |  |
| elementwise_mul_scalar | base-simplification | ok | True | 160/160 | 0.86 | 1 | 0 | 1 | 0 | 0 | 0.29 | same SEAL code as base-none |
| elementwise_mul_scalar | base-depth | ok | True | 0/0 | 0.86 | 1 | 0 | 1 | 0 | 0 | 0.64 | same SEAL code as base-none |
| elementwise_mul_scalar | base-ops_cost | ok | True | 0/0 | 0.86 | 1 | 0 | 1 | 0 | 0 | 1.02 | same SEAL code as base-none |
| elementwise_mul_scalar | base-joined | ok | True | 0/0 | 0.86 | 1 | 0 | 1 | 0 | 0 | 1.04 | same SEAL code as base-none |
| embedding | base-none | ok | True | 160/160 | 74.86 | 2 | 0 | 6 | 26 | 0 | 0.35 |  |
| embedding | base-simplification | ok | True | 160/160 | 74.91 | 2 | 0 | 6 | 26 | 0 | 0.34 |  |
| embedding | base-depth | ok | True | 0/0 | 106.07 | 2 | 0 | 6 | 38 | 0 | 1.16 |  |
| embedding | base-ops_cost | ok | True | 0/0 | 75.04 | 2 | 0 | 6 | 26 | 0 | 1.31 | same SEAL code as base-simplification |
| embedding | base-joined | ok | True | 0/0 | 105.84 | 2 | 0 | 6 | 38 | 0 | 1.51 | same SEAL code as base-depth |
| extract | base-none | ok | True | 160/160 | 2.73 | 1 | 0 | 1 | 1 | 0 | 0.27 |  |
| extract | base-simplification | ok | True | 160/160 | 2.72 | 1 | 0 | 1 | 1 | 0 | 0.28 | same SEAL code as base-none |
| extract | base-depth | ok | True | 0/0 | 2.73 | 1 | 0 | 1 | 1 | 0 | 0.66 | same SEAL code as base-none |
| extract | base-ops_cost | ok | True | 160/160 | 2.73 | 1 | 0 | 1 | 1 | 0 | 0.97 | same SEAL code as base-none |
| extract | base-joined | ok | True | 0/0 | 2.73 | 1 | 0 | 1 | 1 | 0 | 0.93 | same SEAL code as base-none |
| extract_linear | base-none | ok | True | 160/160 | 2.72 | 1 | 0 | 1 | 1 | 0 | 0.28 |  |
| extract_linear | base-simplification | ok | True | 160/160 | 2.73 | 1 | 0 | 1 | 1 | 0 | 0.33 | same SEAL code as base-none |
| extract_linear | base-depth | ok | True | 0/0 | 2.73 | 1 | 0 | 1 | 1 | 0 | 0.64 | same SEAL code as base-none |
| extract_linear | base-ops_cost | ok | True | 0/0 | 2.72 | 1 | 0 | 1 | 1 | 0 | 1.06 | same SEAL code as base-none |
| extract_linear | base-joined | ok | True | 0/0 | 2.73 | 1 | 0 | 1 | 1 | 0 | 0.85 | same SEAL code as base-none |
| matmul_3x3 | base-none | ok | True | 16/16 | 717.9 | 0 | 27 | 0 | 0 | 0 | 0.32 |  |
| matmul_3x3 | base-simplification | ok | True | 16/16 | 717.54 | 0 | 27 | 0 | 0 | 0 | 0.42 |  |
| matmul_3x3 | base-depth | ok | True | 16/16 | 717.76 | 0 | 27 | 0 | 0 | 0 | 0.8 | same SEAL code as base-simplification |
| matmul_3x3 | base-ops_cost | ok | True | 0/0 | 718.35 | 0 | 27 | 0 | 0 | 0 | 1.25 | same SEAL code as base-simplification |
| matmul_3x3 | base-joined | ok | True | 0/0 | 718.72 | 0 | 27 | 0 | 0 | 0 | 1.22 | same SEAL code as base-simplification |
| nn_avgpool2d | base-none | ok | True | 16/16 | 3.86 | 0 | 0 | 0 | 0 | 0 | 0.27 |  |
| nn_avgpool2d | base-simplification | ok | True | 16/16 | 3.89 | 0 | 0 | 0 | 0 | 0 | 0.26 |  |
| nn_avgpool2d | base-depth | ok | True | 0/0 | 3.87 | 0 | 0 | 0 | 0 | 0 | 0.85 |  |
| nn_avgpool2d | base-ops_cost | ok | True | 0/0 | 3.89 | 0 | 0 | 0 | 0 | 0 | 1.14 | same SEAL code as base-simplification |
| nn_avgpool2d | base-joined | ok | True | 0/0 | 3.87 | 0 | 0 | 0 | 0 | 0 | 1.07 | same SEAL code as base-depth |
| nn_conv2d | base-none | ok | True | 16/16 | 3849.82 | 0 | 144 | 0 | 0 | 0 | 0.59 |  |
| nn_conv2d | base-simplification | ok | True | 16/16 | 3841.11 | 0 | 144 | 0 | 0 | 0 | 0.75 |  |
| nn_conv2d | base-depth | ok | True | 16/16 | 3848.81 | 0 | 144 | 0 | 0 | 0 | 5.17 |  |
| nn_conv2d | base-ops_cost | ok | True | 0/0 | 3861.87 | 0 | 144 | 0 | 0 | 0 | 2.88 | same SEAL code as base-simplification |
| nn_conv2d | base-joined | ok | True | 0/0 | 3863.1 | 0 | 144 | 0 | 0 | 0 | 5.52 | same SEAL code as base-depth |
| nn_linear | base-none | ok | True | 16/16 | 958.37 | 0 | 36 | 0 | 0 | 0 | 0.36 |  |
| nn_linear | base-simplification | ok | True | 16/16 | 959.14 | 0 | 36 | 0 | 0 | 0 | 0.37 |  |
| nn_linear | base-depth | ok | True | 0/0 | 958.41 | 0 | 36 | 0 | 0 | 0 | 1.47 |  |
| nn_linear | base-ops_cost | ok | True | 16/16 | 957.11 | 0 | 36 | 0 | 0 | 0 | 1.4 | same SEAL code as base-simplification |
| nn_linear | base-joined | ok | True | 16/16 | 958.26 | 0 | 36 | 0 | 0 | 0 | 1.77 | same SEAL code as base-depth |
| quad | base-none | ok | True | 160/160 | 3.52 | 1 | 1 | 0 | 0 | 0 | 0.27 |  |
| quad | base-simplification | ok | True | 160/160 | 3.52 | 1 | 1 | 0 | 0 | 0 | 0.31 | same SEAL code as base-none |
| quad | base-depth | ok | True | 160/160 | 3.52 | 1 | 1 | 0 | 0 | 0 | 0.57 | same SEAL code as base-none |
| quad | base-ops_cost | ok | True | 160/160 | 3.43 | 1 | 1 | 0 | 0 | 0 | 0.97 |  |
| quad | base-joined | ok | True | 0/0 | 3.53 | 1 | 1 | 0 | 0 | 0 | 0.88 | same SEAL code as base-none |
| relu_composite | base-none | ok | True | 160/160 | 359.17 | 10 | 16 | 12 | 0 | 9 | 0.45 |  |
| relu_composite | base-simplification | ok | True | 160/160 | 359.46 | 10 | 16 | 12 | 0 | 9 | 0.48 |  |
| relu_composite | base-depth | ok | True | 0/0 | 359.29 | 10 | 16 | 12 | 0 | 9 | 1.08 |  |
| relu_composite | base-ops_cost | chehab_error | False | 0/0 | - | None | None | None | None | None | - | CHEHAB rejected the kernel (case dense): |
| relu_composite | base-joined | ok | True | 160/160 | 359.04 | 10 | 16 | 12 | 0 | 9 | 1.17 | same SEAL code as base-depth |
| table_mult | base-none | ok | True | 160/160 | 35.18 | 1 | 0 | 3 | 24 | 0 | 0.34 |  |
| table_mult | base-simplification | ok | True | 160/160 | 35.14 | 1 | 0 | 3 | 24 | 0 | 0.36 | same SEAL code as base-none |
| table_mult | base-depth | ok | True | 0/0 | 50.95 | 1 | 0 | 3 | 36 | 0 | 1.09 |  |
| table_mult | base-ops_cost | ok | True | 0/0 | 35.2 | 1 | 0 | 3 | 24 | 0 | 1.3 | same SEAL code as base-none |
| table_mult | base-joined | ok | True | 0/0 | 51.01 | 1 | 0 | 3 | 36 | 0 | 1.32 | same SEAL code as base-depth |
