# LLM optimization results (latest run per task)

fhe() time is the median evaluation time on the dense case. Levels, ciphertext-ciphertext multiplications (ct x ct), ciphertext-plaintext multiplications (ct x pt) and rotations are CHEHAB's counts for the baseline and for the best correct kernel.

Speedup is from baseline and best timed in alternating batches when the run remeasured it (marked r); older runs only have one median each. 'circuit' says whether the best kernel's circuit differs from the baseline's; 'new' counts distinct new correct circuits in the run.

| task | baseline ms | best ms | speedup | best attempt | correct | circuit | new | CHEHAB errors | levels | ct x ct | ct x pt | rotations | cost USD |
|---|---:|---:|---:|---:|---:|---|---:|---:|---|---|---|---|---:|
| batchnorm1d | 9.86 | 9.86 | x1.00 | 0 | 8/8 | same | 0 | 0 | 1 to 1 | 0 to 0 | 1 to 1 | 0 to 0 | 0.8702 |
| batchnorm2d | 9.91 | 9.91 | x1.00 | 0 | 8/8 | same | 0 | 0 | 1 to 1 | 0 to 0 | 1 to 1 | 0 to 0 | 0.7389 |
| chebyshev_elu_d7 | 182.57 | 168.64 | x1.08 r | 1 | 8/8 | changed | 8 | 0 | 8 to 5 | 6 to 4 | 8 to 9 | 0 to 0 | 4.4267 |
| chebyshev_gelu_d7 | 152.45 | 108.6 | x1.41 r | 1 | 8/8 | changed | 7 | 0 | 7 to 4 | 5 to 3 | 5 to 4 | 0 to 0 | 3.025 |
| chebyshev_hardshrink_d7 | 161.07 | 99.77 | x1.62 r | 8 | 8/8 | changed | 8 | 0 | 8 to 6 | 6 to 4 | 5 to 2 | 0 to 0 | 3.076 |
| chebyshev_mish_d7 | 182.22 | 161.32 | x1.12 r | 6 | 7/8 | changed | 6 | 1 | 8 to 5 | 6 to 5 | 8 to 6 | 0 to 0 | 4.8494 |
| chebyshev_relu_d2 | 53.55 | 41.52 | x1.29 r | 1 | 8/8 | changed | 2 | 0 | 3 to 3 | 1 to 1 | 3 to 2 | 0 to 0 | 1.9853 |
| chebyshev_relu_d7 | 152.63 | 107.81 | x1.41 r | 1 | 8/8 | changed | 7 | 0 | 7 to 4 | 5 to 3 | 5 to 4 | 0 to 0 | 3.7352 |
| chebyshev_selu_d7 | 182.14 | 158.72 | x1.17 r | 8 | 7/8 | changed | 7 | 1 | 8 to 5 | 6 to 4 | 8 to 9 | 0 to 0 | 8.4132 |
| chebyshev_sigmoid_d5 | 130.28 | 83.88 | x1.54 r | 6 | 8/8 | changed | 8 | 0 | 6 to 5 | 4 to 3 | 4 to 2 | 0 to 0 | 4.4553 |
| chebyshev_sigmoid_d7 | 161.85 | 100.78 | x1.60 r | 1 | 8/8 | changed | 8 | 0 | 8 to 6 | 6 to 4 | 5 to 2 | 0 to 0 | 3.8959 |
| chebyshev_silu_d7 | 152.45 | 95.94 | x1.58 r | 2 | 8/8 | changed | 7 | 0 | 7 to 5 | 5 to 3 | 5 to 3 | 0 to 0 | 3.9427 |
| chebyshev_softplus_d7 | 152.0 | 99.5 | x1.52 r | 5 | 8/8 | changed | 8 | 0 | 7 to 4 | 5 to 3 | 5 to 3 | 0 to 0 | 3.3038 |
| elementwise_add | 0.46 | 0.46 | x1.00 | 0 | 8/8 | same | 0 | 0 | 0 to 0 | 0 to 0 | 0 to 0 | 0 to 0 | 0.4584 |
| elementwise_add_plain | 2.72 | 2.72 | x1.00 | 0 | 8/8 | same | 0 | 0 | 0 to 0 | 0 to 0 | 0 to 0 | 0 to 0 | 0.4892 |
| elementwise_add_scalar | 0.26 | 0.26 | x1.00 | 0 | 8/8 | same | 0 | 0 | 0 to 0 | 0 to 0 | 0 to 0 | 0 to 0 | 0.4689 |
| elementwise_mul | 31.58 | 31.58 | x1.00 | 0 | 8/8 | same | 0 | 0 | 1 to 1 | 1 to 1 | 0 to 0 | 0 to 0 | 0.545 |
| elementwise_mul_plain | 7.06 | 7.06 | x1.00 | 0 | 8/8 | same | 0 | 0 | 1 to 1 | 0 to 0 | 1 to 1 | 0 to 0 | 0.5108 |
| elementwise_mul_scalar | 4.59 | 4.59 | x1.00 | 0 | 8/8 | same | 0 | 0 | 1 to 1 | 0 to 0 | 1 to 1 | 0 to 0 | 0.5373 |
| embedding | 620.5 | 107.92 | x5.68 r | 2 | 8/8 | changed | 5 | 0 | 2 to 10 | 0 to 0 | 6 to 14 | 26 to 26 | 6.2258 |
| extract | 29.22 | 29.22 | x1.00 | 0 | 7/8 | same | 0 | 1 | 1 to 1 | 0 to 0 | 1 to 1 | 1 to 1 | 0.9029 |
| extract_linear | 29.18 | 29.18 | x1.00 | 0 | 7/8 | same | 0 | 1 | 1 to 1 | 0 to 0 | 1 to 1 | 1 to 1 | 0.8172 |
| extract_sparse | 88.05 | 64.28 | x1.38 r | 1 | 8/8 | changed | 2 | 0 | 1 to 1 | 0 to 0 | 3 to 2 | 3 to 2 | 1.5549 |
| nn_avgpool2d | 3.92 | 3.92 | x1.00 | 0 | 8/8 | same | 1 | 0 | None to None | 0 to 0 | 0 to 0 | 0 to 0 | 1.0479 |
| nn_conv2d | 3425.2 | 938.34 | x3.69 r | 4 | 8/8 | changed | 8 | 0 | None to None | 144 to 36 | 0 to 16 | 0 to 0 | 8.3936 |
| nn_linear | 866.74 | 866.74 | x1.00 | 0 | 8/8 | same | 1 | 0 | None to None | 36 to 36 | 0 to 0 | 0 to 0 | 1.6287 |
| quad | 31.51 | 31.51 | x1.00 | 0 | 8/8 | same | 4 | 0 | 1 to 1 | 1 to 1 | 0 to 0 | 0 to 0 | 0.7906 |
| relu_composite | 321.07 | 321.07 | x1.00 | 0 | 7/8 | same | 6 | 1 | 10 to 10 | 16 to 16 | 12 to 12 | 0 to 0 | 9.2507 |
| table_mult | 569.63 | 525.92 | x1.07 r | 5 | 3/8 | changed | 1 | 0 | 1 to 2 | 0 to 0 | 3 to 5 | 24 to 24 | 2.4679 |

29 tasks; 15 measured 3% or more faster, 15 with a changed circuit; 222 correct attempts, 104 new circuits, 5 CHEHAB errors. Total cost $82.81. Models: claude-opus-5-5.
