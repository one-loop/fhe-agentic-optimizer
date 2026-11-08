# chebyshev_gelu_d7: LLM optimization run 20261025-161756

Model `claude-sonnet-5-5` (effort medium), 8 attempts, wall time 451 s, approximate API cost $0.0000.

| | fhe() time (ms) | levels | (depth, mult. depth) | ct-ct mul | ct-pt mul | rotations | max error |
|---|---:|---:|---|---:|---:|---:|---:|
| baseline | 300.48 | 7 | (19, 7) | 5 | 5 | 0 | 9.5e-08 |
| best (attempt 4) | 219.53 | 4 | (9, 4) | 3 | 4 | 0 | 1.1e-07 |

Speedup: x1.41 (baseline and best timed in 7 alternating batches; 1.00 when no candidate was confirmed faster).

| attempt | outcome | fhe() time (ms) | verdict | note |
|---:|---|---:|---|---|
| 1 | ok | 227.62 | new_best, x1.31 vs attempt 0 |  |
| 2 | ok | 220.31 | new_best, x1.04 vs attempt 1 |  |
| 3 | ok | 265.97 | slower |  |
| 4 | ok | 219.53 | new_best, x1.03 vs attempt 2 |  |
| 5 | ok | 216.75 | not_confirmed, x1.00 vs attempt 4 |  |
| 6 | ok | 212.02 | not_confirmed, x1.01 vs attempt 4 |  |
| 7 | ok | - | same_as_best (attempt 4) |  |
| 8 | ok | - | duplicate (attempt 6) |  |
