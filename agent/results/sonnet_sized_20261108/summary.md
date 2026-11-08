# LLM optimization results (latest run per task)

fhe() time is the median evaluation time on the dense case. Levels, ciphertext-ciphertext multiplications (ct x ct), ciphertext-plaintext multiplications (ct x pt) and rotations are CHEHAB's counts for the baseline and for the best correct kernel.

Speedup is from baseline and best timed in alternating batches when the run remeasured it (marked r); older runs only have one median each. 'circuit' says whether the best kernel's circuit differs from the baseline's; 'new' counts distinct new correct circuits in the run.

| task | baseline ms | best ms | speedup | best attempt | correct | circuit | new | CHEHAB errors | levels | ct x ct | ct x pt | rotations | cost USD |
|---|---:|---:|---:|---:|---:|---|---:|---:|---|---|---|---|---:|
| batchnorm1d | 4.15 | 4.15 | x1.00 | 0 | 8/8 | same | 1 | 0 | 1 to 1 | 0 to 0 | 1 to 1 | 0 to 0 | 0.4954 |
| batchnorm2d | 3.77 | 3.77 | x1.00 | 0 | 7/8 | same | 1 | 1 | 1 to 1 | 0 to 0 | 1 to 1 | 0 to 0 | 0.389 |
| chebyshev_elu_d7 | 241.78 | 166.02 | x1.47 r | 5 | 5/8 | changed | 4 | 3 | 8 to 6 | 6 to 4 | 8 to 10 | 0 to 0 | 1.2935 |
| chebyshev_gelu_d7 | 166.62 | 56.68 | x2.86 r | 7 | 6/8 | changed | 5 | 2 | 7 to 4 | 5 to 3 | 5 to 4 | 0 to 0 | 0.9159 |
| chebyshev_hardshrink_d7 | 215.54 | 94.14 | x2.33 r | 6 | 6/8 | changed | 5 | 2 | 8 to 5 | 6 to 4 | 5 to 5 | 0 to 0 | 1.0308 |
| chebyshev_mish_d7 | 242.94 | 122.56 | x2.00 r | 1 | 7/8 | changed | 7 | 0 | 8 to 5 | 6 to 4 | 8 to 9 | 0 to 0 | 1.5166 |
| chebyshev_relu_d2 | 24.07 | 17.04 | x1.41 r | 1 | 8/8 | changed | 4 | 0 | 3 to 3 | 1 to 1 | 3 to 2 | 0 to 0 | 0.824 |
| chebyshev_relu_d7 | 168.65 | 56.43 | x2.97 r | 2 | 7/8 | changed | 4 | 1 | 7 to 4 | 5 to 3 | 5 to 4 | 0 to 0 | 0.794 |
| chebyshev_selu_d7 | 243.78 | 140.26 | x1.73 r | 4 | 3/8 | changed | 3 | 5 | 8 to 5 | 6 to 5 | 8 to 10 | 0 to 0 | 1.4171 |
| chebyshev_sigmoid_d5 | 113.4 | 57.12 | x1.99 r | 1 | 8/8 | changed | 6 | 0 | 6 to 4 | 4 to 3 | 4 to 4 | 0 to 0 | 1.371 |
| chebyshev_sigmoid_d7 | 217.24 | 86.56 | x2.50 r | 6 | 6/8 | changed | 6 | 2 | 8 to 6 | 6 to 4 | 5 to 2 | 0 to 0 | 1.0857 |
| chebyshev_silu_d7 | 167.76 | 55.6 | x3.05 r | 6 | 6/8 | changed | 5 | 1 | 7 to 4 | 5 to 3 | 5 to 4 | 0 to 0 | 0.8704 |
| chebyshev_softplus_d7 | 182.47 | 57.72 | x2.97 r | 1 | 8/8 | changed | 6 | 0 | 7 to 4 | 5 to 3 | 5 to 4 | 0 to 0 | 1.0799 |
| elementwise_add | 0.08 | 0.08 | x1.00 | 0 | 8/8 | same | 0 | 0 | 0 to 0 | 0 to 0 | 0 to 0 | 0 to 0 | 0.2208 |
| elementwise_add_plain | 1.1 | 1.1 | x1.00 | 0 | 8/8 | same | 0 | 0 | 0 to 0 | 0 to 0 | 0 to 0 | 0 to 0 | 0.2344 |
| elementwise_add_scalar | 0.06 | 0.06 | x1.00 | 0 | 7/8 | same | 0 | 0 | 0 to 0 | 0 to 0 | 0 to 0 | 0 to 0 | 0.2446 |
| elementwise_mul | 6.87 | 6.87 | x1.00 | 0 | 7/8 | same | 0 | 0 | 1 to 1 | 1 to 1 | 0 to 0 | 0 to 0 | 0.3092 |
| elementwise_mul_plain | 2.68 | 2.68 | x1.00 | 0 | 8/8 | same | 0 | 0 | 1 to 1 | 0 to 0 | 1 to 1 | 0 to 0 | 0.2746 |
| elementwise_mul_scalar | 1.69 | 1.69 | x1.00 | 0 | 7/8 | same | 0 | 0 | 1 to 1 | 0 to 0 | 1 to 1 | 0 to 0 | 0.2723 |
| embedding | 135.31 | 135.31 | x1.00 | 0 | 8/8 | same | 3 | 0 | 2 to 2 | 0 to 0 | 6 to 6 | 26 to 26 | 1.7698 |
| extract | 4.7 | 4.7 | x1.00 | 0 | 7/8 | same | 0 | 1 | 1 to 1 | 0 to 0 | 1 to 1 | 1 to 1 | 0.4775 |
| extract_linear | 4.75 | 4.75 | x1.00 | 0 | 5/8 | same | 0 | 1 | 1 to 1 | 0 to 0 | 1 to 1 | 1 to 1 | 0.3908 |
| extract_sparse | 14.47 | 10.99 | x1.29 r | 2 | 8/8 | changed | 2 | 0 | 1 to 1 | 0 to 0 | 3 to 2 | 3 to 2 | 0.612 |
| nn_avgpool2d | 7.56 | 7.56 | x1.00 | 0 | 6/8 | same | 1 | 0 | None to None | 0 to 0 | 0 to 0 | 0 to 0 | 0.5101 |
| nn_conv2d | 6674.26 | 3223.63 | x2.21 r | 3 | 7/8 | changed | 3 | 0 | None to None | 144 to 64 | 0 to 0 | 0 to 0 | 2.7042 |
| nn_linear | 1782.6 | 1782.6 | x1.00 | 0 | 8/8 | same | 4 | 0 | None to None | 36 to 36 | 0 to 0 | 0 to 0 | 0.8387 |
| quad | 6.1 | 6.1 | x1.00 | 0 | 8/8 | same | 1 | 0 | 1 to 1 | 1 to 1 | 0 to 0 | 0 to 0 | 0.3526 |
| relu_composite | 641.09 | 641.09 | x1.00 | 0 | 6/8 | same | 5 | 2 | 10 to 10 | 16 to 16 | 12 to 12 | 0 to 0 | 1.7642 |
| table_mult | 61.01 | 61.01 | x1.00 | 0 | 7/8 | same | 0 | 1 | 1 to 1 | 0 to 0 | 3 to 3 | 24 to 24 | 0.8233 |

29 tasks; 13 measured 3% or more faster, 13 with a changed circuit; 200 correct attempts, 76 new circuits, 22 CHEHAB errors. Total cost $24.88. Models: claude-sonnet-5-5.
