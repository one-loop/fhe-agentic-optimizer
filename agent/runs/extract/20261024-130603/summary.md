# extract: LLM optimization run 20261024-130603

Model `claude-sonnet-5-5` (effort medium), 8 attempts, wall time 166 s, approximate API cost $0.0000.

| | fhe() time (ms) | levels | (depth, mult. depth) | ct-ct mul | ct-pt mul | rotations | max error |
|---|---:|---:|---|---:|---:|---:|---:|
| baseline | 60.48 | 1 | (2, 1) | 0 | 1 | 1 | 5.9e-06 |
| best (attempt 6) | 59.61 | 1 | (2, 1) | 0 | 1 | 1 | 2.2e-06 |

Speedup: x1.01.

| attempt | outcome | fhe() time (ms) | note |
|---:|---|---:|---|
| 1 | ok | 61.21 | kernel 94dca51d9c |
| 2 | ok | 59.70 | new best |
| 3 | ok | 60.83 | kernel 94dca51d9c |
| 4 | ok | 60.03 | kernel 94dca51d9c |
| 5 | ok | 60.57 | kernel 94dca51d9c |
| 6 | ok | 59.61 | new best |
| 7 | ok | 60.59 | kernel 94dca51d9c |
| 8 | ok | 59.67 | kernel 94dca51d9c |
