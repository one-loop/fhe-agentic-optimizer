# chebyshev_mish_d7: LLM optimization run 20261024-121104

Model `claude-sonnet-5-5` (effort medium), 8 attempts, wall time 283 s, approximate API cost $0.0000.

| | fhe() time (ms) | levels | (depth, mult. depth) | ct-ct mul | ct-pt mul | rotations | max error |
|---|---:|---:|---|---:|---:|---:|---:|
| baseline | 363.05 | 8 | (22, 8) | 6 | 8 | 0 | 1.2e-07 |
| best (attempt 1) | 353.09 | 5 | (19, 5) | 4 | 9 | 0 | 1.4e-07 |

Speedup: x1.03.

| attempt | outcome | fhe() time (ms) | note |
|---:|---|---:|---|
| 1 | ok | 353.09 | new best |
| 2 | ok | 361.10 | kernel 077fea73ea |
| 3 | ok | 356.28 | kernel facb8179b2 |
| 4 | ok | 355.09 | kernel 3cd94d4844 |
| 5 | ok | 420.24 | kernel e1beec33da |
| 6 | ok | 354.94 | kernel d1964c51f2 |
| 7 | ok | 356.17 | kernel d1964c51f2 |
| 8 | ok | 363.59 | kernel d1964c51f2 |
