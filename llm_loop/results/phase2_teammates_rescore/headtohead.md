# Head-to-head neutral measurement of final kernels

Commit d1ccac7b28; team kernels from shared/llm-feedback-loop at 7bdb7c2. One session per task; correctness on fixed, dense and held-out cases; speedup = baseline eval-only median / kernel eval-only median. Compares kernels, not search methods: models, runs and budgets differ.

| task | baseline (ms) | my archive (no hints), median [all] | my linear (no hints), median [all] | api_haiku | sonnet_v1 | sonnet_v2 |
|---|---:|---|---|---:|---:|---:|
| chebyshev_gelu_d7 | 174.4 | x1.467 [x1.549 x1.411 x1.467 x1.556 x1.414] | x1.416 [x1.371 x1.468 x1.428 x1.416 x1.411] | x1.305 | x1.416 | x1.441 |
| chebyshev_silu_d7 | 174.2 | x1.415 [x1.410 x1.554 x1.415] | x1.407 [x1.407 x1.412 x1.192] | x1.417 | x1.547 | x1.467 |
| chebyshev_sigmoid_d7 | 185.2 | x1.305 [x1.446 x1.185 x1.225 x1.305 x1.320] | x1.207 [x1.186 x1.219 x1.207 x1.187 x1.320] | x1.128 | x1.465 | x1.259 |
| chebyshev_elu_d7 | 207.3 | x1.090 [x1.090 x1.078 x1.086 x1.092 x1.094] | x1.077 [x1.096 x1.076 x1.102 x1.077 x1.076] | x1.075 | x1.000 | x1.422 |
| chebyshev_selu_d7 | 207.0 | x1.074 [x1.077 x1.072 x1.074] | x1.090 [x1.092 x1.090 x1.078] | x1.035 | x1.423 | x1.181 |
| chebyshev_softplus_d7 | 173.8 | x1.427 [x1.427 x1.470 x1.413] | x1.414 [x1.466 x1.414 x1.196] | x1.368 | x1.399 | x1.476 |
| chebyshev_mish_d7 | 207.1 | x1.079 [x1.095 x1.075 x1.079] | x1.078 [x1.091 x1.078 x1.074] | x1.068 | x1.018 | x1.422 |
| chebyshev_hardshrink_d7 | 184.4 | x1.232 [x1.369 x1.232 x1.222] | x1.220 [x1.451 x1.188 x1.220] | x1.122 | x1.133 | x1.200 |
| chebyshev_sigmoid_d5 | 148.2 | x1.361 [x1.131 x1.361 x1.521] | x1.251 [x1.308 x1.251 x1.250] | x1.131 | x1.132 | x1.208 |
| quad | 36.6 | x1.000 [x1.000] | x1.000 [x1.000] | x1.011 | x1.000 | x1.000 |
| batchnorm1d | 10.7 | x1.000 [x1.000] | x1.000 [x1.000] | x1.000 | x1.000 | x1.000 |
| batchnorm2d | 10.8 | x0.996 [x0.996] | x1.000 [x1.000] | x1.000 | x1.000 | x1.000 |
