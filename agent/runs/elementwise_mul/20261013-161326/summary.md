# elementwise_mul: LLM optimization run 20261013-161326

Model `claude-haiku-5-5` (effort medium), 8 attempts, wall time 124 s, approximate API cost $0.0023.

| | fhe() time (ms) | levels | (depth, mult. depth) | ct-ct mul | ct-pt mul | rotations | max error |
|---|---:|---:|---|---:|---:|---:|---:|
| baseline | 65.53 | 1 | (1, 1) | 1 | 0 | 0 | 4.4e-08 |
| best (attempt 1) | 64.83 | 1 | (1, 1) | 1 | 0 | 0 | 2.9e-08 |

Speedup: x1.01.

| attempt | outcome | fhe() time (ms) | note |
|---:|---|---:|---|
| 1 | ok | 64.83 | new best |
| 2 | ok | 68.16 | kernel bb769c7fb8 |
| 3 | ok | 65.71 | kernel 2c3aed7bd4 |
| 4 | ok | 67.64 | kernel 2a8c906fc8 |
| 5 | ok | 68.05 | kernel 2a8c906fc8 |
| 6 | ok | 66.58 | kernel 2a8c906fc8 |
| 7 | ok | 65.04 | kernel 2a8c906fc8 |
| 8 | ok | 66.24 | kernel 2a8c906fc8 |
