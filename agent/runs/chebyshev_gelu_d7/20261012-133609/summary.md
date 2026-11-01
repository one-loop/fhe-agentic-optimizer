# chebyshev_gelu_d7: LLM optimization run 20261012-133609

Model `claude-haiku-5-5` (effort medium), 8 attempts, wall time 136 s, approximate API cost $0.0082.

| | fhe() time (ms) | levels | (depth, mult. depth) | ct-ct mul | ct-pt mul | rotations | max error |
|---|---:|---:|---|---:|---:|---:|---:|
| baseline | 150.21 | 7 | (19, 7) | 5 | 5 | 0 | 9.1e-08 |
| best (attempt 2) | 115.00 | 5 | (13, 5) | 3 | 5 | 0 | 1e-07 |

Speedup: x1.31.

| attempt | outcome | fhe() time (ms) | note |
|---:|---|---:|---|
| 1 | ok | 139.87 | new best |
| 2 | ok | 115.00 | new best |
| 3 | ok | 116.24 | kernel 5c460681cb |
| 4 | ok | 116.11 | kernel 5c460681cb |
| 5 | ok | 117.64 | kernel 5c460681cb |
| 6 | ok | 115.86 | kernel 5c460681cb |
| 7 | ok | 116.01 | kernel 5c460681cb |
| 8 | ok | 115.78 | kernel 5c460681cb |
