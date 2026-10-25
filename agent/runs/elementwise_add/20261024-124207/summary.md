# elementwise_add: LLM optimization run 20261024-124207

Model `claude-sonnet-5-5` (effort medium), 8 attempts, wall time 129 s, approximate API cost $0.0000.

| | fhe() time (ms) | levels | (depth, mult. depth) | ct-ct mul | ct-pt mul | rotations | max error |
|---|---:|---:|---|---:|---:|---:|---:|
| baseline | 0.72 | 0 | (1, 0) | 0 | 0 | 0 | 2.3e-08 |
| best (attempt 0) | 0.72 | 0 | (1, 0) | 0 | 0 | 0 | 2.3e-08 |

Speedup: x1.00.

| attempt | outcome | fhe() time (ms) | note |
|---:|---|---:|---|
| 1 | ok | 1.71 | kernel 37fceeacdf |
| 2 | ok | 0.76 | kernel 37fceeacdf |
| 3 | ok | 0.74 | kernel 37fceeacdf |
| 4 | ok | 0.80 | kernel 37fceeacdf |
| 5 | ok | 0.75 | kernel 37fceeacdf |
| 6 | ok | 1.02 | kernel 37fceeacdf |
| 7 | ok | 0.80 | kernel 37fceeacdf |
| 8 | ok | 0.82 | kernel 37fceeacdf |
