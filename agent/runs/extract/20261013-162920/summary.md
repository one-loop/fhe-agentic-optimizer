# extract: LLM optimization run 20261013-162920

Model `claude-haiku-5-5` (effort medium), 8 attempts, wall time 131 s, approximate API cost $0.0036.

| | fhe() time (ms) | levels | (depth, mult. depth) | ct-ct mul | ct-pt mul | rotations | max error |
|---|---:|---:|---|---:|---:|---:|---:|
| baseline | 59.59 | 1 | (2, 1) | 0 | 1 | 1 | 5.2e-06 |
| best (attempt 7) | 59.24 | 1 | (2, 1) | 0 | 1 | 1 | 5.3e-06 |

Speedup: x1.01.

| attempt | outcome | fhe() time (ms) | note |
|---:|---|---:|---|
| 1 | ok | 59.29 | new best |
| 2 | ok | 60.02 | kernel b21ed31fdf |
| 3 | ok | 59.86 | kernel bd7b0beb90 |
| 4 | ok | 59.60 | kernel bd7b0beb90 |
| 5 | ok | 60.76 | kernel bd7b0beb90 |
| 6 | ok | 61.89 | kernel bd7b0beb90 |
| 7 | ok | 59.24 | new best |
| 8 | ok | 59.65 | kernel bd7b0beb90 |
