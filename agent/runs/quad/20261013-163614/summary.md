# quad: LLM optimization run 20261013-163614

Model `claude-haiku-5-5` (effort medium), 8 attempts, wall time 125 s, approximate API cost $0.0021.

| | fhe() time (ms) | levels | (depth, mult. depth) | ct-ct mul | ct-pt mul | rotations | max error |
|---|---:|---:|---|---:|---:|---:|---:|
| baseline | 64.70 | 1 | (1, 1) | 1 | 0 | 0 | 7e-08 |
| best (attempt 6) | 63.71 | 1 | (1, 1) | 0 | 0 | 0 | 1.1e-07 |

Speedup: x1.02.

| attempt | outcome | fhe() time (ms) | note |
|---:|---|---:|---|
| 1 | ok | 63.80 | new best |
| 2 | ok | 64.01 | kernel 3b349cc462 |
| 3 | ok | 63.84 | kernel 3a3162f7d8 |
| 4 | ok | 64.14 | kernel 3a3162f7d8 |
| 5 | ok | 64.65 | kernel 3a3162f7d8 |
| 6 | ok | 63.71 | new best |
| 7 | ok | 64.04 | kernel 3a3162f7d8 |
| 8 | ok | 64.76 | kernel 3a3162f7d8 |
