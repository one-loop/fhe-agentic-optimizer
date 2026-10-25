# extract_linear: LLM optimization run 20261024-130849

Model `claude-sonnet-5-5` (effort medium), 8 attempts, wall time 150 s, approximate API cost $0.0000.

| | fhe() time (ms) | levels | (depth, mult. depth) | ct-ct mul | ct-pt mul | rotations | max error |
|---|---:|---:|---|---:|---:|---:|---:|
| baseline | 59.42 | 1 | (2, 1) | 0 | 1 | 1 | 4.9e-06 |
| best (attempt 3) | 59.23 | 1 | (2, 1) | 0 | 1 | 1 | 4.4e-06 |

Speedup: x1.00.

| attempt | outcome | fhe() time (ms) | note |
|---:|---|---:|---|
| 1 | ok | 59.63 | kernel 94dca51d9c |
| 2 | chehab_error | - | backend does not support plain plain evaluation |
| 3 | ok | 59.23 | new best |
| 4 | ok | 65.94 | kernel 94dca51d9c |
| 5 | ok | 60.04 | kernel 94dca51d9c |
| 6 | ok | 59.45 | kernel 94dca51d9c |
| 7 | ok | 60.67 | kernel 94dca51d9c |
| 8 | ok | 61.58 | kernel 94dca51d9c |
