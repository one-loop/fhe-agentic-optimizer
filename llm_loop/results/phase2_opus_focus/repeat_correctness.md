# Repeated correctness check

Each kernel evaluated 20 times on the fixed, dense and held-out cases (fresh encryption
each time). Written by `experiments/repeat_correctness.py`. No timing.

| task | kernel | passed | worst max abs error | tolerance |
|---|---|---:|---:|---:|
| chebyshev_gelu_d7 | baseline | 20 / 20 | 3.08e-07 | 1e-06 |
| chebyshev_gelu_d7 | archive/run1 | 19 / 20 | 1.06e-06 | 1e-06 |
| chebyshev_sigmoid_d7 | baseline | 20 / 20 | 1.29e-07 | 1e-06 |
| chebyshev_sigmoid_d7 | archive/run1 | 20 / 20 | 8.08e-08 | 1e-06 |
