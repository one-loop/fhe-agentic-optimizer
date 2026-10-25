# elementwise_add_scalar: LLM optimization run 20261013-161142

Model `claude-haiku-5-5` (effort medium), 8 attempts, wall time 104 s, approximate API cost $0.0016.

| | fhe() time (ms) | levels | (depth, mult. depth) | ct-ct mul | ct-pt mul | rotations | max error |
|---|---:|---:|---|---:|---:|---:|---:|
| baseline | 0.51 | 0 | (1, 0) | 0 | 0 | 0 | 1.8e-08 |
| best (attempt 3) | 0.39 | 0 | (1, 0) | 0 | 0 | 0 | 1.9e-08 |

Speedup: x1.31.

| attempt | outcome | fhe() time (ms) | note |
|---:|---|---:|---|
| 1 | ok | 0.45 | new best |
| 2 | ok | 0.45 | kernel 16c0c9f13f |
| 3 | ok | 0.39 | new best |
| 4 | ok | 0.43 | kernel 16c0c9f13f |
| 5 | ok | 0.62 | kernel 16c0c9f13f |
| 6 | ok | 0.41 | kernel 16c0c9f13f |
| 7 | ok | 0.43 | kernel 16c0c9f13f |
| 8 | ok | 0.52 | kernel 16c0c9f13f |
