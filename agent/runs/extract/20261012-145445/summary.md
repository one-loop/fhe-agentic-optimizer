# extract: LLM optimization run 20261012-145445

Model `claude-haiku-5-5` (effort medium), 8 attempts, wall time 90 s, approximate API cost $0.0032.

| | fhe() time (ms) | levels | (depth, mult. depth) | ct-ct mul | ct-pt mul | rotations | max error |
|---|---:|---:|---|---:|---:|---:|---:|
| baseline | 29.17 | 1 | (2, 1) | 0 | 1 | 1 | 6.3e-06 |
| best (attempt 6) | 29.09 | 1 | (2, 1) | 0 | 1 | 1 | 4.7e-06 |

Speedup: x1.00.

| attempt | outcome | fhe() time (ms) | note |
|---:|---|---:|---|
| 1 | ok | 29.64 | kernel 0b02f92aa7 |
| 2 | ok | 29.31 | kernel 34673b818a |
| 3 | ok | 29.47 | kernel be5dd1d2ff |
| 4 | ok | 29.14 | new best |
| 5 | ok | 29.17 | kernel bf3d6a7ff5 |
| 6 | ok | 29.09 | new best |
| 7 | ok | 29.12 | kernel bf3d6a7ff5 |
| 8 | ok | 29.56 | kernel bf3d6a7ff5 |
