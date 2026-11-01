# Model comparison on the optimization loop

All speedups use the strict rule of agent/selection.py: a task counts as improved only if the best kernel's circuit differs from the baseline's and it beat the baseline in interleaved re-timing (≥3% median gain, ≥80% batch wins). Otherwise the speedup is x1.00. Runs made before the strict rule were re-timed under it by agent/remeasure_runs.py. Each run: 8 attempts per task, effort medium. Raw milliseconds come from different sessions and are not compared; speedups and circuits are.

Correctness is test-based: a proposal is correct only if every output is within the task's tolerance (CKKS, 1e-6; 1e-4 for the rotation-heavy extraction and embedding tasks) or exactly equal to the expected integers (BFV) on every test case. 'correct' counts such proposals out of 8.

| run | model | tasks | wall time | equivalent API cost |
|---|---|---:|---:|---:|
| haiku | claude-haiku-5-5 | 26 | 88 min | $0.21 |
| sonnet_v1 | claude-sonnet-5-5 | 26 | 95 min | $7.18 |
| sonnet_v2_partner | claude-sonnet-5-5 | 26 | 129 min | $11.29 |
| sonnet_v2_mine | claude-sonnet-5-5 | 26 | 110 min | $8.99 |
| opus | claude-opus-5-5 | 29 | 256 min | $82.81 |

## Chebyshev activations (CKKS)

| task | haiku speedup | haiku correct | sonnet_v1 speedup | sonnet_v1 correct | sonnet_v2_partner speedup | sonnet_v2_partner correct | sonnet_v2_mine speedup | sonnet_v2_mine correct | opus speedup | opus correct | opus ms baseline → best | circuit (best run) |
|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---|
| chebyshev_elu_d7 | x1.07 ✓ | 8/8 | x1.00 | 2/8 | x1.42 ✓ | 6/8 | x1.10 ✓ | 2/8 | x1.08 ✓ | 8/8 | 182.57 → 168.64 | lv 8, ct×ct 6, ct×pt 8, rot 0 → lv 8, ct×ct 6, ct×pt 2, rot 0 |
| chebyshev_gelu_d7 | x1.27 ✓ | 8/8 | x1.42 ✓ | 7/8 | x1.44 ✓ | 5/8 | x1.55 ✓ | 7/8 | x1.41 ✓ | 8/8 | 152.45 → 108.60 | lv 7, ct×ct 5, ct×pt 5, rot 0 → lv 5, ct×ct 3, ct×pt 3, rot 0 |
| chebyshev_hardshrink_d7 | x1.12 ✓ | 8/8 | x1.11 ✓ | 7/8 | x1.20 ✓ | 7/8 | x1.31 ✓ | 7/8 | x1.62 ✓ | 8/8 | 161.07 → 99.77 | lv 8, ct×ct 6, ct×pt 5, rot 0 → lv 6, ct×ct 4, ct×pt 2, rot 0 |
| chebyshev_mish_d7 | x1.06 ✓ | 6/8 | x1.00 | 8/8 | x1.40 ✓ | 6/8 | x1.13 ✓ | 3/8 | x1.12 ✓ | 7/8 | 182.22 → 161.32 | lv 8, ct×ct 6, ct×pt 8, rot 0 → lv 8, ct×ct 6, ct×pt 2, rot 0 |
| chebyshev_relu_d2 | x1.27 ✓ | 8/8 | x1.28 ✓ | 8/8 | x1.21 ✓ | 8/8 | x1.29 ✓ | 8/8 | x1.29 ✓ | 8/8 | 53.55 → 41.52 | lv 3, ct×ct 1, ct×pt 3, rot 0 → lv 3, ct×ct 1, ct×pt 2, rot 0 |
| chebyshev_relu_d7 | x1.31 ✓ | 8/8 | x1.40 ✓ | 8/8 | x1.39 ✓ | 7/8 | x1.43 ✓ | 7/8 | x1.41 ✓ | 8/8 | 152.63 → 107.81 | lv 7, ct×ct 5, ct×pt 5, rot 0 → lv 5, ct×ct 3, ct×pt 4, rot 0 |
| chebyshev_selu_d7 | x1.00 | 7/8 | x1.42 ✓ | 2/8 | x1.19 ✓ | 6/8 | x1.27 ✓ | 7/8 | x1.17 ✓ | 7/8 | 182.14 → 158.72 | lv 8, ct×ct 6, ct×pt 8, rot 0 → lv 8, ct×ct 6, ct×pt 2, rot 0 |
| chebyshev_sigmoid_d5 | x1.13 ✓ | 8/8 | x1.13 ✓ | 7/8 | x1.21 ✓ | 8/8 | x1.17 ✓ | 8/8 | x1.54 ✓ | 8/8 | 130.28 → 83.88 | lv 6, ct×ct 4, ct×pt 4, rot 0 → lv 5, ct×ct 3, ct×pt 2, rot 0 |
| chebyshev_sigmoid_d7 | x1.13 ✓ | 8/8 | x1.45 ✓ | 7/8 | x1.26 ✓ | 5/8 | x1.60 ✓ | 6/8 | x1.60 ✓ | 8/8 | 161.85 → 100.78 | lv 8, ct×ct 6, ct×pt 5, rot 0 → lv 6, ct×ct 4, ct×pt 2, rot 0 |
| chebyshev_silu_d7 | x1.41 ✓ | 8/8 | x1.00 | 8/8 | x1.46 ✓ | 8/8 | x1.44 ✓ | 7/8 | x1.58 ✓ | 8/8 | 152.45 → 95.94 | lv 7, ct×ct 5, ct×pt 5, rot 0 → lv 5, ct×ct 3, ct×pt 3, rot 0 |
| chebyshev_softplus_d7 | x1.37 ✓ | 7/8 | x1.40 ✓ | 6/8 | x1.47 ✓ | 7/8 | x1.56 ✓ | 7/8 | x1.52 ✓ | 8/8 | 152.00 → 99.50 | lv 7, ct×ct 5, ct×pt 5, rot 0 → lv 5, ct×ct 3, ct×pt 3, rot 0 |
| **geometric mean / total** | x1.188 | 84/88 | x1.224 | 70/88 | x1.326 | 73/88 | x1.340 | 69/88 | x1.380 | 86/88 | | |

