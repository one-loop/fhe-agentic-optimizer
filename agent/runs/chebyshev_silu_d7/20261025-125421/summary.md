# chebyshev_silu_d7: LLM optimization run 20261025-125421

Model `claude-sonnet-5-5` (effort medium), 8 attempts, wall time 364 s, approximate API cost $0.0000.

| | fhe() time (ms) | levels | (depth, mult. depth) | ct-ct mul | ct-pt mul | rotations | max error |
|---|---:|---:|---|---:|---:|---:|---:|
| baseline | 304.29 | 7 | (19, 7) | 5 | 5 | 0 | 8.1e-08 |
| best (attempt 2) | 212.70 | 4 | (10, 4) | 3 | 4 | 0 | 1e-07 |

Speedup: x1.43 (baseline and best timed in 7 alternating batches; 1.00 when no candidate was confirmed faster).

| attempt | outcome | fhe() time (ms) | verdict | note |
|---:|---|---:|---|---|
| 1 | ok | 230.67 | new_best, x1.32 vs attempt 0 |  |
| 2 | ok | 212.70 | new_best, x1.09 vs attempt 1 |  |
| 3 | ok | 215.22 | slower |  |
| 4 | ok | 220.93 | slower |  |
| 5 | ok | - | same_as_best (attempt 2) |  |
| 6 | ok | 214.50 | slower |  |
| 7 | ok | - | same_as_best (attempt 2) |  |
| 8 | ok | 218.22 | slower |  |
