# chebyshev_relu_d2: LLM optimization run 20261024-121547

Model `claude-sonnet-5-5` (effort medium), 8 attempts, wall time 188 s, approximate API cost $0.0000.

| | fhe() time (ms) | levels | (depth, mult. depth) | ct-ct mul | ct-pt mul | rotations | max error |
|---|---:|---:|---|---:|---:|---:|---:|
| baseline | 107.23 | 3 | (7, 3) | 1 | 3 | 0 | 1e-07 |
| best (attempt 3) | 81.55 | 3 | (7, 3) | 1 | 2 | 0 | 6.9e-08 |

Speedup: x1.31.

| attempt | outcome | fhe() time (ms) | note |
|---:|---|---:|---|
| 1 | ok | 81.95 | new best |
| 2 | ok | 81.57 | new best |
| 3 | ok | 81.55 | new best |
| 4 | ok | 81.80 | kernel 489da5b1f8 |
| 5 | ok | 81.69 | kernel 489da5b1f8 |
| 6 | ok | 82.32 | kernel 489da5b1f8 |
| 7 | ok | 82.68 | kernel 489da5b1f8 |
| 8 | ok | 81.75 | kernel 489da5b1f8 |