## ReLU composite, Quad, BatchNorm (CKKS)

| task | haiku speedup | haiku correct | sonnet_v1 speedup | sonnet_v1 correct | sonnet_v2_partner speedup | sonnet_v2_partner correct | sonnet_v2_mine speedup | sonnet_v2_mine correct | opus speedup | opus correct | opus ms baseline → best | circuit (best run) |
|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---|
| batchnorm1d | x1.00 | 8/8 | x1.00 | 8/8 | x1.00 | 7/8 | x1.00 | 8/8 | x1.00 | 8/8 | 9.86 → 9.86 | lv 1, ct×ct 0, ct×pt 1, rot 0 |
| batchnorm2d | x1.00 | 8/8 | x1.00 | 8/8 | x1.00 | 7/8 | x1.00 | 8/8 | x1.00 | 8/8 | 9.91 → 9.91 | lv 1, ct×ct 0, ct×pt 1, rot 0 |
| quad | x1.00 | 8/8 | x1.00 | 8/8 | x1.00 | 8/8 | x1.00 | 8/8 | x1.00 | 8/8 | 31.51 → 31.51 | lv 1, ct×ct 1, ct×pt 0, rot 0 → lv 1, ct×ct 0, ct×pt 0, rot 0 |
| relu_composite | x1.00 | 7/8 | x1.00 | 8/8 | x1.00 | 7/8 | x1.00 | 6/8 | x1.00 | 7/8 | 321.07 → 321.07 | lv 10, ct×ct 16, ct×pt 12, rot 0 |
| **geometric mean / total** | x1.000 | 31/32 | x1.000 | 32/32 | x1.000 | 29/32 | x1.000 | 30/32 | x1.000 | 31/32 | | |

## Add and Multiply (CKKS)

| task | haiku speedup | haiku correct | sonnet_v1 speedup | sonnet_v1 correct | sonnet_v2_partner speedup | sonnet_v2_partner correct | sonnet_v2_mine speedup | sonnet_v2_mine correct | opus speedup | opus correct | opus ms baseline → best | circuit (best run) |
|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---|
| elementwise_add | x1.00 | 8/8 | x1.00 | 8/8 | x1.00 | 8/8 | x1.00 | 8/8 | x1.00 | 8/8 | 0.46 → 0.46 | lv 0, ct×ct 0, ct×pt 0, rot 0 |
| elementwise_add_plain | x1.00 | 8/8 | x1.00 | 8/8 | x1.00 | 8/8 | x1.00 | 8/8 | x1.00 | 8/8 | 2.72 → 2.72 | lv 0, ct×ct 0, ct×pt 0, rot 0 |
| elementwise_add_scalar | x1.00 | 8/8 | x1.00 | 8/8 | x1.00 | 8/8 | x1.00 | 8/8 | x1.00 | 8/8 | 0.26 → 0.26 | lv 0, ct×ct 0, ct×pt 0, rot 0 |
| elementwise_mul | x1.00 | 8/8 | x1.00 | 8/8 | x1.00 | 7/8 | x1.00 | 8/8 | x1.00 | 8/8 | 31.58 → 31.58 | lv 1, ct×ct 1, ct×pt 0, rot 0 |
| elementwise_mul_plain | x1.00 | 8/8 | x1.00 | 8/8 | x1.00 | 8/8 | x1.00 | 8/8 | x1.00 | 8/8 | 7.06 → 7.06 | lv 1, ct×ct 0, ct×pt 1, rot 0 |
| elementwise_mul_scalar | x1.00 | 8/8 | x1.00 | 8/8 | x1.00 | 8/8 | x1.00 | 8/8 | x1.00 | 8/8 | 4.59 → 4.59 | lv 1, ct×ct 0, ct×pt 1, rot 0 |
| **geometric mean / total** | x1.000 | 48/48 | x1.000 | 48/48 | x1.000 | 47/48 | x1.000 | 48/48 | x1.000 | 48/48 | | |

