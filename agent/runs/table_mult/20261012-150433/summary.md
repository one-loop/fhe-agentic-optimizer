# table_mult: LLM optimization run 20261012-150433

Model `claude-haiku-5-5` (effort medium), 8 attempts, wall time 227 s, approximate API cost $0.0099.

| | fhe() time (ms) | levels | (depth, mult. depth) | ct-ct mul | ct-pt mul | rotations | max error |
|---|---:|---:|---|---:|---:|---:|---:|
| baseline | 560.02 | 1 | (17, 1) | 0 | 3 | 24 | 6e-05 |
| best (attempt 8) | 559.87 | 1 | (17, 1) | 0 | 3 | 24 | 4.8e-05 |

Speedup: x1.00.

| attempt | outcome | fhe() time (ms) | note |
|---:|---|---:|---|
| 1 | ok | 560.45 | kernel 8fb13ac6de |
| 2 | ok | 560.19 | kernel 8fb13ac6de |
| 3 | ok | 559.91 | new best |
| 4 | ok | 560.25 | kernel 8fb13ac6de |
| 5 | ok | 560.01 | kernel 8fb13ac6de |
| 6 | ok | 560.62 | kernel 8fb13ac6de |
| 7 | ok | 560.16 | kernel 8fb13ac6de |
| 8 | ok | 559.87 | new best |
