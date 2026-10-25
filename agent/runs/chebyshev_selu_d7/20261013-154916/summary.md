# chebyshev_selu_d7: LLM optimization run 20261013-154916

Model `claude-haiku-5-5` (effort medium), 8 attempts, wall time 228 s, approximate API cost $0.0112.

| | fhe() time (ms) | levels | (depth, mult. depth) | ct-ct mul | ct-pt mul | rotations | max error |
|---|---:|---:|---|---:|---:|---:|---:|
| baseline | 360.66 | 8 | (22, 8) | 6 | 8 | 0 | 1.1e-07 |
| best (attempt 4) | 346.39 | 5 | (14, 5) | 4 | 9 | 0 | 1.8e-07 |

Speedup: x1.04.

| attempt | outcome | fhe() time (ms) | note |
|---:|---|---:|---|
| 1 | wrong_output | 312.66 | wrong result on case fixed (9 slots): max abs error 0.948 > tolerance 1e-06. The kernel does not compute the s |
| 2 | ok | 348.76 | new best |
| 3 | ok | 347.74 | new best |
| 4 | ok | 346.39 | new best |
| 5 | ok | 346.74 | kernel d679ffd1a8 |
| 6 | ok | 353.48 | kernel d679ffd1a8 |
| 7 | ok | 359.23 | kernel d679ffd1a8 |
| 8 | ok | 356.61 | kernel d679ffd1a8 |
