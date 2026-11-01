# relu_composite: LLM optimization run 20261012-134732

Model `claude-haiku-5-5` (effort medium), 8 attempts, wall time 237 s, approximate API cost $0.0165.

| | fhe() time (ms) | levels | (depth, mult. depth) | ct-ct mul | ct-pt mul | rotations | max error |
|---|---:|---:|---|---:|---:|---:|---:|
| baseline | 318.38 | 10 | (17, 10) | 16 | 12 | 0 | 1.7e-07 |
| best (attempt 2) | 317.45 | 10 | (17, 10) | 16 | 12 | 0 | 2e-07 |

Speedup: x1.00.

| attempt | outcome | fhe() time (ms) | note |
|---:|---|---:|---|
| 1 | ok | 317.78 | new best |
| 2 | ok | 317.45 | new best |
| 3 | ok | 317.92 | kernel 3196e7e725 |
| 4 | ok | 317.75 | kernel 3196e7e725 |
| 5 | ok | 317.96 | kernel 3196e7e725 |
| 6 | ok | 318.49 | kernel 3196e7e725 |
| 7 | ok | 318.11 | kernel 3196e7e725 |
| 8 | ok | 321.86 | kernel 3196e7e725 |
