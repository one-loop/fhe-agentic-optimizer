# LLM optimization results (latest run per task)

fhe() time is the median evaluation time on the dense case. Levels, ciphertext-ciphertext multiplications (ct x ct), ciphertext-plaintext multiplications (ct x pt) and rotations are CHEHAB's counts for the baseline and for the best correct kernel.

Speedup is from baseline and best timed in alternating batches when the run remeasured it (marked r); older runs only have one median each. 'circuit' says whether the best kernel's circuit differs from the baseline's; 'new' counts distinct new correct circuits in the run.

| task | baseline ms | best ms | speedup | best attempt | correct | circuit | new | CHEHAB errors | levels | ct x ct | ct x pt | rotations | cost USD |
|---|---:|---:|---:|---:|---:|---|---:|---:|---|---|---|---|---:|
| batchnorm1d | 17.9 | 17.9 | x1.00 | 0 | 8/8 | same | 1 | 0 | 1 to 1 | 0 to 0 | 1 to 1 | 0 to 0 | 0.0 |
| batchnorm2d | 22.79 | 22.79 | x1.00 | 0 | 8/8 | same | 1 | 0 | 1 to 1 | 0 to 0 | 1 to 1 | 0 to 0 | 0.0 |
| chebyshev_elu_d7 | 363.97 | 334.62 | x1.07 r | 6 | 3/8 | changed | 3 | 5 | 8 to 6 | 6 to 5 | 8 to 7 | 0 to 0 | 0.0 |
| chebyshev_gelu_d7 | 320.25 | 214.56 | x1.45 r | 6 | 7/8 | changed | 7 | 1 | 7 to 5 | 5 to 3 | 5 to 4 | 0 to 0 | 0.0 |
| chebyshev_hardshrink_d7 | 335.88 | 257.24 | x1.28 r | 4 | 6/8 | changed | 5 | 2 | 8 to 5 | 6 to 4 | 5 to 4 | 0 to 0 | 0.0 |
| chebyshev_mish_d7 | 359.77 | 346.62 | x1.04 r | 2 | 7/8 | changed | 6 | 1 | 8 to 5 | 6 to 4 | 8 to 9 | 0 to 0 | 0.0 |
| chebyshev_relu_d2 | 104.55 | 81.23 | x1.28 r | 2 | 7/8 | changed | 3 | 1 | 3 to 3 | 1 to 1 | 3 to 2 | 0 to 0 | 0.0 |
| chebyshev_relu_d7 | 303.21 | 215.18 | x1.41 r | 2 | 8/8 | changed | 8 | 0 | 7 to 4 | 5 to 3 | 5 to 4 | 0 to 0 | 0.0 |
| chebyshev_selu_d7 | 358.61 | 345.59 | x1.04 r | 2 | 7/8 | changed | 6 | 1 | 8 to 5 | 6 to 4 | 8 to 9 | 0 to 0 | 0.0 |
| chebyshev_sigmoid_d5 | 257.73 | 176.52 | x1.48 r | 3 | 8/8 | changed | 7 | 0 | 6 to 5 | 4 to 3 | 4 to 2 | 0 to 0 | 0.0 |
| chebyshev_sigmoid_d7 | 339.73 | 225.45 | x1.42 r | 3 | 6/8 | changed | 6 | 2 | 8 to 6 | 6 to 4 | 5 to 3 | 0 to 0 | 0.0 |
| chebyshev_silu_d7 | 304.29 | 212.7 | x1.43 r | 2 | 8/8 | changed | 6 | 0 | 7 to 4 | 5 to 3 | 5 to 4 | 0 to 0 | 0.0 |
| chebyshev_softplus_d7 | 301.57 | 211.46 | x1.42 r | 4 | 7/8 | changed | 6 | 1 | 7 to 4 | 5 to 3 | 5 to 4 | 0 to 0 | 0.0 |
| elementwise_add | 0.7 | 0.7 | x1.00 | 0 | 8/8 | same | 0 | 0 | 0 to 0 | 0 to 0 | 0 to 0 | 0 to 0 | 0.0 |
| elementwise_add_plain | 4.64 | 4.64 | x1.00 | 0 | 8/8 | same | 0 | 0 | 0 to 0 | 0 to 0 | 0 to 0 | 0 to 0 | 0.0 |
| elementwise_add_scalar | 0.45 | 0.45 | x1.00 | 0 | 7/8 | same | 1 | 0 | 0 to 0 | 0 to 0 | 0 to 0 | 0 to 0 | 0.0 |
| elementwise_mul | 64.59 | 64.59 | x1.00 | 0 | 8/8 | same | 0 | 0 | 1 to 1 | 1 to 1 | 0 to 0 | 0 to 0 | 0.0 |
| elementwise_mul_plain | 13.67 | 13.67 | x1.00 | 0 | 8/8 | same | 1 | 0 | 1 to 1 | 0 to 0 | 1 to 1 | 0 to 0 | 0.0 |
| elementwise_mul_scalar | 8.81 | 8.81 | x1.00 | 0 | 8/8 | same | 1 | 0 | 1 to 1 | 0 to 0 | 1 to 1 | 0 to 0 | 0.0 |
| embedding | 1288.55 | 1288.55 | x1.00 | 0 | 7/8 | same | 2 | 1 | 2 to 2 | 0 to 0 | 6 to 6 | 26 to 26 | 0.0 |
| extract | 58.96 | 58.96 | x1.00 | 0 | 7/8 | same | 0 | 1 | 1 to 1 | 0 to 0 | 1 to 1 | 1 to 1 | 0.0 |
| extract_linear | 59.15 | 59.15 | x1.00 | 0 | 7/8 | same | 1 | 1 | 1 to 1 | 0 to 0 | 1 to 1 | 1 to 1 | 0.0 |
| extract_sparse | 177.38 | 128.47 | x1.39 r | 1 | 8/8 | changed | 5 | 0 | 1 to 1 | 0 to 0 | 3 to 2 | 3 to 2 | 0.0 |
| quad | 64.31 | 64.31 | x1.00 | 0 | 8/8 | same | 2 | 0 | 1 to 1 | 1 to 1 | 0 to 0 | 0 to 0 | 0.0 |
| relu_composite | 643.2 | 643.2 | x1.00 | 0 | 5/8 | same | 3 | 3 | 10 to 10 | 16 to 16 | 12 to 12 | 0 to 0 | 0.0 |
| table_mult | 1157.88 | 1157.88 | x1.00 | 0 | 7/8 | same | 0 | 0 | 1 to 1 | 0 to 0 | 3 to 3 | 24 to 24 | 0.0 |

26 tasks; 12 measured 3% or more faster, 12 with a changed circuit; 186 correct attempts, 81 new circuits, 20 CHEHAB errors. Total cost $0.00. Models: claude-sonnet-5-5.
