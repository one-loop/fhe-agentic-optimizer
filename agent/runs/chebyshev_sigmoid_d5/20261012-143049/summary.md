# chebyshev_sigmoid_d5: LLM optimization run 20261012-143049

Model `claude-haiku-5-5` (effort medium), 8 attempts, wall time 168 s, approximate API cost $0.0130.

| | fhe() time (ms) | levels | (depth, mult. depth) | ct-ct mul | ct-pt mul | rotations | max error |
|---|---:|---:|---|---:|---:|---:|---:|
| baseline | 129.13 | 6 | (16, 6) | 4 | 4 | 0 | 4.2e-08 |
| best (attempt 2) | 107.66 | 5 | (18, 5) | 3 | 3 | 0 | 3e-08 |

Speedup: x1.20.

| attempt | outcome | fhe() time (ms) | note |
|---:|---|---:|---|
| 1 | ok | 107.78 | new best |
| 2 | ok | 107.66 | new best |
| 3 | ok | 107.77 | kernel 7add702e84 |
| 4 | ok | 107.87 | kernel 7add702e84 |
| 5 | ok | 107.89 | kernel 7add702e84 |
| 6 | ok | 108.08 | kernel 7add702e84 |
| 7 | ok | 107.97 | kernel 7add702e84 |
| 8 | ok | 108.17 | kernel 7add702e84 |
