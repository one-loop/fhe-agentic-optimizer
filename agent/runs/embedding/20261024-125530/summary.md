# embedding: LLM optimization run 20261024-125530

Model `claude-sonnet-5-5` (effort medium), 8 attempts, wall time 633 s, approximate API cost $0.0000.

| | fhe() time (ms) | levels | (depth, mult. depth) | ct-ct mul | ct-pt mul | rotations | max error |
|---|---:|---:|---|---:|---:|---:|---:|
| baseline | 1313.33 | 2 | (21, 2) | 0 | 6 | 26 | 2.3e-05 |
| best (attempt 4) | 1276.17 | 2 | (22, 2) | 0 | 6 | 26 | 2.1e-05 |

Speedup: x1.03.

| attempt | outcome | fhe() time (ms) | note |
|---:|---|---:|---|
| 1 | ok | 1313.86 | kernel 51a2b4c799 |
| 2 | ok | 1313.36 | kernel 8dcc2456cb |
| 3 | ok | 1311.30 | new best |
| 4 | ok | 1276.17 | new best |
| 5 | ok | 1298.39 | kernel 8dcc2456cb |
| 6 | ok | 1308.07 | kernel 8dcc2456cb |
| 7 | ok | 1302.07 | kernel 8dcc2456cb |
| 8 | ok | 1381.64 | kernel 8dcc2456cb |
