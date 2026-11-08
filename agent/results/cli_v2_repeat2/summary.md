# LLM optimization results (latest run per task)

fhe() time is the median evaluation time on the dense case. Levels, ciphertext-ciphertext multiplications (ct x ct), ciphertext-plaintext multiplications (ct x pt) and rotations are CHEHAB's counts for the baseline and for the best correct kernel.

Speedup is from baseline and best timed in alternating batches when the run remeasured it (marked r); older runs only have one median each. 'circuit' says whether the best kernel's circuit differs from the baseline's; 'new' counts distinct new correct circuits in the run.

| task | baseline ms | best ms | speedup | best attempt | correct | circuit | new | CHEHAB errors | levels | ct x ct | ct x pt | rotations | cost USD |
|---|---:|---:|---:|---:|---:|---|---:|---:|---|---|---|---|---:|
| batchnorm1d | 18.03 | 18.03 | x1.00 | 0 | 7/8 | same | 1 | 0 | 1 to 1 | 0 to 0 | 1 to 1 | 0 to 0 | 0.0 |
| batchnorm2d | 17.96 | 17.96 | x1.00 | 0 | 8/8 | same | 1 | 0 | 1 to 1 | 0 to 0 | 1 to 1 | 0 to 0 | 0.0 |
| chebyshev_elu_d7 | 380.88 | 251.73 | x1.42 r | 4 | 7/8 | changed | 6 | 1 | 8 to 8 | 6 to 6 | 8 to 2 | 0 to 0 | 0.0 |
| chebyshev_gelu_d7 | 300.37 | 189.5 | x1.59 r | 5 | 8/8 | changed | 8 | 0 | 7 to 5 | 5 to 3 | 5 to 3 | 0 to 0 | 0.0 |
| chebyshev_hardshrink_d7 | 318.32 | 268.18 | x1.19 r | 3 | 5/8 | changed | 5 | 2 | 8 to 6 | 6 to 4 | 5 to 5 | 0 to 0 | 0.0 |
| chebyshev_mish_d7 | 358.66 | 186.56 | x1.92 r | 7 | 6/8 | changed | 4 | 2 | 8 to 10 | 6 to 6 | 8 to 4 | 0 to 0 | 0.0 |
| chebyshev_relu_d2 | 104.34 | 81.44 | x1.28 r | 1 | 8/8 | changed | 3 | 0 | 3 to 3 | 1 to 1 | 3 to 2 | 0 to 0 | 0.0 |
| chebyshev_relu_d7 | 301.79 | 213.56 | x1.42 r | 2 | 7/8 | changed | 3 | 1 | 7 to 4 | 5 to 3 | 5 to 4 | 0 to 0 | 0.0 |
| chebyshev_selu_d7 | 358.09 | 330.21 | x1.08 r | 5 | 5/8 | changed | 4 | 3 | 8 to 5 | 6 to 4 | 8 to 9 | 0 to 0 | 0.0 |
| chebyshev_sigmoid_d5 | 256.75 | 171.41 | x1.50 r | 4 | 8/8 | changed | 5 | 0 | 6 to 5 | 4 to 3 | 4 to 2 | 0 to 0 | 0.0 |
| chebyshev_sigmoid_d7 | 319.56 | 244.32 | x1.31 r | 7 | 5/8 | changed | 5 | 3 | 8 to 6 | 6 to 4 | 5 to 4 | 0 to 0 | 0.0 |
| chebyshev_silu_d7 | 300.65 | 211.95 | x1.41 r | 3 | 7/8 | changed | 5 | 1 | 7 to 4 | 5 to 3 | 5 to 4 | 0 to 0 | 0.0 |
| chebyshev_softplus_d7 | 301.13 | 210.38 | x1.42 r | 2 | 8/8 | changed | 7 | 0 | 7 to 4 | 5 to 3 | 5 to 4 | 0 to 0 | 0.0 |
| elementwise_add | 0.75 | 0.75 | x1.00 | 0 | 8/8 | same | 0 | 0 | 0 to 0 | 0 to 0 | 0 to 0 | 0 to 0 | 0.0 |
| elementwise_add_plain | 4.81 | 4.81 | x1.00 | 0 | 8/8 | same | 1 | 0 | 0 to 0 | 0 to 0 | 0 to 0 | 0 to 0 | 0.0 |
| elementwise_add_scalar | 0.46 | 0.46 | x1.00 | 0 | 8/8 | same | 0 | 0 | 0 to 0 | 0 to 0 | 0 to 0 | 0 to 0 | 0.0 |
| elementwise_mul | 64.5 | 64.5 | x1.00 | 0 | 8/8 | same | 0 | 0 | 1 to 1 | 1 to 1 | 0 to 0 | 0 to 0 | 0.0 |
| elementwise_mul_plain | 13.73 | 13.73 | x1.00 | 0 | 8/8 | same | 0 | 0 | 1 to 1 | 0 to 0 | 1 to 1 | 0 to 0 | 0.0 |
| elementwise_mul_scalar | 8.76 | 8.76 | x1.00 | 0 | 8/8 | same | 0 | 0 | 1 to 1 | 0 to 0 | 1 to 1 | 0 to 0 | 0.0 |
| embedding | 1269.88 | 1269.88 | x1.00 | 0 | 8/8 | same | 5 | 0 | 2 to 2 | 0 to 0 | 6 to 6 | 26 to 26 | 0.0 |
| extract | 58.89 | 58.89 | x1.00 | 0 | 7/8 | same | 0 | 1 | 1 to 1 | 0 to 0 | 1 to 1 | 1 to 1 | 0.0 |
| extract_linear | 59.27 | 59.27 | x1.00 | 0 | 7/8 | same | 1 | 1 | 1 to 1 | 0 to 0 | 1 to 1 | 1 to 1 | 0.0 |
| extract_sparse | 177.8 | 128.63 | x1.39 r | 1 | 8/8 | changed | 4 | 0 | 1 to 1 | 0 to 0 | 3 to 2 | 3 to 2 | 0.0 |
| quad | 64.47 | 64.47 | x1.00 | 0 | 8/8 | same | 1 | 0 | 1 to 1 | 1 to 1 | 0 to 0 | 0 to 0 | 0.0 |
| relu_composite | 638.16 | 638.16 | x1.00 | 0 | 6/8 | same | 5 | 2 | 10 to 10 | 16 to 16 | 12 to 12 | 0 to 0 | 0.0 |
| table_mult | 1175.1 | 1175.1 | x1.00 | 0 | 7/8 | same | 0 | 1 | 1 to 1 | 0 to 0 | 3 to 3 | 24 to 24 | 0.0 |

26 tasks; 12 measured 3% or more faster, 12 with a changed circuit; 188 correct attempts, 74 new circuits, 18 CHEHAB errors. Total cost $0.00. Models: claude-sonnet-5-5.
