# chebyshev_relu_d2: LLM optimization run 20261012-142324

Model `claude-haiku-5-5` (effort medium), 8 attempts, wall time 115 s, approximate API cost $0.0077.

| | fhe() time (ms) | levels | (depth, mult. depth) | ct-ct mul | ct-pt mul | rotations | max error |
|---|---:|---:|---|---:|---:|---:|---:|
| baseline | 53.43 | 3 | (7, 3) | 1 | 3 | 0 | 1.3e-07 |
| best (attempt 1) | 41.32 | 3 | (7, 3) | 1 | 2 | 0 | 1.3e-07 |

Speedup: x1.29.

| attempt | outcome | fhe() time (ms) | note |
|---:|---|---:|---|
| 1 | ok | 41.32 | new best |
| 2 | ok | 41.39 | kernel 20d89381b5 |
| 3 | ok | 41.40 | kernel 20d89381b5 |
| 4 | ok | 41.41 | kernel 20d89381b5 |
| 5 | ok | 41.48 | kernel 20d89381b5 |
| 6 | ok | 41.48 | kernel 20d89381b5 |
| 7 | ok | 41.45 | kernel 20d89381b5 |
| 8 | ok | 41.47 | kernel 20d89381b5 |
