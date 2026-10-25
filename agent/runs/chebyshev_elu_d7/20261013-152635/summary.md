# chebyshev_elu_d7: LLM optimization run 20261013-152635

Model `claude-haiku-5-5` (effort medium), 8 attempts, wall time 269 s, approximate API cost $0.0134.

| | fhe() time (ms) | levels | (depth, mult. depth) | ct-ct mul | ct-pt mul | rotations | max error |
|---|---:|---:|---|---:|---:|---:|---:|
| baseline | 360.63 | 8 | (22, 8) | 6 | 8 | 0 | 1.1e-07 |
| best (attempt 3) | 335.27 | 5 | (13, 5) | 4 | 9 | 0 | 1.2e-07 |

Speedup: x1.08.

| attempt | outcome | fhe() time (ms) | note |
|---:|---|---:|---|
| 1 | ok | 345.14 | new best |
| 2 | ok | 354.51 | kernel 3a64bc6ed9 |
| 3 | ok | 335.27 | new best |
| 4 | ok | 360.06 | kernel d270cac4fe |
| 5 | ok | 337.61 | kernel 6b9bebd633 |
| 6 | ok | 428.71 | kernel 6b9bebd633 |
| 7 | ok | 342.13 | kernel 6b9bebd633 |
| 8 | ok | 337.47 | kernel 6b9bebd633 |
