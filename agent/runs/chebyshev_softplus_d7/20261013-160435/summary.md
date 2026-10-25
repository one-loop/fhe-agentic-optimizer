# chebyshev_softplus_d7: LLM optimization run 20261013-160435

Model `claude-haiku-5-5` (effort medium), 8 attempts, wall time 213 s, approximate API cost $0.0119.

| | fhe() time (ms) | levels | (depth, mult. depth) | ct-ct mul | ct-pt mul | rotations | max error |
|---|---:|---:|---|---:|---:|---:|---:|
| baseline | 303.82 | 7 | (19, 7) | 5 | 5 | 0 | 1e-07 |
| best (attempt 4) | 218.66 | 4 | (12, 4) | 3 | 4 | 0 | 1.1e-07 |

Speedup: x1.39.

| attempt | outcome | fhe() time (ms) | note |
|---:|---|---:|---|
| 1 | wrong_output | 261.13 | wrong result on case dense (256 slots): max abs error 1.46e-06 > tolerance 1e-06. The kernel does not compute  |
| 2 | ok | 229.34 | new best |
| 3 | ok | 219.71 | new best |
| 4 | ok | 218.66 | new best |
| 5 | ok | 219.06 | kernel 55e29f10c1 |
| 6 | ok | 218.88 | kernel 55e29f10c1 |
| 7 | ok | 219.89 | kernel 55e29f10c1 |
| 8 | ok | 218.67 | kernel 55e29f10c1 |
