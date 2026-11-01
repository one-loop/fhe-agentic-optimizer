# elementwise_mul_scalar: LLM optimization run 20261012-144714

Model `claude-haiku-5-5` (effort medium), 8 attempts, wall time 74 s, approximate API cost $0.0018.

| | fhe() time (ms) | levels | (depth, mult. depth) | ct-ct mul | ct-pt mul | rotations | max error |
|---|---:|---:|---|---:|---:|---:|---:|
| baseline | 4.59 | 1 | (1, 1) | 0 | 1 | 0 | 4.3e-08 |
| best (attempt 4) | 4.58 | 1 | (1, 1) | 0 | 1 | 0 | 4.2e-08 |

Speedup: x1.00.

| attempt | outcome | fhe() time (ms) | note |
|---:|---|---:|---|
| 1 | ok | 4.59 | kernel d015b5f55b |
| 2 | ok | 4.60 | kernel d015b5f55b |
| 3 | ok | 4.60 | kernel d015b5f55b |
| 4 | ok | 4.58 | new best |
| 5 | ok | 4.59 | kernel d015b5f55b |
| 6 | ok | 4.60 | kernel d015b5f55b |
| 7 | ok | 4.58 | kernel d015b5f55b |
| 8 | ok | 4.58 | kernel d015b5f55b |
