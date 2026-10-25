# elementwise_mul_plain: LLM optimization run 20261013-161531

Model `claude-haiku-5-5` (effort medium), 8 attempts, wall time 106 s, approximate API cost $0.0019.

| | fhe() time (ms) | levels | (depth, mult. depth) | ct-ct mul | ct-pt mul | rotations | max error |
|---|---:|---:|---|---:|---:|---:|---:|
| baseline | 13.43 | 1 | (1, 1) | 0 | 1 | 0 | 2.6e-08 |
| best (attempt 4) | 13.01 | 1 | (1, 1) | 0 | 1 | 0 | 2.8e-08 |

Speedup: x1.03.

| attempt | outcome | fhe() time (ms) | note |
|---:|---|---:|---|
| 1 | ok | 13.15 | new best |
| 2 | ok | 14.66 | kernel 88c5adc697 |
| 3 | ok | 13.21 | kernel 88c5adc697 |
| 4 | ok | 13.01 | new best |
| 5 | ok | 13.52 | kernel 88c5adc697 |
| 6 | ok | 13.32 | kernel 88c5adc697 |
| 7 | ok | 13.26 | kernel 88c5adc697 |
| 8 | ok | 13.03 | kernel 88c5adc697 |
