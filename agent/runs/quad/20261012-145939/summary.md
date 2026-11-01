# quad: LLM optimization run 20261012-145939

Model `claude-haiku-5-5` (effort medium), 8 attempts, wall time 79 s, approximate API cost $0.0020.

| | fhe() time (ms) | levels | (depth, mult. depth) | ct-ct mul | ct-pt mul | rotations | max error |
|---|---:|---:|---|---:|---:|---:|---:|
| baseline | 31.41 | 1 | (1, 1) | 1 | 0 | 0 | 4.5e-08 |
| best (attempt 6) | 30.93 | 1 | (1, 1) | 0 | 0 | 0 | 6e-08 |

Speedup: x1.02.

| attempt | outcome | fhe() time (ms) | note |
|---:|---|---:|---|
| 1 | ok | 31.01 | new best |
| 2 | ok | 30.95 | new best |
| 3 | ok | 30.97 | kernel f74e88be02 |
| 4 | ok | 30.97 | kernel f74e88be02 |
| 5 | ok | 30.96 | kernel f74e88be02 |
| 6 | ok | 30.93 | new best |
| 7 | ok | 31.00 | kernel f74e88be02 |
| 8 | ok | 30.94 | kernel f74e88be02 |
