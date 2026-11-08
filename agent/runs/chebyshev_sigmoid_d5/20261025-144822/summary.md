# chebyshev_sigmoid_d5: LLM optimization run 20261025-144822

Model `claude-sonnet-5-5` (effort medium), 8 attempts, wall time 365 s, approximate API cost $0.0000.

| | fhe() time (ms) | levels | (depth, mult. depth) | ct-ct mul | ct-pt mul | rotations | max error |
|---|---:|---:|---|---:|---:|---:|---:|
| baseline | 256.75 | 6 | (16, 6) | 4 | 4 | 0 | 4.5e-08 |
| best (attempt 4) | 171.41 | 5 | (13, 5) | 3 | 2 | 0 | 3.8e-08 |

Speedup: x1.50 (baseline and best timed in 7 alternating batches; 1.00 when no candidate was confirmed faster).

| attempt | outcome | fhe() time (ms) | verdict | note |
|---:|---|---:|---|---|
| 1 | ok | 210.59 | new_best, x1.22 vs attempt 0 |  |
| 2 | ok | - | same_as_best (attempt 1) |  |
| 3 | ok | 230.69 | slower |  |
| 4 | ok | 171.41 | new_best, x1.23 vs attempt 1 |  |
| 5 | ok | 218.05 | slower |  |
| 6 | ok | 170.89 | not_confirmed, x1.00 vs attempt 4 |  |
| 7 | ok | - | duplicate (attempt 6) |  |
| 8 | ok | - | duplicate (attempt 6) |  |
