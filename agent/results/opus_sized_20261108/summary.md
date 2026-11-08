# LLM optimization results (latest run per task)

fhe() time is the median evaluation time on the dense case. Levels, ciphertext-ciphertext multiplications (ct x ct), ciphertext-plaintext multiplications (ct x pt) and rotations are CHEHAB's counts for the baseline and for the best correct kernel.

Speedup is from baseline and best timed in alternating batches when the run remeasured it (marked r); older runs only have one median each. 'circuit' says whether the best kernel's circuit differs from the baseline's; 'new' counts distinct new correct circuits in the run.

| task | baseline ms | best ms | speedup | best attempt | correct | circuit | new | CHEHAB errors | levels | ct x ct | ct x pt | rotations | cost USD |
|---|---:|---:|---:|---:|---:|---|---:|---:|---|---|---|---|---:|
| batchnorm1d | 3.79 | 3.79 | x1.00 | 0 | 8/8 | same | 0 | 0 | 1 to 1 | 0 to 0 | 1 to 1 | 0 to 0 | 0.8711 |
| batchnorm2d | 3.73 | 3.73 | x1.00 | 0 | 8/8 | same | 0 | 0 | 1 to 1 | 0 to 0 | 1 to 1 | 0 to 0 | 0.7982 |
| chebyshev_elu_d7 | 241.48 | 91.03 | x2.65 r | 2 | 8/8 | changed | 7 | 0 | 8 to 4 | 6 to 5 | 8 to 5 | 0 to 0 | 5.4094 |
| chebyshev_gelu_d7 | 167.42 | 57.25 | x2.96 r | 1 | 8/8 | changed | 7 | 0 | 7 to 4 | 5 to 3 | 5 to 4 | 0 to 0 | 3.5564 |
| chebyshev_hardshrink_d7 | 213.05 | 84.09 | x2.40 r | 4 | 8/8 | changed | 8 | 0 | 8 to 4 | 6 to 5 | 5 to 5 | 0 to 0 | 4.4689 |
| chebyshev_mish_d7 | 242.69 | 108.95 | x2.22 r | 6 | 7/8 | changed | 7 | 1 | 8 to 5 | 6 to 5 | 8 to 6 | 0 to 0 | 4.8947 |
| chebyshev_relu_d2 | 24.03 | 17.04 | x1.41 r | 1 | 8/8 | changed | 3 | 0 | 3 to 3 | 1 to 1 | 3 to 2 | 0 to 0 | 1.9538 |
| chebyshev_relu_d7 | 166.9 | 57.04 | x2.96 r | 1 | 8/8 | changed | 7 | 0 | 7 to 4 | 5 to 3 | 5 to 4 | 0 to 0 | 3.0182 |
| chebyshev_selu_d7 | 241.22 | 88.1 | x2.71 r | 6 | 8/8 | changed | 5 | 0 | 8 to 4 | 6 to 5 | 8 to 5 | 0 to 0 | 6.6981 |
| chebyshev_sigmoid_d5 | 113.16 | 55.07 | x2.05 r | 4 | 8/8 | changed | 8 | 0 | 6 to 5 | 4 to 3 | 4 to 2 | 0 to 0 | 4.3654 |
| chebyshev_sigmoid_d7 | 214.04 | 86.11 | x2.59 r | 3 | 8/8 | changed | 7 | 0 | 8 to 4 | 6 to 5 | 5 to 5 | 0 to 0 | 5.2714 |
| chebyshev_silu_d7 | 177.99 | 62.13 | x2.99 r | 1 | 8/8 | changed | 7 | 0 | 7 to 4 | 5 to 3 | 5 to 4 | 0 to 0 | 3.5357 |
| chebyshev_softplus_d7 | 166.92 | 55.82 | x3.01 r | 3 | 8/8 | changed | 7 | 0 | 7 to 4 | 5 to 3 | 5 to 4 | 0 to 0 | 3.3698 |
| elementwise_add | 0.08 | 0.08 | x1.00 | 0 | 8/8 | same | 0 | 0 | 0 to 0 | 0 to 0 | 0 to 0 | 0 to 0 | 0.4977 |
| elementwise_add_plain | 1.23 | 1.23 | x1.00 | 0 | 8/8 | same | 0 | 0 | 0 to 0 | 0 to 0 | 0 to 0 | 0 to 0 | 0.4998 |
| elementwise_add_scalar | 0.05 | 0.05 | x1.00 | 0 | 8/8 | same | 0 | 0 | 0 to 0 | 0 to 0 | 0 to 0 | 0 to 0 | 0.5102 |
| elementwise_mul | 6.12 | 6.12 | x1.00 | 0 | 8/8 | same | 0 | 0 | 1 to 1 | 1 to 1 | 0 to 0 | 0 to 0 | 0.6565 |
| elementwise_mul_plain | 2.89 | 2.89 | x1.00 | 0 | 8/8 | same | 0 | 0 | 1 to 1 | 0 to 0 | 1 to 1 | 0 to 0 | 0.6163 |
| elementwise_mul_scalar | 1.66 | 1.66 | x1.00 | 0 | 8/8 | same | 0 | 0 | 1 to 1 | 0 to 0 | 1 to 1 | 0 to 0 | 0.6415 |
| embedding | 130.11 | 130.11 | x1.00 | 0 | 8/8 | same | 1 | 0 | 2 to 2 | 0 to 0 | 6 to 6 | 26 to 26 | 6.4201 |
| extract | 4.76 | 4.76 | x1.00 | 0 | 7/8 | same | 0 | 1 | 1 to 1 | 0 to 0 | 1 to 1 | 1 to 1 | 0.9964 |
| extract_linear | 4.75 | 4.75 | x1.00 | 0 | 7/8 | same | 0 | 1 | 1 to 1 | 0 to 0 | 1 to 1 | 1 to 1 | 0.8004 |
| extract_sparse | 14.16 | 12.14 | x1.18 r | 2 | 8/8 | changed | 2 | 0 | 1 to 1 | 0 to 0 | 3 to 3 | 3 to 2 | 1.4182 |
| nn_avgpool2d | 7.27 | 7.27 | x1.00 | 0 | 8/8 | same | 1 | 0 | None to None | 0 to 0 | 0 to 0 | 0 to 0 | 1.1171 |
| nn_conv2d | 6761.76 | 1797.29 | x3.74 r | 2 | 8/8 | changed | 8 | 0 | None to None | 144 to 36 | 0 to 16 | 0 to 0 | 11.5351 |
| nn_linear | 1700.5 | 1700.5 | x1.00 | 0 | 8/8 | same | 3 | 0 | None to None | 36 to 36 | 0 to 0 | 0 to 0 | 2.738 |
| quad | 6.06 | 6.06 | x1.00 | 0 | 8/8 | same | 1 | 0 | 1 to 1 | 1 to 1 | 0 to 0 | 0 to 0 | 0.6808 |
| relu_composite | 641.15 | 589.31 | x1.09 r | 5 | 5/8 | changed | 4 | 3 | 10 to 9 | 16 to 19 | 12 to 12 | 0 to 0 | 6.4965 |
| table_mult | 60.93 | 60.93 | x1.00 | 0 | 7/8 | same | 1 | 1 | 1 to 1 | 0 to 0 | 3 to 3 | 24 to 24 | 2.4171 |

29 tasks; 14 measured 3% or more faster, 14 with a changed circuit; 225 correct attempts, 94 new circuits, 7 CHEHAB errors. Total cost $86.25. Models: claude-opus-5-5.
