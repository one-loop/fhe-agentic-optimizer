# table_mult: LLM optimization run 20261024-132332

Model `claude-sonnet-5-5` (effort medium), 8 attempts, wall time 367 s, approximate API cost $0.0000.

| | fhe() time (ms) | levels | (depth, mult. depth) | ct-ct mul | ct-pt mul | rotations | max error |
|---|---:|---:|---|---:|---:|---:|---:|
| baseline | 1230.94 | 1 | (17, 1) | 0 | 3 | 24 | 3.3e-05 |
| best (attempt 5) | 1175.38 | 1 | (17, 1) | 0 | 3 | 24 | 3.9e-05 |

Speedup: x1.05.

| attempt | outcome | fhe() time (ms) | note |
|---:|---|---:|---|
| 1 | ok | 1506.96 | kernel fd4d38a882 |
| 2 | ok | 1372.69 | kernel fd4d38a882 |
| 3 | chehab_error | - | backend does not support plain plain evaluation |
| 4 | ok | 1198.39 | new best |
| 5 | ok | 1175.38 | new best |
| 6 | ok | 1195.24 | kernel fd4d38a882 |
| 7 | ok | 1192.67 | kernel fd4d38a882 |
| 8 | ok | 1188.97 | kernel fd4d38a882 |
