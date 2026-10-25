# LLM optimization results (latest run per task)

fhe() time is the median evaluation time on the dense case. Levels, ciphertext-ciphertext multiplications (ct x ct), ciphertext-plaintext multiplications (ct x pt) and rotations are CHEHAB's counts for the baseline and for the best correct kernel.

Speedup is from baseline and best timed in alternating batches when the run remeasured it (marked r); older runs only have one median each. 'circuit' says whether the best kernel's circuit differs from the baseline's; 'new' counts distinct new correct circuits in the run.

| task | baseline ms | best ms | speedup | best attempt | correct | circuit | new | CHEHAB errors | levels | ct x ct | ct x pt | rotations | cost USD |
|---|---:|---:|---:|---:|---:|---|---:|---:|---|---|---|---|---:|
| batchnorm1d | 17.95 | 17.95 | x1.00 | 0 | 7/8 | same | 0 | 1 | 1 to 1 | 0 to 0 | 1 to 1 | 0 to 0 | 0.0 |
| batchnorm2d | 18.01 | 18.01 | x1.00 | 0 | 7/8 | same | 1 | 0 | 1 to 1 | 0 to 0 | 1 to 1 | 0 to 0 | 0.0 |
| chebyshev_elu_d7 | 358.28 | 254.67 | x1.42 r | 4 | 6/8 | changed | 6 | 2 | 8 to 8 | 6 to 6 | 8 to 2 | 0 to 0 | 0.0 |
| chebyshev_gelu_d7 | 303.56 | 209.96 | x1.44 r | 2 | 5/8 | changed | 3 | 3 | 7 to 5 | 5 to 3 | 5 to 4 | 0 to 0 | 0.0 |
| chebyshev_hardshrink_d7 | 318.57 | 264.24 | x1.21 r | 1 | 7/8 | changed | 6 | 1 | 8 to 5 | 6 to 4 | 5 to 5 | 0 to 0 | 0.0 |
| chebyshev_mish_d7 | 357.94 | 252.46 | x1.42 r | 3 | 6/8 | changed | 6 | 2 | 8 to 8 | 6 to 6 | 8 to 2 | 0 to 0 | 0.0 |
| chebyshev_relu_d2 | 103.62 | 82.33 | x1.28 r | 1 | 8/8 | changed | 3 | 0 | 3 to 3 | 1 to 1 | 3 to 2 | 0 to 0 | 0.0 |
| chebyshev_relu_d7 | 300.04 | 212.85 | x1.41 r | 3 | 7/8 | changed | 6 | 1 | 7 to 4 | 5 to 3 | 5 to 4 | 0 to 0 | 0.0 |
| chebyshev_selu_d7 | 358.24 | 301.94 | x1.19 r | 2 | 6/8 | changed | 5 | 2 | 8 to 6 | 6 to 4 | 8 to 7 | 0 to 0 | 0.0 |
| chebyshev_sigmoid_d5 | 255.72 | 211.44 | x1.21 r | 1 | 8/8 | changed | 8 | 0 | 6 to 4 | 4 to 3 | 4 to 4 | 0 to 0 | 0.0 |
| chebyshev_sigmoid_d7 | 319.06 | 253.45 | x1.26 r | 4 | 5/8 | changed | 5 | 3 | 8 to 6 | 6 to 4 | 5 to 4 | 0 to 0 | 0.0 |
| chebyshev_silu_d7 | 300.04 | 205.16 | x1.46 r | 3 | 8/8 | changed | 7 | 0 | 7 to 5 | 5 to 3 | 5 to 4 | 0 to 0 | 0.0 |
| chebyshev_softplus_d7 | 299.96 | 205.17 | x1.47 r | 2 | 7/8 | changed | 5 | 1 | 7 to 5 | 5 to 3 | 5 to 4 | 0 to 0 | 0.0 |
| elementwise_add | 0.77 | 0.77 | x1.00 | 0 | 8/8 | same | 0 | 0 | 0 to 0 | 0 to 0 | 0 to 0 | 0 to 0 | 0.0 |
| elementwise_add_plain | 4.63 | 4.63 | x1.00 | 0 | 8/8 | same | 0 | 0 | 0 to 0 | 0 to 0 | 0 to 0 | 0 to 0 | 0.0 |
| elementwise_add_scalar | 0.43 | 0.43 | x1.00 | 0 | 8/8 | same | 1 | 0 | 0 to 0 | 0 to 0 | 0 to 0 | 0 to 0 | 0.0 |
| elementwise_mul | 64.3 | 64.3 | x1.00 | 0 | 7/8 | same | 0 | 0 | 1 to 1 | 1 to 1 | 0 to 0 | 0 to 0 | 0.0 |
| elementwise_mul_plain | 12.92 | 12.92 | x1.00 | 0 | 8/8 | same | 0 | 0 | 1 to 1 | 0 to 0 | 1 to 1 | 0 to 0 | 0.0 |
| elementwise_mul_scalar | 8.7 | 8.7 | x1.00 | 0 | 8/8 | same | 0 | 0 | 1 to 1 | 0 to 0 | 1 to 1 | 0 to 0 | 0.0 |
| embedding | 1285.55 | 1285.55 | x1.00 | 0 | 7/8 | same | 1 | 1 | 2 to 2 | 0 to 0 | 6 to 6 | 26 to 26 | 0.0 |
| extract | 63.48 | 63.48 | x1.00 | 0 | 7/8 | same | 0 | 1 | 1 to 1 | 0 to 0 | 1 to 1 | 1 to 1 | 0.0 |
| extract_linear | 59.04 | 59.04 | x1.00 | 0 | 7/8 | same | 0 | 1 | 1 to 1 | 0 to 0 | 1 to 1 | 1 to 1 | 0.0 |
| extract_sparse | 178.98 | 129.28 | x1.41 r | 5 | 8/8 | changed | 3 | 0 | 1 to 1 | 0 to 0 | 3 to 2 | 3 to 2 | 0.0 |
| quad | 64.23 | 64.23 | x1.00 | 0 | 8/8 | same | 2 | 0 | 1 to 1 | 1 to 1 | 0 to 0 | 0 to 0 | 0.0 |
| relu_composite | 655.61 | 655.61 | x1.00 | 0 | 7/8 | same | 5 | 1 | 10 to 10 | 16 to 16 | 12 to 12 | 0 to 0 | 0.0 |
| table_mult | 1165.08 | 1165.08 | x1.00 | 0 | 7/8 | same | 0 | 1 | 1 to 1 | 0 to 0 | 3 to 3 | 24 to 24 | 0.0 |

26 tasks; 12 measured 3% or more faster, 12 with a changed circuit; 185 correct attempts, 73 new circuits, 21 CHEHAB errors. Total cost $0.00. Models: claude-sonnet-5-5.
