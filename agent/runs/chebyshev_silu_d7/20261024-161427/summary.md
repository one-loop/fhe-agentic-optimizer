# chebyshev_silu_d7: LLM optimization run 20261024-161427

Model `claude-sonnet-5-5` (effort medium), 8 attempts, wall time 406 s, approximate API cost $0.0000.

| | fhe() time (ms) | levels | (depth, mult. depth) | ct-ct mul | ct-pt mul | rotations | max error |
|---|---:|---:|---|---:|---:|---:|---:|
| baseline | 300.04 | 7 | (19, 7) | 5 | 5 | 0 | 9.3e-08 |
| best (attempt 3) | 205.16 | 5 | (14, 5) | 3 | 4 | 0 | 9.4e-08 |

Speedup: x1.46 (baseline and best timed in 7 alternating batches; 1.00 when no candidate was confirmed faster).

| attempt | outcome | fhe() time (ms) | verdict | note |
|---:|---|---:|---|---|
| 1 | ok | 228.90 | new_best, x1.31 vs attempt 0 |  |
| 2 | ok | 210.84 | new_best, x1.08 vs attempt 1 |  |
| 3 | ok | 205.16 | new_best, x1.03 vs attempt 2 |  |
| 4 | ok | 205.91 | slower |  |
| 5 | ok | 204.46 | not_confirmed, x1.01 vs attempt 3 |  |
| 6 | ok | 204.37 | not_confirmed, x1.00 vs attempt 3 |  |
| 7 | ok | - | duplicate (attempt 4) |  |
| 8 | ok | 210.19 | slower |  |
