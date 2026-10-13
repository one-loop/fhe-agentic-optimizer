# LLM optimization results (latest run per task)

fhe() time is the median evaluation time on the dense case. Levels, ciphertext-ciphertext multiplications (ct x ct), ciphertext-plaintext multiplications (ct x pt) and rotations are CHEHAB's counts for the baseline and for the best correct kernel.

| task | baseline ms | best ms | speedup | best attempt | correct | levels | ct x ct | ct x pt | rotations | cost USD |
|---|---:|---:|---:|---:|---:|---|---|---|---|---:|
| batchnorm1d | 9.59 | 9.59 | x1.00 | 0 | 8/8 | 1 to 1 | 0 to 0 | 1 to 1 | 0 to 0 | 0.0026 |
| batchnorm2d | 9.84 | 9.69 | x1.02 | 6 | 8/8 | 1 to 1 | 0 to 0 | 1 to 1 | 0 to 0 | 0.0023 |
| chebyshev_elu_d7 | 181.82 | 175.04 | x1.04 | 5 | 6/8 | 8 to 5 | 6 to 4 | 8 to 9 | 0 to 0 | 0.0159 |
| chebyshev_gelu_d7 | 152.48 | 115.88 | x1.32 | 3 | 8/8 | 7 to 5 | 5 to 3 | 5 to 5 | 0 to 0 | 0.0074 |
| chebyshev_hardshrink_d7 | 161.19 | 136.17 | x1.18 | 2 | 8/8 | 8 to 6 | 6 to 4 | 5 to 5 | 0 to 0 | 0.0131 |
| chebyshev_mish_d7 | 181.92 | 168.22 | x1.08 | 3 | 6/8 | 8 to 5 | 6 to 4 | 8 to 9 | 0 to 0 | 0.0208 |
| chebyshev_relu_d2 | 53.43 | 41.32 | x1.29 | 1 | 8/8 | 3 to 3 | 1 to 1 | 3 to 2 | 0 to 0 | 0.0077 |
| chebyshev_relu_d7 | 152.36 | 133.96 | x1.14 | 2 | 8/8 | 7 to 5 | 5 to 3 | 5 to 5 | 0 to 0 | 0.0127 |
| chebyshev_selu_d7 | 181.9 | 167.67 | x1.08 | 3 | 6/8 | 8 to 5 | 6 to 4 | 8 to 9 | 0 to 0 | 0.0136 |
| chebyshev_sigmoid_d5 | 129.13 | 107.66 | x1.20 | 2 | 8/8 | 6 to 5 | 4 to 3 | 4 to 3 | 0 to 0 | 0.013 |
| chebyshev_sigmoid_d7 | 161.56 | 149.4 | x1.08 | 5 | 7/8 | 8 to 5 | 6 to 4 | 5 to 7 | 0 to 0 | 0.015 |
| chebyshev_silu_d7 | 152.35 | 110.58 | x1.38 | 2 | 8/8 | 7 to 4 | 5 to 3 | 5 to 4 | 0 to 0 | 0.0094 |
| chebyshev_softplus_d7 | 152.28 | 115.69 | x1.32 | 4 | 8/8 | 7 to 5 | 5 to 3 | 5 to 5 | 0 to 0 | 0.008 |
| elementwise_add | 0.46 | 0.45 | x1.01 | 8 | 8/8 | 0 to 0 | 0 to 0 | 0 to 0 | 0 to 0 | 0.0018 |
| elementwise_add_plain | 2.69 | 2.49 | x1.08 | 6 | 8/8 | 0 to 0 | 0 to 0 | 0 to 0 | 0 to 0 | 0.0016 |
| elementwise_add_scalar | 0.26 | 0.25 | x1.05 | 3 | 8/8 | 0 to 0 | 0 to 0 | 0 to 0 | 0 to 0 | 0.0018 |
| elementwise_mul | 31.49 | 31.49 | x1.00 | 0 | 8/8 | 1 to 1 | 1 to 1 | 0 to 0 | 0 to 0 | 0.002 |
| elementwise_mul_plain | 7.03 | 7.03 | x1.00 | 0 | 8/8 | 1 to 1 | 0 to 0 | 1 to 1 | 0 to 0 | 0.0017 |
| elementwise_mul_scalar | 4.59 | 4.58 | x1.00 | 4 | 8/8 | 1 to 1 | 0 to 0 | 1 to 1 | 0 to 0 | 0.0018 |
| embedding | 616.8 | 615.82 | x1.00 | 3 | 8/8 | 2 to 2 | 0 to 0 | 6 to 6 | 26 to 26 | 0.0175 |
| extract | 29.17 | 29.09 | x1.00 | 6 | 8/8 | 1 to 1 | 0 to 0 | 1 to 1 | 1 to 1 | 0.0032 |
| extract_linear | 29.13 | 29.1 | x1.00 | 7 | 8/8 | 1 to 1 | 0 to 0 | 1 to 1 | 1 to 1 | 0.003 |
| extract_sparse | 87.72 | 63.53 | x1.38 | 1 | 8/8 | 1 to 1 | 0 to 0 | 3 to 2 | 3 to 2 | 0.0059 |
| quad | 31.41 | 30.93 | x1.01 | 6 | 8/8 | 1 to 1 | 1 to 0 | 0 to 0 | 0 to 0 | 0.002 |
| relu_composite | 318.5 | 317.94 | x1.00 | 3 | 8/8 | 10 to 10 | 16 to 16 | 12 to 12 | 0 to 0 | 0.0134 |
| table_mult | 560.02 | 559.87 | x1.00 | 8 | 8/8 | 1 to 1 | 0 to 0 | 3 to 3 | 24 to 24 | 0.0099 |

26 tasks; 14 improved by 3% or more. Total cost $0.21. Models: claude-haiku-5-5.
