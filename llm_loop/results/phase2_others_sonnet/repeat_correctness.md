# Repeated correctness check

Each kernel evaluated 20 times on the fixed, dense and held-out cases (fresh encryption
each time). Written by `experiments/repeat_correctness.py`. No timing.

| task | kernel | passed | worst max abs error | tolerance |
|---|---|---:|---:|---:|
| relu_composite | baseline | 17 / 20 | 1.1e-06 | 1e-06 |
| relu_composite | archive/run1 | 18 / 20 | 1.44e-06 | 1e-06 |
