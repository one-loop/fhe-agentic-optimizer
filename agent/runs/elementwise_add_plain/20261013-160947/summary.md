# elementwise_add_plain: LLM optimization run 20261013-160947

Model `claude-haiku-5-5` (effort medium), 8 attempts, wall time 115 s, approximate API cost $0.0019.

| | fhe() time (ms) | levels | (depth, mult. depth) | ct-ct mul | ct-pt mul | rotations | max error |
|---|---:|---:|---|---:|---:|---:|---:|
| baseline | 4.74 | 0 | (1, 0) | 0 | 0 | 0 | 2.5e-08 |
| best (attempt 5) | 4.63 | 0 | (1, 0) | 0 | 0 | 0 | 2.2e-08 |

Speedup: x1.02.

| attempt | outcome | fhe() time (ms) | note |
|---:|---|---:|---|
| 1 | ok | 4.64 | new best |
| 2 | ok | 4.67 | kernel c18e8ea006 |
| 3 | ok | 4.66 | kernel c18e8ea006 |
| 4 | ok | 6.66 | kernel c18e8ea006 |
| 5 | ok | 4.63 | new best |
| 6 | ok | 4.84 | kernel c18e8ea006 |
| 7 | ok | 4.70 | kernel c18e8ea006 |
| 8 | ok | 4.67 | kernel c18e8ea006 |
