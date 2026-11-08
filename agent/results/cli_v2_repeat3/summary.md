# LLM optimization results (latest run per task)

fhe() time is the median evaluation time on the dense case. Levels, ciphertext-ciphertext multiplications (ct x ct), ciphertext-plaintext multiplications (ct x pt) and rotations are CHEHAB's counts for the baseline and for the best correct kernel.

Speedup is from baseline and best timed in alternating batches when the run remeasured it (marked r); older runs only have one median each. 'circuit' says whether the best kernel's circuit differs from the baseline's; 'new' counts distinct new correct circuits in the run.

| task | baseline ms | best ms | speedup | best attempt | correct | circuit | new | CHEHAB errors | levels | ct x ct | ct x pt | rotations | cost USD |
|---|---:|---:|---:|---:|---:|---|---:|---:|---|---|---|---|---:|
| batchnorm1d | 17.94 | 17.94 | x1.00 | 0 | 8/8 | same | 0 | 0 | 1 to 1 | 0 to 0 | 1 to 1 | 0 to 0 | 0.0 |
| batchnorm2d | 17.91 | 17.91 | x1.00 | 0 | 7/8 | same | 1 | 1 | 1 to 1 | 0 to 0 | 1 to 1 | 0 to 0 | 0.0 |
| chebyshev_elu_d7 | 357.89 | 331.26 | x1.08 r | 3 | 6/8 | changed | 6 | 2 | 8 to 5 | 6 to 4 | 8 to 9 | 0 to 0 | 0.0 |
| chebyshev_gelu_d7 | 300.48 | 219.53 | x1.41 r | 4 | 8/8 | changed | 6 | 0 | 7 to 4 | 5 to 3 | 5 to 4 | 0 to 0 | 0.0 |
| chebyshev_hardshrink_d7 | 320.33 | 273.65 | x1.18 r | 2 | 7/8 | changed | 6 | 1 | 8 to 6 | 6 to 4 | 5 to 5 | 0 to 0 | 0.0 |
| chebyshev_mish_d7 | 383.98 | 338.19 | x1.09 r | 5 | 6/8 | changed | 6 | 2 | 8 to 5 | 6 to 5 | 8 to 6 | 0 to 0 | 0.0 |
| chebyshev_relu_d2 | 104.12 | 81.17 | x1.28 r | 1 | 8/8 | changed | 5 | 0 | 3 to 3 | 1 to 1 | 3 to 2 | 0 to 0 | 0.0 |
| chebyshev_relu_d7 | 300.57 | 196.5 | x1.55 r | 3 | 7/8 | changed | 6 | 0 | 7 to 5 | 5 to 3 | 5 to 3 | 0 to 0 | 0.0 |
| chebyshev_selu_d7 | 358.66 | 251.57 | x1.42 r | 4 | 4/8 | changed | 4 | 3 | 8 to 8 | 6 to 6 | 8 to 2 | 0 to 0 | 0.0 |
| chebyshev_sigmoid_d5 | 256.5 | 168.2 | x1.53 r | 4 | 8/8 | changed | 7 | 0 | 6 to 5 | 4 to 3 | 4 to 2 | 0 to 0 | 0.0 |
| chebyshev_sigmoid_d7 | 319.79 | 297.49 | x1.09 r | 4 | 5/8 | changed | 5 | 2 | 8 to 6 | 6 to 4 | 5 to 5 | 0 to 0 | 0.0 |
| chebyshev_silu_d7 | 335.25 | 209.77 | x1.43 r | 7 | 5/8 | changed | 4 | 2 | 7 to 5 | 5 to 3 | 5 to 4 | 0 to 0 | 0.0 |
| chebyshev_softplus_d7 | 301.5 | 219.68 | x1.42 r | 5 | 8/8 | changed | 8 | 0 | 7 to 4 | 5 to 3 | 5 to 4 | 0 to 0 | 0.0 |
| elementwise_add | 0.79 | 0.79 | x1.00 | 0 | 8/8 | same | 0 | 0 | 0 to 0 | 0 to 0 | 0 to 0 | 0 to 0 | 0.0 |
| elementwise_add_plain | 4.64 | 4.64 | x1.00 | 0 | 8/8 | same | 1 | 0 | 0 to 0 | 0 to 0 | 0 to 0 | 0 to 0 | 0.0 |
| elementwise_add_scalar | 0.47 | 0.47 | x1.00 | 0 | 8/8 | same | 0 | 0 | 0 to 0 | 0 to 0 | 0 to 0 | 0 to 0 | 0.0 |
| elementwise_mul | 64.65 | 64.65 | x1.00 | 0 | 8/8 | same | 0 | 0 | 1 to 1 | 1 to 1 | 0 to 0 | 0 to 0 | 0.0 |
| elementwise_mul_plain | 13.05 | 13.05 | x1.00 | 0 | 8/8 | same | 2 | 0 | 1 to 1 | 0 to 0 | 1 to 1 | 0 to 0 | 0.0 |
| elementwise_mul_scalar | 9.1 | 9.1 | x1.00 | 0 | 8/8 | same | 0 | 0 | 1 to 1 | 0 to 0 | 1 to 1 | 0 to 0 | 0.0 |
| embedding | 1279.22 | 1279.22 | x1.00 | 0 | 8/8 | same | 5 | 0 | 2 to 2 | 0 to 0 | 6 to 6 | 26 to 26 | 0.0 |
| extract | 62.61 | 62.61 | x1.00 | 0 | 7/8 | same | 0 | 1 | 1 to 1 | 0 to 0 | 1 to 1 | 1 to 1 | 0.0 |
| extract_linear | 60.2 | 60.2 | x1.00 | 0 | 8/8 | same | 0 | 0 | 1 to 1 | 0 to 0 | 1 to 1 | 1 to 1 | 0.0 |
| extract_sparse | 220.39 | 131.97 | x1.43 r | 1 | 8/8 | changed | 4 | 0 | 1 to 1 | 0 to 0 | 3 to 2 | 3 to 2 | 0.0 |
| quad | 65.95 | 65.95 | x1.00 | 0 | 8/8 | same | 2 | 0 | 1 to 1 | 1 to 1 | 0 to 0 | 0 to 0 | 0.0 |
| relu_composite | 654.15 | 654.15 | x1.00 | 0 | 6/8 | same | 4 | 2 | 10 to 10 | 16 to 16 | 12 to 12 | 0 to 0 | 0.0 |
| table_mult | 1294.0 | 1294.0 | x1.00 | 0 | 8/8 | same | 0 | 0 | 1 to 1 | 0 to 0 | 3 to 3 | 24 to 24 | 0.0 |

26 tasks; 12 measured 3% or more faster, 12 with a changed circuit; 188 correct attempts, 82 new circuits, 16 CHEHAB errors. Total cost $0.00. Models: claude-sonnet-5-5.
