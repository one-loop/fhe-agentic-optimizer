# chebyshev_relu_d7: LLM optimization run 20261012-142518

Model `claude-haiku-5-5` (effort medium), 8 attempts, wall time 166 s, approximate API cost $0.0127.

| | fhe() time (ms) | levels | (depth, mult. depth) | ct-ct mul | ct-pt mul | rotations | max error |
|---|---:|---:|---|---:|---:|---:|---:|
| baseline | 152.36 | 7 | (19, 7) | 5 | 5 | 0 | 7e-08 |
| best (attempt 2) | 133.96 | 5 | (17, 5) | 3 | 5 | 0 | 1.1e-07 |

Speedup: x1.14.

| attempt | outcome | fhe() time (ms) | note |
|---:|---|---:|---|
| 1 | ok | 140.60 | new best |
| 2 | ok | 133.96 | new best |
| 3 | ok | 133.97 | kernel 7a3874f300 |
| 4 | ok | 134.15 | kernel 7a3874f300 |
| 5 | ok | 134.42 | kernel 7a3874f300 |
| 6 | ok | 134.52 | kernel 7a3874f300 |
| 7 | ok | 134.40 | kernel 7a3874f300 |
| 8 | ok | 134.33 | kernel 7a3874f300 |
