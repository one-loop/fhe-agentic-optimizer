# elementwise_add: LLM optimization run 20261013-160809

Model `claude-haiku-5-5` (effort medium), 8 attempts, wall time 98 s, approximate API cost $0.0017.

| | fhe() time (ms) | levels | (depth, mult. depth) | ct-ct mul | ct-pt mul | rotations | max error |
|---|---:|---:|---|---:|---:|---:|---:|
| baseline | 0.73 | 0 | (1, 0) | 0 | 0 | 0 | 2.6e-08 |
| best (attempt 8) | 0.72 | 0 | (1, 0) | 0 | 0 | 0 | 3.1e-08 |

Speedup: x1.01.

| attempt | outcome | fhe() time (ms) | note |
|---:|---|---:|---|
| 1 | ok | 0.75 | kernel d3397e6287 |
| 2 | ok | 0.75 | kernel c98a8a9818 |
| 3 | ok | 0.84 | kernel c98a8a9818 |
| 4 | ok | 0.75 | kernel c98a8a9818 |
| 5 | ok | 0.83 | kernel c98a8a9818 |
| 6 | ok | 0.79 | kernel c98a8a9818 |
| 7 | ok | 0.73 | kernel c98a8a9818 |
| 8 | ok | 0.72 | new best |
