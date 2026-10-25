# chebyshev_silu_d7: LLM optimization run 20261013-160122

Model `claude-haiku-5-5` (effort medium), 8 attempts, wall time 193 s, approximate API cost $0.0087.

| | fhe() time (ms) | levels | (depth, mult. depth) | ct-ct mul | ct-pt mul | rotations | max error |
|---|---:|---:|---|---:|---:|---:|---:|
| baseline | 303.26 | 7 | (19, 7) | 5 | 5 | 0 | 8.2e-08 |
| best (attempt 2) | 214.25 | 4 | (10, 4) | 3 | 4 | 0 | 1.1e-07 |

Speedup: x1.42.

| attempt | outcome | fhe() time (ms) | note |
|---:|---|---:|---|
| 1 | ok | 231.49 | new best |
| 2 | ok | 214.25 | new best |
| 3 | ok | 214.99 | kernel 3019c5a74b |
| 4 | ok | 217.28 | kernel 3019c5a74b |
| 5 | ok | 216.08 | kernel 3019c5a74b |
| 6 | ok | 216.38 | kernel 3019c5a74b |
| 7 | ok | 214.34 | kernel 3019c5a74b |
| 8 | ok | 215.00 | kernel 3019c5a74b |
