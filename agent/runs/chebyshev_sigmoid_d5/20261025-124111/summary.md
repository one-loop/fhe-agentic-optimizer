# chebyshev_sigmoid_d5: LLM optimization run 20261025-124111

Model `claude-sonnet-5-5` (effort medium), 8 attempts, wall time 404 s, approximate API cost $0.0000.

| | fhe() time (ms) | levels | (depth, mult. depth) | ct-ct mul | ct-pt mul | rotations | max error |
|---|---:|---:|---|---:|---:|---:|---:|
| baseline | 257.73 | 6 | (16, 6) | 4 | 4 | 0 | 3.3e-08 |
| best (attempt 3) | 176.52 | 5 | (16, 5) | 3 | 2 | 0 | 3.4e-08 |

Speedup: x1.48 (baseline and best timed in 7 alternating batches; 1.00 when no candidate was confirmed faster).

| attempt | outcome | fhe() time (ms) | verdict | note |
|---:|---|---:|---|---|
| 1 | ok | 203.56 | new_best, x1.26 vs attempt 0 |  |
| 2 | ok | 225.32 | slower |  |
| 3 | ok | 176.52 | new_best, x1.17 vs attempt 1 |  |
| 4 | ok | 170.52 | not_confirmed, x1.03 vs attempt 3 |  |
| 5 | ok | - | duplicate (attempt 4) |  |
| 6 | ok | 226.13 | slower |  |
| 7 | ok | 178.78 | slower |  |
| 8 | ok | 174.01 | not_confirmed, x1.00 vs attempt 3 |  |
