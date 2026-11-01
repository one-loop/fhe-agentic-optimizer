# elementwise_mul_plain: LLM optimization run 20261012-144559

Model `claude-haiku-5-5` (effort medium), 8 attempts, wall time 75 s, approximate API cost $0.0017.

| | fhe() time (ms) | levels | (depth, mult. depth) | ct-ct mul | ct-pt mul | rotations | max error |
|---|---:|---:|---|---:|---:|---:|---:|
| baseline | 7.03 | 1 | (1, 1) | 0 | 1 | 0 | 2.2e-08 |
| best (attempt 0) | 7.03 | 1 | (1, 1) | 0 | 1 | 0 | 2.2e-08 |

Speedup: x1.00.

| attempt | outcome | fhe() time (ms) | note |
|---:|---|---:|---|
| 1 | ok | 7.03 | kernel c3c5ae909a |
| 2 | ok | 7.05 | kernel c3c5ae909a |
| 3 | ok | 7.04 | kernel c3c5ae909a |
| 4 | ok | 7.04 | kernel c3c5ae909a |
| 5 | ok | 7.06 | kernel c3c5ae909a |
| 6 | ok | 7.04 | kernel c3c5ae909a |
| 7 | ok | 7.06 | kernel c3c5ae909a |
| 8 | ok | 7.05 | kernel c3c5ae909a |
