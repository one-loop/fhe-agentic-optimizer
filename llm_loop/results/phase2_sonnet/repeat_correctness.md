# Repeated correctness check

Each kernel evaluated 20 times on the fixed, dense and held-out cases (fresh encryption
each time). Written by `experiments/repeat_correctness.py`. No timing.

| task | kernel | passed | worst max abs error | tolerance |
|---|---|---:|---:|---:|
| chebyshev_hardshrink_d7 | baseline | 20 / 20 | 1.74e-07 | 1e-06 |
| chebyshev_hardshrink_d7 | archive/run3 | 20 / 20 | 8.11e-07 | 1e-06 |
