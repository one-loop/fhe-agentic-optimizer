# elementwise_mul_scalar: LLM optimization run 20261013-161717

Model `claude-haiku-5-5` (effort medium), 8 attempts, wall time 111 s, approximate API cost $0.0017.

| | fhe() time (ms) | levels | (depth, mult. depth) | ct-ct mul | ct-pt mul | rotations | max error |
|---|---:|---:|---|---:|---:|---:|---:|
| baseline | 9.19 | 1 | (1, 1) | 0 | 1 | 0 | 4.6e-08 |
| best (attempt 7) | 8.88 | 1 | (1, 1) | 0 | 1 | 0 | 4e-08 |

Speedup: x1.03.

| attempt | outcome | fhe() time (ms) | note |
|---:|---|---:|---|
| 1 | ok | 8.92 | new best |
| 2 | ok | 9.34 | kernel d015b5f55b |
| 3 | ok | 9.10 | kernel d015b5f55b |
| 4 | ok | 9.07 | kernel d015b5f55b |
| 5 | ok | 9.02 | kernel d015b5f55b |
| 6 | ok | 8.90 | new best |
| 7 | ok | 8.88 | new best |
| 8 | ok | 8.95 | kernel d015b5f55b |
