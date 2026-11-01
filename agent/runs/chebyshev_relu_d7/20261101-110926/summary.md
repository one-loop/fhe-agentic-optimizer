# chebyshev_relu_d7: LLM optimization run 20261101-110926

Model `claude-opus-5-5` (effort medium), 8 attempts, wall time 487 s, approximate API cost $0.0000.

| | fhe() time (ms) | levels | (depth, mult. depth) | ct-ct mul | ct-pt mul | rotations | max error |
|---|---:|---:|---|---:|---:|---:|---:|
| baseline | 152.63 | 7 | (19, 7) | 5 | 5 | 0 | 1.5e-07 |
| best (attempt 1) | 107.81 | 4 | (13, 4) | 3 | 4 | 0 | 1.4e-07 |

Speedup: x1.41 (baseline and best timed in 7 alternating batches; 1.00 when no candidate was confirmed faster).

| attempt | outcome | fhe() time (ms) | verdict | note |
|---:|---|---:|---|---|
| 1 | ok | 107.81 | new_best, x1.41 vs attempt 0 |  |
| 2 | ok | 107.83 | slower |  |
| 3 | ok | 107.59 | not_confirmed, x1.00 vs attempt 1 |  |
| 4 | ok | 107.08 | not_confirmed, x1.01 vs attempt 1 |  |
| 5 | ok | 105.82 | not_confirmed, x1.02 vs attempt 1 |  |
| 6 | ok | 105.64 | not_confirmed, x1.02 vs attempt 1 |  |
| 7 | ok | - | duplicate (attempt 5) |  |
| 8 | ok | 106.51 | not_confirmed, x1.02 vs attempt 1 |  |
