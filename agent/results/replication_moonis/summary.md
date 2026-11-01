# LLM optimization results (latest run per task)

fhe() time is the median evaluation time on the dense case. Levels, ciphertext-ciphertext multiplications (ct x ct), ciphertext-plaintext multiplications (ct x pt) and rotations are CHEHAB's counts for the baseline and for the best correct kernel.

Speedup is from baseline and best timed in alternating batches when the run remeasured it (marked r); older runs only have one median each. 'circuit' says whether the best kernel's circuit differs from the baseline's; 'new' counts distinct new correct circuits in the run.

| task | baseline ms | best ms | speedup | best attempt | correct | circuit | new | CHEHAB errors | levels | ct x ct | ct x pt | rotations | cost USD |
|---|---:|---:|---:|---:|---:|---|---:|---:|---|---|---|---|---:|
| batchnorm1d | 9.68 | 9.68 | x1.00 | 0 | 8/8 | same | 1 | 0 | 1 to 1 | 0 to 0 | 1 to 1 | 0 to 0 | 0.0 |
| batchnorm2d | 9.84 | 9.84 | x1.00 | 0 | 8/8 | same | 0 | 0 | 1 to 1 | 0 to 0 | 1 to 1 | 0 to 0 | 0.0 |
| chebyshev_elu_d7 | 183.24 | 164.14 | x1.10 r | 5 | 2/8 | changed | 2 | 3 | 8 to 6 | 6 to 4 | 8 to 11 | 0 to 0 | 0.0 |
| chebyshev_gelu_d7 | 152.72 | 97.94 | x1.55 r | 4 | 7/8 | changed | 6 | 1 | 7 to 5 | 5 to 3 | 5 to 3 | 0 to 0 | 0.0 |
| chebyshev_hardshrink_d7 | 161.53 | 122.95 | x1.31 r | 2 | 7/8 | changed | 7 | 1 | 8 to 6 | 6 to 4 | 5 to 4 | 0 to 0 | 0.0 |
| chebyshev_mish_d7 | 182.3 | 161.86 | x1.13 r | 8 | 3/8 | changed | 3 | 4 | 8 to 6 | 6 to 4 | 8 to 7 | 0 to 0 | 0.0 |
| chebyshev_relu_d2 | 53.79 | 41.54 | x1.29 r | 1 | 8/8 | changed | 4 | 0 | 3 to 3 | 1 to 1 | 3 to 2 | 0 to 0 | 0.0 |
| chebyshev_relu_d7 | 152.34 | 106.17 | x1.44 r | 2 | 7/8 | changed | 6 | 1 | 7 to 5 | 5 to 3 | 5 to 4 | 0 to 0 | 0.0 |
| chebyshev_selu_d7 | 182.07 | 143.49 | x1.27 r | 5 | 7/8 | changed | 5 | 1 | 8 to 5 | 6 to 4 | 8 to 5 | 0 to 0 | 0.0 |
| chebyshev_sigmoid_d5 | 130.11 | 110.46 | x1.17 r | 1 | 8/8 | changed | 8 | 0 | 6 to 5 | 4 to 3 | 4 to 6 | 0 to 0 | 0.0 |
| chebyshev_sigmoid_d7 | 161.92 | 101.02 | x1.60 r | 7 | 6/8 | changed | 5 | 1 | 8 to 6 | 6 to 4 | 5 to 2 | 0 to 0 | 0.0 |
| chebyshev_silu_d7 | 153.45 | 105.86 | x1.44 r | 4 | 7/8 | changed | 7 | 1 | 7 to 5 | 5 to 3 | 5 to 4 | 0 to 0 | 0.0 |
| chebyshev_softplus_d7 | 153.2 | 97.66 | x1.56 r | 5 | 7/8 | changed | 5 | 1 | 7 to 5 | 5 to 3 | 5 to 3 | 0 to 0 | 0.0 |
| elementwise_add | 0.46 | 0.46 | x1.00 | 0 | 8/8 | same | 0 | 0 | 0 to 0 | 0 to 0 | 0 to 0 | 0 to 0 | 0.0 |
| elementwise_add_plain | 2.7 | 2.7 | x1.00 | 0 | 8/8 | same | 1 | 0 | 0 to 0 | 0 to 0 | 0 to 0 | 0 to 0 | 0.0 |
| elementwise_add_scalar | 0.27 | 0.27 | x1.00 | 0 | 8/8 | same | 0 | 0 | 0 to 0 | 0 to 0 | 0 to 0 | 0 to 0 | 0.0 |
| elementwise_mul | 31.71 | 31.71 | x1.00 | 0 | 8/8 | same | 0 | 0 | 1 to 1 | 1 to 1 | 0 to 0 | 0 to 0 | 0.0 |
| elementwise_mul_plain | 7.16 | 7.16 | x1.00 | 0 | 8/8 | same | 1 | 0 | 1 to 1 | 0 to 0 | 1 to 1 | 0 to 0 | 0.0 |
| elementwise_mul_scalar | 4.6 | 4.6 | x1.00 | 0 | 8/8 | same | 1 | 0 | 1 to 1 | 0 to 0 | 1 to 1 | 0 to 0 | 0.0 |
| embedding | 619.26 | 619.26 | x1.00 | 0 | 8/8 | same | 2 | 0 | 2 to 2 | 0 to 0 | 6 to 6 | 26 to 26 | 0.0 |
| extract | 29.18 | 29.18 | x1.00 | 0 | 7/8 | same | 0 | 1 | 1 to 1 | 0 to 0 | 1 to 1 | 1 to 1 | 0.0 |
| extract_linear | 29.2 | 29.2 | x1.00 | 0 | 7/8 | same | 0 | 1 | 1 to 1 | 0 to 0 | 1 to 1 | 1 to 1 | 0.0 |
| extract_sparse | 87.88 | 63.55 | x1.39 r | 1 | 8/8 | changed | 3 | 0 | 1 to 1 | 0 to 0 | 3 to 2 | 3 to 2 | 0.0 |
| quad | 32.11 | 32.11 | x1.00 | 0 | 8/8 | same | 4 | 0 | 1 to 1 | 1 to 1 | 0 to 0 | 0 to 0 | 0.0 |
| relu_composite | 319.24 | 319.24 | x1.00 | 0 | 6/8 | same | 5 | 2 | 10 to 10 | 16 to 16 | 12 to 12 | 0 to 0 | 0.0 |
| table_mult | 573.38 | 573.38 | x1.00 | 0 | 6/8 | same | 2 | 1 | 1 to 1 | 0 to 0 | 3 to 3 | 24 to 24 | 0.0 |

26 tasks; 12 measured 3% or more faster, 12 with a changed circuit; 183 correct attempts, 78 new circuits, 19 CHEHAB errors. Total cost $0.00. Models: claude-sonnet-5-5.
