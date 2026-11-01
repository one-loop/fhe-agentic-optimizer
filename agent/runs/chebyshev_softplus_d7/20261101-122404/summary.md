# chebyshev_softplus_d7: LLM optimization run 20261101-122404

Model `claude-opus-5-5` (effort medium), 8 attempts, wall time 455 s, approximate API cost $0.0000.

| | fhe() time (ms) | levels | (depth, mult. depth) | ct-ct mul | ct-pt mul | rotations | max error |
|---|---:|---:|---|---:|---:|---:|---:|
| baseline | 152.00 | 7 | (19, 7) | 5 | 5 | 0 | 6.7e-08 |
| best (attempt 5) | 99.50 | 4 | (11, 4) | 3 | 3 | 0 | 1.4e-07 |

Speedup: x1.52 (baseline and best timed in 7 alternating batches; 1.00 when no candidate was confirmed faster).

| attempt | outcome | fhe() time (ms) | verdict | note |
|---:|---|---:|---|---|
| 1 | ok | 107.80 | new_best, x1.41 vs attempt 0 |  |
| 2 | ok | 106.03 | not_confirmed, x1.01 vs attempt 1 |  |
| 3 | ok | 105.31 | not_confirmed, x1.02 vs attempt 1 |  |
| 4 | ok | 105.80 | not_confirmed, x1.01 vs attempt 1 |  |
| 5 | ok | 99.50 | new_best, x1.07 vs attempt 1 |  |
| 6 | ok | 101.43 | slower |  |
| 7 | ok | 99.73 | slower |  |
| 8 | ok | 101.12 | slower |  |
