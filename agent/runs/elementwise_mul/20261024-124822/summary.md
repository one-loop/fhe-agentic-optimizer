# elementwise_mul: LLM optimization run 20261024-124822

Model `claude-sonnet-5-5` (effort medium), 8 attempts, wall time 150 s, approximate API cost $0.0000.

| | fhe() time (ms) | levels | (depth, mult. depth) | ct-ct mul | ct-pt mul | rotations | max error |
|---|---:|---:|---|---:|---:|---:|---:|
| baseline | 67.83 | 1 | (1, 1) | 1 | 0 | 0 | 3.9e-08 |
| best (attempt 2) | 64.87 | 1 | (1, 1) | 1 | 0 | 0 | 6.4e-08 |

Speedup: x1.05.

| attempt | outcome | fhe() time (ms) | note |
|---:|---|---:|---|
| 1 | ok | 64.89 | new best |
| 2 | ok | 64.87 | new best |
| 3 | ok | 64.95 | kernel 5e8ed7a9b4 |
| 4 | ok | 65.22 | kernel 5e8ed7a9b4 |
| 5 | ok | 71.50 | kernel 5e8ed7a9b4 |
| 6 | ok | 73.66 | kernel 5e8ed7a9b4 |
| 7 | ok | 65.40 | kernel 5e8ed7a9b4 |
| 8 | ok | 66.09 | kernel 5e8ed7a9b4 |
