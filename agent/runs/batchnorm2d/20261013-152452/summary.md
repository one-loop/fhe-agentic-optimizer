# batchnorm2d: LLM optimization run 20261013-152452

Model `claude-haiku-5-5` (effort medium), 8 attempts, wall time 104 s, approximate API cost $0.0021.

| | fhe() time (ms) | levels | (depth, mult. depth) | ct-ct mul | ct-pt mul | rotations | max error |
|---|---:|---:|---|---:|---:|---:|---:|
| baseline | 19.28 | 1 | (2, 1) | 0 | 1 | 0 | 2.5e-08 |
| best (attempt 7) | 18.05 | 1 | (2, 1) | 0 | 1 | 0 | 2.1e-08 |

Speedup: x1.07.

| attempt | outcome | fhe() time (ms) | note |
|---:|---|---:|---|
| 1 | ok | 18.16 | new best |
| 2 | ok | 18.20 | kernel 8b2bd54e81 |
| 3 | ok | 18.06 | new best |
| 4 | ok | 18.83 | kernel 8b2bd54e81 |
| 5 | ok | 18.07 | kernel 8b2bd54e81 |
| 6 | ok | 18.26 | kernel 8b2bd54e81 |
| 7 | ok | 18.05 | new best |
| 8 | ok | 18.06 | kernel 8b2bd54e81 |
