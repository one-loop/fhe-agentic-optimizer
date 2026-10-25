# elementwise_mul_plain: LLM optimization run 20261024-125052

Model `claude-sonnet-5-5` (effort medium), 8 attempts, wall time 140 s, approximate API cost $0.0000.

| | fhe() time (ms) | levels | (depth, mult. depth) | ct-ct mul | ct-pt mul | rotations | max error |
|---|---:|---:|---|---:|---:|---:|---:|
| baseline | 13.52 | 1 | (1, 1) | 0 | 1 | 0 | 4.6e-08 |
| best (attempt 3) | 13.03 | 1 | (1, 1) | 0 | 1 | 0 | 5.5e-08 |

Speedup: x1.04.

| attempt | outcome | fhe() time (ms) | note |
|---:|---|---:|---|
| 1 | ok | 13.06 | new best |
| 2 | ok | 13.15 | kernel c3c5ae909a |
| 3 | ok | 13.03 | new best |
| 4 | ok | 13.19 | kernel c3c5ae909a |
| 5 | ok | 13.38 | kernel c3c5ae909a |
| 6 | ok | 13.73 | kernel c3c5ae909a |
| 7 | ok | 13.05 | kernel c3c5ae909a |
| 8 | ok | 13.39 | kernel c3c5ae909a |
