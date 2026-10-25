# table_mult: LLM optimization run 20261013-164345

Model `claude-haiku-5-5` (effort medium), 8 attempts, wall time 448 s, approximate API cost $0.0190.

| | fhe() time (ms) | levels | (depth, mult. depth) | ct-ct mul | ct-pt mul | rotations | max error |
|---|---:|---:|---|---:|---:|---:|---:|
| baseline | 1266.12 | 1 | (17, 1) | 0 | 3 | 24 | 4e-05 |
| best (attempt 7) | 1172.19 | 1 | (17, 1) | 0 | 3 | 24 | 2.9e-05 |

Speedup: x1.08.

| attempt | outcome | fhe() time (ms) | note |
|---:|---|---:|---|
| 1 | ok | 1203.04 | new best |
| 2 | ok | 1203.90 | kernel be02c2df33 |
| 3 | ok | 1250.76 | kernel d267bea6c6 |
| 4 | ok | 1172.21 | new best |
| 5 | ok | 1262.89 | kernel ce20382645 |
| 6 | ok | 1182.08 | kernel ce20382645 |
| 7 | ok | 1172.19 | new best |
| 8 | ok | 1182.73 | kernel ce20382645 |
