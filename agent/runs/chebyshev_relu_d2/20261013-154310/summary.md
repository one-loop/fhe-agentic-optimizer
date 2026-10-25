# chebyshev_relu_d2: LLM optimization run 20261013-154310

Model `claude-haiku-5-5` (effort medium), 8 attempts, wall time 169 s, approximate API cost $0.0087.

| | fhe() time (ms) | levels | (depth, mult. depth) | ct-ct mul | ct-pt mul | rotations | max error |
|---|---:|---:|---|---:|---:|---:|---:|
| baseline | 117.16 | 3 | (7, 3) | 1 | 3 | 0 | 1.2e-07 |
| best (attempt 3) | 81.18 | 3 | (7, 3) | 1 | 2 | 0 | 1e-07 |

Speedup: x1.44.

| attempt | outcome | fhe() time (ms) | note |
|---:|---|---:|---|
| 1 | ok | 81.22 | new best |
| 2 | ok | 81.39 | kernel 0c1f93d9e3 |
| 3 | ok | 81.18 | new best |
| 4 | ok | 81.31 | kernel 0c1f93d9e3 |
| 5 | ok | 81.53 | kernel 0c1f93d9e3 |
| 6 | ok | 83.23 | kernel 0c1f93d9e3 |
| 7 | ok | 85.42 | kernel 0c1f93d9e3 |
| 8 | ok | 93.40 | kernel 0c1f93d9e3 |
