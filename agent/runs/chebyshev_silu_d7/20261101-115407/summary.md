# chebyshev_silu_d7: LLM optimization run 20261101-115407

Model `claude-opus-5-5` (effort medium), 8 attempts, wall time 1797 s, approximate API cost $0.0000.

| | fhe() time (ms) | levels | (depth, mult. depth) | ct-ct mul | ct-pt mul | rotations | max error |
|---|---:|---:|---|---:|---:|---:|---:|
| baseline | 152.45 | 7 | (19, 7) | 5 | 5 | 0 | 9.6e-08 |
| best (attempt 2) | 95.94 | 5 | (15, 5) | 3 | 3 | 0 | 1.4e-07 |

Speedup: x1.58 (baseline and best timed in 7 alternating batches; 1.00 when no candidate was confirmed faster).

| attempt | outcome | fhe() time (ms) | verdict | note |
|---:|---|---:|---|---|
| 1 | ok | 107.93 | new_best, x1.41 vs attempt 0 |  |
| 2 | ok | 95.94 | new_best, x1.12 vs attempt 1 |  |
| 3 | ok | 96.34 | slower |  |
| 4 | ok | 95.56 | not_confirmed, x1.00 vs attempt 2 |  |
| 5 | ok | 93.85 | not_confirmed, x1.00 vs attempt 2 |  |
| 6 | ok | 96.59 | slower |  |
| 7 | ok | - | duplicate (attempt 5) |  |
| 8 | ok | 98.87 | slower |  |
