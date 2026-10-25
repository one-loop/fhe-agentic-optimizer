# chebyshev_hardshrink_d7: LLM optimization run 20261013-153421

Model `claude-haiku-5-5` (effort medium), 8 attempts, wall time 231 s, approximate API cost $0.0112.

| | fhe() time (ms) | levels | (depth, mult. depth) | ct-ct mul | ct-pt mul | rotations | max error |
|---|---:|---:|---|---:|---:|---:|---:|
| baseline | 319.82 | 8 | (21, 8) | 6 | 5 | 0 | 1.2e-07 |
| best (attempt 1) | 284.01 | 5 | (14, 5) | 4 | 6 | 0 | 9.7e-08 |

Speedup: x1.13.

| attempt | outcome | fhe() time (ms) | note |
|---:|---|---:|---|
| 1 | ok | 284.01 | new best |
| 2 | ok | 319.70 | kernel 3d03d0dce0 |
| 3 | ok | 296.18 | kernel 0759e94bd4 |
| 4 | ok | 287.57 | kernel 0759e94bd4 |
| 5 | ok | 285.63 | kernel 0759e94bd4 |
| 6 | ok | 296.92 | kernel 0759e94bd4 |
| 7 | ok | 355.63 | kernel 0759e94bd4 |
| 8 | ok | 298.20 | kernel 0759e94bd4 |
