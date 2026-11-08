# chebyshev_relu_d7: LLM optimization run 20261025-122731

Model `claude-sonnet-5-5` (effort medium), 8 attempts, wall time 404 s, approximate API cost $0.0000.

| | fhe() time (ms) | levels | (depth, mult. depth) | ct-ct mul | ct-pt mul | rotations | max error |
|---|---:|---:|---|---:|---:|---:|---:|
| baseline | 303.21 | 7 | (19, 7) | 5 | 5 | 0 | 1.1e-07 |
| best (attempt 2) | 215.18 | 4 | (10, 4) | 3 | 4 | 0 | 9.2e-08 |

Speedup: x1.41 (baseline and best timed in 7 alternating batches; 1.00 when no candidate was confirmed faster).

| attempt | outcome | fhe() time (ms) | verdict | note |
|---:|---|---:|---|---|
| 1 | ok | 229.23 | new_best, x1.31 vs attempt 0 |  |
| 2 | ok | 215.18 | new_best, x1.08 vs attempt 1 |  |
| 3 | ok | 219.73 | slower |  |
| 4 | ok | 222.98 | slower |  |
| 5 | ok | 232.07 | slower |  |
| 6 | ok | 217.47 | slower |  |
| 7 | ok | 221.91 | slower |  |
| 8 | ok | 214.72 | not_confirmed, x1.00 vs attempt 2 |  |
