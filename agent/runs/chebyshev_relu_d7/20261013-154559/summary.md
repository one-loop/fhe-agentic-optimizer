# chebyshev_relu_d7: LLM optimization run 20261013-154559

Model `claude-haiku-5-5` (effort medium), 8 attempts, wall time 197 s, approximate API cost $0.0083.

| | fhe() time (ms) | levels | (depth, mult. depth) | ct-ct mul | ct-pt mul | rotations | max error |
|---|---:|---:|---|---:|---:|---:|---:|
| baseline | 305.33 | 7 | (19, 7) | 5 | 5 | 0 | 1.3e-07 |
| best (attempt 2) | 229.62 | 5 | (13, 5) | 3 | 5 | 0 | 1.1e-07 |

Speedup: x1.33.

| attempt | outcome | fhe() time (ms) | note |
|---:|---|---:|---|
| 1 | ok | 233.17 | new best |
| 2 | ok | 229.62 | new best |
| 3 | ok | 234.86 | kernel d467de14fd |
| 4 | ok | 229.79 | kernel d467de14fd |
| 5 | ok | 230.05 | kernel d467de14fd |
| 6 | ok | 230.52 | kernel d467de14fd |
| 7 | ok | 229.93 | kernel d467de14fd |
| 8 | ok | 229.78 | kernel d467de14fd |