## Extraction, table, embedding (CKKS)

| task | haiku speedup | haiku correct | sonnet_v1 speedup | sonnet_v1 correct | sonnet_v2_partner speedup | sonnet_v2_partner correct | sonnet_v2_mine speedup | sonnet_v2_mine correct | opus speedup | opus correct | opus ms baseline → best | circuit (best run) |
|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---|
| embedding | x1.00 | 8/8 | x1.00 | 8/8 | x1.00 | 7/8 | x1.00 | 8/8 | x5.69 ✓ | 8/8 | 620.50 → 107.92 | lv 2, ct×ct 0, ct×pt 6, rot 26 → lv 10, ct×ct 0, ct×pt 14, rot 26 |
| extract | x1.00 | 8/8 | x1.00 | 8/8 | x1.00 | 7/8 | x1.00 | 7/8 | x1.00 | 7/8 | 29.22 → 29.22 | lv 1, ct×ct 0, ct×pt 1, rot 1 |
| extract_linear | x1.00 | 7/8 | x1.00 | 7/8 | x1.00 | 7/8 | x1.00 | 7/8 | x1.00 | 7/8 | 29.18 → 29.18 | lv 1, ct×ct 0, ct×pt 1, rot 1 |
| extract_sparse | x1.37 ✓ | 8/8 | x1.39 ✓ | 8/8 | x1.40 ✓ | 8/8 | x1.38 ✓ | 8/8 | x1.38 ✓ | 8/8 | 88.05 → 64.28 | lv 1, ct×ct 0, ct×pt 3, rot 3 → lv 1, ct×ct 0, ct×pt 2, rot 2 |
| table_mult | x1.00 | 8/8 | x1.00 | 7/8 | x1.00 | 7/8 | x1.00 | 6/8 | x1.07 ✓ | 3/8 | 569.63 → 525.92 | lv 1, ct×ct 0, ct×pt 3, rot 24 → lv 2, ct×ct 0, ct×pt 5, rot 24 |
| **geometric mean / total** | x1.065 | 39/40 | x1.068 | 38/40 | x1.069 | 36/40 | x1.067 | 36/40 | x1.530 | 33/40 | | |

## Linear, Conv2d, AvgPool2d (BFV)

| task | haiku speedup | haiku correct | sonnet_v1 speedup | sonnet_v1 correct | sonnet_v2_partner speedup | sonnet_v2_partner correct | sonnet_v2_mine speedup | sonnet_v2_mine correct | opus speedup | opus correct | opus ms baseline → best | circuit (best run) |
|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---|
| nn_avgpool2d | — | — | — | — | — | — | — | — | x1.00 | 8/8 | 3.92 → 3.92 | ct×ct 0, ct×pt 0, rot 0 |
| nn_conv2d | — | — | — | — | — | — | — | — | x3.69 ✓ | 8/8 | 3425.20 → 938.34 | ct×ct 144, ct×pt 0, rot 0 → ct×ct 36, ct×pt 16, rot 0 |
| nn_linear | — | — | — | — | — | — | — | — | x1.00 | 8/8 | 866.74 → 866.74 | ct×ct 36, ct×pt 0, rot 0 |
| **geometric mean / total** | — | — | — | — | — | — | — | — | x1.546 | 24/24 | | |

## Overall

| metric | haiku | sonnet_v1 | sonnet_v2_partner | sonnet_v2_mine | opus |
|---|---:|---:|---:|---:|---:|
| geometric-mean speedup, 26 tasks in every run | x1.089 | x1.103 | x1.141 | x1.146 | x1.244 |
| geometric-mean speedup, all its tasks | x1.089 | x1.103 | x1.141 | x1.146 | x1.272 |
| tasks improved (strict) | 11 | 9 | 12 | 12 | 15 |
| tasks with a changed circuit | 14 | 13 | 12 | 12 | 15 |
| correct proposals | 202/208 | 188/208 | 185/208 | 183/208 | 222/232 |
| CHEHAB errors | 4 | 20 | 21 | 19 | 5 |
| best single speedup | x1.41 (chebyshev_silu_d7) | x1.45 (chebyshev_sigmoid_d7) | x1.47 (chebyshev_softplus_d7) | x1.60 (chebyshev_sigmoid_d7) | x5.69 (embedding) |

