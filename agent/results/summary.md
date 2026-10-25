# LLM optimization results (latest run per task)

fhe() time is the median evaluation time on the dense case. Levels, ciphertext-ciphertext multiplications (ct x ct), ciphertext-plaintext multiplications (ct x pt) and rotations are CHEHAB's counts for the baseline and for the best correct kernel.

| task | baseline ms | best ms | speedup | best attempt | correct | levels | ct x ct | ct x pt | rotations | cost USD |
|---|---:|---:|---:|---:|---:|---|---|---|---|---:|
| batchnorm1d | 17.74 | 17.74 | x1.00 | 0 | 8/8 | 1 to 1 | 0 to 0 | 1 to 1 | 0 to 0 | 0.0025 |
| batchnorm2d | 19.28 | 18.05 | x1.07 | 7 | 8/8 | 1 to 1 | 0 to 0 | 1 to 1 | 0 to 0 | 0.0021 |
| chebyshev_elu_d7 | 360.63 | 335.27 | x1.08 | 3 | 8/8 | 8 to 5 | 6 to 4 | 8 to 9 | 0 to 0 | 0.0134 |
| chebyshev_gelu_d7 | 304.31 | 231.47 | x1.31 | 4 | 8/8 | 7 to 5 | 5 to 3 | 5 to 5 | 0 to 0 | 0.0082 |
| chebyshev_hardshrink_d7 | 319.82 | 284.01 | x1.13 | 1 | 8/8 | 8 to 5 | 6 to 4 | 5 to 6 | 0 to 0 | 0.0112 |
| chebyshev_mish_d7 | 388.14 | 334.52 | x1.16 | 4 | 6/8 | 8 to 5 | 6 to 4 | 8 to 9 | 0 to 0 | 0.0221 |
| chebyshev_relu_d2 | 117.16 | 81.18 | x1.44 | 3 | 8/8 | 3 to 3 | 1 to 1 | 3 to 2 | 0 to 0 | 0.0087 |
| chebyshev_relu_d7 | 305.33 | 229.62 | x1.33 | 2 | 8/8 | 7 to 5 | 5 to 3 | 5 to 5 | 0 to 0 | 0.0083 |
| chebyshev_selu_d7 | 360.66 | 346.39 | x1.04 | 4 | 7/8 | 8 to 5 | 6 to 4 | 8 to 9 | 0 to 0 | 0.0112 |
| chebyshev_sigmoid_d5 | 257.56 | 227.54 | x1.13 | 5 | 8/8 | 6 to 5 | 4 to 3 | 4 to 4 | 0 to 0 | 0.0174 |
| chebyshev_sigmoid_d7 | 330.23 | 286.53 | x1.15 | 6 | 8/8 | 8 to 5 | 6 to 4 | 5 to 6 | 0 to 0 | 0.0112 |
| chebyshev_silu_d7 | 303.26 | 214.25 | x1.42 | 2 | 8/8 | 7 to 4 | 5 to 3 | 5 to 4 | 0 to 0 | 0.0087 |
| chebyshev_softplus_d7 | 303.82 | 218.66 | x1.39 | 4 | 7/8 | 7 to 4 | 5 to 3 | 5 to 4 | 0 to 0 | 0.0119 |
| elementwise_add | 0.73 | 0.72 | x1.01 | 8 | 8/8 | 0 to 0 | 0 to 0 | 0 to 0 | 0 to 0 | 0.0017 |
| elementwise_add_plain | 4.74 | 4.63 | x1.02 | 5 | 8/8 | 0 to 0 | 0 to 0 | 0 to 0 | 0 to 0 | 0.0019 |
| elementwise_add_scalar | 0.51 | 0.39 | x1.31 | 3 | 8/8 | 0 to 0 | 0 to 0 | 0 to 0 | 0 to 0 | 0.0016 |
| elementwise_mul | 65.53 | 64.83 | x1.01 | 1 | 8/8 | 1 to 1 | 1 to 1 | 0 to 0 | 0 to 0 | 0.0023 |
| elementwise_mul_plain | 13.43 | 13.01 | x1.03 | 4 | 8/8 | 1 to 1 | 0 to 0 | 1 to 1 | 0 to 0 | 0.0019 |
| elementwise_mul_scalar | 9.19 | 8.88 | x1.03 | 7 | 8/8 | 1 to 1 | 0 to 0 | 1 to 1 | 0 to 0 | 0.0017 |
| embedding | 1324.11 | 1285.32 | x1.03 | 1 | 8/8 | 2 to 2 | 0 to 0 | 6 to 6 | 26 to 26 | 0.0147 |
| extract | 59.59 | 59.24 | x1.01 | 7 | 8/8 | 1 to 1 | 0 to 0 | 1 to 1 | 1 to 1 | 0.0036 |
| extract_linear | 59.11 | 59.11 | x1.00 | 0 | 7/8 | 1 to 1 | 0 to 0 | 1 to 1 | 1 to 1 | 0.0028 |
| extract_sparse | 177.95 | 128.1 | x1.39 | 1 | 8/8 | 1 to 1 | 0 to 0 | 3 to 2 | 3 to 2 | 0.0053 |
| quad | 64.7 | 63.71 | x1.02 | 6 | 8/8 | 1 to 1 | 1 to 0 | 0 to 0 | 0 to 0 | 0.0021 |
| relu_composite | 672.92 | 639.17 | x1.05 | 1 | 7/8 | 10 to 10 | 16 to 16 | 12 to 12 | 0 to 0 | 0.0173 |
| table_mult | 1266.12 | 1172.19 | x1.08 | 7 | 8/8 | 1 to 1 | 0 to 0 | 3 to 3 | 24 to 24 | 0.019 |

26 tasks; 19 improved by 3% or more. Total cost $0.21. Models: claude-haiku-5-5.
