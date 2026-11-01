# chebyshev_silu_d7: LLM optimization run 20261012-143634

Model `claude-haiku-5-5` (effort medium), 8 attempts, wall time 141 s, approximate API cost $0.0094.

| | fhe() time (ms) | levels | (depth, mult. depth) | ct-ct mul | ct-pt mul | rotations | max error |
|---|---:|---:|---|---:|---:|---:|---:|
| baseline | 152.35 | 7 | (19, 7) | 5 | 5 | 0 | 1.2e-07 |
| best (attempt 2) | 110.58 | 4 | (12, 4) | 3 | 4 | 0 | 9e-08 |

Speedup: x1.38.

| attempt | outcome | fhe() time (ms) | note |
|---:|---|---:|---|
| 1 | ok | 115.86 | new best |
| 2 | ok | 110.58 | new best |
| 3 | ok | 110.61 | kernel 866b0c4904 |
| 4 | ok | 110.71 | kernel 866b0c4904 |
| 5 | ok | 110.75 | kernel 866b0c4904 |
| 6 | ok | 110.72 | kernel 866b0c4904 |
| 7 | ok | 110.77 | kernel 866b0c4904 |
| 8 | ok | 110.75 | kernel 866b0c4904 |
