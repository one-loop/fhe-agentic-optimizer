# extract_linear: LLM optimization run 20261012-145615

Model `claude-haiku-5-5` (effort medium), 8 attempts, wall time 88 s, approximate API cost $0.0030.

| | fhe() time (ms) | levels | (depth, mult. depth) | ct-ct mul | ct-pt mul | rotations | max error |
|---|---:|---:|---|---:|---:|---:|---:|
| baseline | 29.13 | 1 | (2, 1) | 0 | 1 | 1 | 2.3e-06 |
| best (attempt 7) | 29.10 | 1 | (2, 1) | 0 | 1 | 1 | 2e-06 |

Speedup: x1.00.

| attempt | outcome | fhe() time (ms) | note |
|---:|---|---:|---|
| 1 | ok | 29.11 | new best |
| 2 | ok | 29.12 | kernel 50519a0d20 |
| 3 | ok | 29.42 | kernel baf32b75ba |
| 4 | ok | 29.18 | kernel 50519a0d20 |
| 5 | ok | 29.24 | kernel 50519a0d20 |
| 6 | ok | 29.18 | kernel baf32b75ba |
| 7 | ok | 29.10 | new best |
| 8 | ok | 29.15 | kernel baf32b75ba |
