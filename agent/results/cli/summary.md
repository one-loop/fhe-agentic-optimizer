# LLM optimization results (latest run per task)

fhe() time is the median evaluation time on the dense case. Levels, ciphertext-ciphertext multiplications (ct x ct), ciphertext-plaintext multiplications (ct x pt) and rotations are CHEHAB's counts for the baseline and for the best correct kernel.

Speedup is from baseline and best timed in alternating batches when the run remeasured it (marked r); older runs only have one median each. 'circuit' says whether the best kernel's circuit differs from the baseline's; 'new' counts distinct new correct circuits in the run.

| task | baseline ms | best ms | speedup | best attempt | correct | circuit | new | CHEHAB errors | levels | ct x ct | ct x pt | rotations | cost USD |
|---|---:|---:|---:|---:|---:|---|---:|---:|---|---|---|---|---:|
| batchnorm1d | 18.21 | 18.14 | x1.00 | 3 | 8/8 | same | 0 | 0 | 1 to 1 | 0 to 0 | 1 to 1 | 0 to 0 | 0.0 |
| batchnorm2d | 18.54 | 18.06 | x1.03 | 7 | 8/8 | same | 0 | 0 | 1 to 1 | 0 to 0 | 1 to 1 | 0 to 0 | 0.0 |
| chebyshev_elu_d7 | 394.26 | 369.6 | x1.07 | 7 | 2/8 | same | 0 | 6 | 8 to 8 | 6 to 6 | 8 to 8 | 0 to 0 | 0.0 |
| chebyshev_gelu_d7 | 314.7 | 214.06 | x1.47 | 8 | 7/8 | changed | 2 | 1 | 7 to 4 | 5 to 3 | 5 to 4 | 0 to 0 | 0.0 |
| chebyshev_hardshrink_d7 | 319.79 | 286.11 | x1.12 | 2 | 7/8 | changed | 1 | 1 | 8 to 5 | 6 to 4 | 5 to 6 | 0 to 0 | 0.0 |
| chebyshev_mish_d7 | 363.05 | 353.09 | x1.03 | 1 | 8/8 | changed | 4 | 0 | 8 to 5 | 6 to 4 | 8 to 9 | 0 to 0 | 0.0 |
| chebyshev_relu_d2 | 107.23 | 81.55 | x1.31 | 3 | 8/8 | changed | 1 | 0 | 3 to 3 | 1 to 1 | 3 to 2 | 0 to 0 | 0.0 |
| chebyshev_relu_d7 | 302.74 | 212.22 | x1.43 | 6 | 8/8 | changed | 3 | 0 | 7 to 4 | 5 to 3 | 5 to 4 | 0 to 0 | 0.0 |
| chebyshev_selu_d7 | 357.52 | 253.38 | x1.41 | 8 | 2/8 | changed | 2 | 6 | 8 to 8 | 6 to 6 | 8 to 2 | 0 to 0 | 0.0 |
| chebyshev_sigmoid_d5 | 255.23 | 227.53 | x1.12 | 3 | 7/8 | changed | 3 | 1 | 6 to 5 | 4 to 3 | 4 to 4 | 0 to 0 | 0.0 |
| chebyshev_sigmoid_d7 | 323.76 | 220.88 | x1.47 | 7 | 7/8 | changed | 4 | 1 | 8 to 6 | 6 to 3 | 5 to 3 | 0 to 0 | 0.0 |
| chebyshev_silu_d7 | 307.11 | 199.19 | x1.54 | 7 | 8/8 | changed | 4 | 0 | 7 to 5 | 5 to 3 | 5 to 3 | 0 to 0 | 0.0 |
| chebyshev_softplus_d7 | 313.32 | 217.91 | x1.44 | 8 | 6/8 | changed | 4 | 2 | 7 to 5 | 5 to 1 | 5 to 4 | 0 to 0 | 0.0 |
| elementwise_add | 0.72 | 0.72 | x1.00 | 0 | 8/8 | same | 0 | 0 | 0 to 0 | 0 to 0 | 0 to 0 | 0 to 0 | 0.0 |
| elementwise_add_plain | 4.69 | 4.64 | x1.01 | 3 | 8/8 | same | 0 | 0 | 0 to 0 | 0 to 0 | 0 to 0 | 0 to 0 | 0.0 |
| elementwise_add_scalar | 0.55 | 0.38 | x1.43 | 2 | 8/8 | same | 0 | 0 | 0 to 0 | 0 to 0 | 0 to 0 | 0 to 0 | 0.0 |
| elementwise_mul | 67.83 | 64.87 | x1.05 | 2 | 8/8 | same | 0 | 0 | 1 to 1 | 1 to 1 | 0 to 0 | 0 to 0 | 0.0 |
| elementwise_mul_plain | 13.52 | 13.03 | x1.04 | 3 | 8/8 | same | 0 | 0 | 1 to 1 | 0 to 0 | 1 to 1 | 0 to 0 | 0.0 |
| elementwise_mul_scalar | 8.89 | 8.8 | x1.01 | 4 | 8/8 | same | 0 | 0 | 1 to 1 | 0 to 0 | 1 to 1 | 0 to 0 | 0.0 |
| embedding | 1313.33 | 1276.17 | x1.03 | 4 | 8/8 | changed | 1 | 0 | 2 to 2 | 0 to 0 | 6 to 6 | 26 to 26 | 0.0 |
| extract | 60.48 | 59.61 | x1.01 | 6 | 8/8 | same | 0 | 0 | 1 to 1 | 0 to 0 | 1 to 1 | 1 to 1 | 0.0 |
| extract_linear | 59.42 | 59.23 | x1.00 | 3 | 7/8 | same | 0 | 1 | 1 to 1 | 0 to 0 | 1 to 1 | 1 to 1 | 0.0 |
| extract_sparse | 194.16 | 128.61 | x1.51 | 7 | 8/8 | changed | 2 | 0 | 1 to 1 | 0 to 0 | 3 to 2 | 3 to 2 | 0.0 |
| quad | 64.64 | 64.64 | x1.00 | 0 | 8/8 | same | 1 | 0 | 1 to 1 | 1 to 1 | 0 to 0 | 0 to 0 | 0.0 |
| relu_composite | 639.39 | 632.87 | x1.01 | 3 | 8/8 | changed | 3 | 0 | 10 to 10 | 16 to 15 | 12 to 11 | 0 to 0 | 0.0 |
| table_mult | 1230.94 | 1175.38 | x1.05 | 5 | 7/8 | same | 0 | 1 | 1 to 1 | 0 to 0 | 3 to 3 | 24 to 24 | 0.0 |

26 tasks; 15 measured 3% or more faster, 13 with a changed circuit; 188 correct attempts, 35 new circuits, 20 CHEHAB errors. Total cost $0.00. Models: claude-sonnet-5-5.