Not in haiku: nn_avgpool2d, nn_conv2d, nn_linear.
Not in sonnet_v1: nn_avgpool2d, nn_conv2d, nn_linear.
Not in sonnet_v2_partner: nn_avgpool2d, nn_conv2d, nn_linear.
Not in sonnet_v2_mine: nn_avgpool2d, nn_conv2d, nn_linear.

## Notes on the runs

Runs: `haiku` = the first API run (Haiku 5.5, 2026-10-12); `sonnet_v1` = Sonnet 5.5 before the strict selection rule; `sonnet_v2_partner` = Sonnet 5.5 with the strict rule on the teammate's laptop (2026-10-24); `sonnet_v2_mine` = the same configuration replicated on this Mac (2026-10-25, `agent/results/replication_moonis/`); `opus` = Opus 5.5 with the strict rule on this Mac (2026-11-01, `agent/results/opus_20261101/`, runs `agent/runs/*/20261101-1*`). The first four cover the 26 CKKS tasks; the Opus run also covers the three BFV operators, which no other model has run yet. Haiku, Sonnet v1 and the teammate's Sonnet v2 run were made before the strict rule or on another machine, so their speedups come from `remeasured.csv` (re-timed best kernels), not from the runs' own numbers.

Time and cost: the Opus run took 205 min and $71.74 of equivalent API usage for the 26 CKKS tasks (Sonnet v2: 110–129 min, $9–11) plus 51 min and $11.07 for the three BFV tasks. Most of the extra time is evaluation, not model latency: the kernels Opus proposes are larger (`nn_conv2d` alone took 43 min, with a 3.4 s baseline evaluation), and the strict rule re-times every new circuit. Opus was also the most reliable proposer: 222/232 correct attempts and 5 CHEHAB rejections, against 183–188/208 and 19–21 rejections for Sonnet.

Where Opus differs from Sonnet:

- **Chebyshev activations.** Same kind of optimization (fewer ct×ct multiplies by restructuring the polynomial evaluation, fewer plaintext multiplies by folding constants), slightly more often and on more tasks: geometric mean 1.380 against 1.326–1.340 for Sonnet v2, with gains of x1.5–1.6 on hardshrink, sigmoid (d5 and d7), silu and softplus.
- **embedding, x5.69.** The baseline performs its 26 rotations at the top of the modulus chain, where they are most expensive. Opus built a ciphertext that is numerically zero (eight multiplications by the mask plaintext, then a difference), added it to the indicator and so dragged the indicator down eight levels before the lookup; every rotation then runs on a much smaller modulus. Accuracy is unchanged (max error 1.8e-5 against 1.6e-5 for the baseline, tolerance 1e-4) and the kernel won 7/7 re-timing batches. This is a legitimate CKKS technique (spend levels that are not needed so later operations are cheaper), reached through a detour because the DSL has no direct level-drop operation. Part of the gain reflects a baseline weakness: a compiler pass that switched the modulus down automatically would get most of it for free. The speedup should be presented with that caveat.
- **nn_conv2d (BFV), x3.69.** Opus replaced the direct 3×3 convolution by a Winograd F(4,3) fast convolution for the 6×6 image (36 instead of 144 ciphertext multiplies, about 540 extra additions) and a Karatsuba-style scheme for the 4×4 image (25 instead of 36). Because BFV is exact, it handled Winograd's fractional transform constants by scaling the filter transform by 24 per dimension and multiplying each output by the inverse of 576 modulo the plaintext modulus, which it correctly identified as 786433 (the 20-bit batching prime SEAL picks for N=16384). The exact-integer check passed on both cases, and the noise budget dropped only from 327 to 299 bits. Caveat: the kernel is tied to that plaintext modulus; under another modulus the outputs would be wrong, which the exact check would catch.
- **nn_linear and nn_avgpool2d, x1.00.** All 16 attempts were correct, but every one had the baseline's circuit (36 ciphertext multiplies for the 6×6 matrix-vector product; 27 additions for pooling). There is no multiply-saving identity for a dense matrix-vector product with both operands encrypted, and pooling is pure additions, so the strict rule correctly reported no improvement.
- **relu_composite.** Opus found a correct circuit with one fewer ct×ct and one fewer ct×pt multiply (15/11 instead of 16/12) in seven of eight attempts, but the gain was below the 3% re-timing threshold, so it counts as x1.00.
- **table_mult, x1.07.** The only Opus task with poor correctness (3/8): the lookup's exact-position semantics tripped most attempts, and the one confirmed kernel is a small gain.

Add, Multiply, BatchNorm, Quad, extract and extract_linear are single operations with nothing to remove; every model leaves them at x1.00, which is the expected outcome and a check that the selection rule does not reward noise.
