# extract_linear: LLM optimization run 20261013-163132

Model `claude-haiku-5-5` (effort medium), 8 attempts, wall time 116 s, approximate API cost $0.0028.

| | fhe() time (ms) | levels | (depth, mult. depth) | ct-ct mul | ct-pt mul | rotations | max error |
|---|---:|---:|---|---:|---:|---:|---:|
| baseline | 59.11 | 1 | (2, 1) | 0 | 1 | 1 | 3.4e-06 |
| best (attempt 0) | 59.11 | 1 | (2, 1) | 0 | 1 | 1 | 3.4e-06 |

Speedup: x1.00.

| attempt | outcome | fhe() time (ms) | note |
|---:|---|---:|---|
| 1 | ok | 60.12 | kernel 0b310d745c |
| 2 | chehab_error | - | backend does not support plain plain evaluation |
| 3 | ok | 63.48 | kernel 4ef1bf7c33 |
| 4 | ok | 59.61 | kernel 94dca51d9c |
| 5 | ok | 59.50 | kernel 94dca51d9c |
| 6 | ok | 59.51 | kernel 94dca51d9c |
| 7 | ok | 59.45 | kernel 94dca51d9c |
| 8 | ok | 59.29 | kernel 94dca51d9c |
