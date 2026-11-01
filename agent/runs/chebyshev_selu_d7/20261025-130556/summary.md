# chebyshev_selu_d7: LLM optimization run 20261025-130556

Model `claude-sonnet-5-5` (effort medium), 8 attempts, wall time 383 s, approximate API cost $0.0000.

| | fhe() time (ms) | levels | (depth, mult. depth) | ct-ct mul | ct-pt mul | rotations | max error |
|---|---:|---:|---|---:|---:|---:|---:|
| baseline | 182.07 | 8 | (22, 8) | 6 | 8 | 0 | 1.4e-07 |
| best (attempt 5) | 143.49 | 5 | (12, 5) | 4 | 5 | 0 | 1.4e-07 |

Speedup: x1.27 (baseline and best timed in 7 alternating batches; 1.00 when no candidate was confirmed faster).

| attempt | outcome | fhe() time (ms) | verdict | note |
|---:|---|---:|---|---|
| 1 | ok | 178.69 | not_confirmed, x1.02 vs attempt 0 |  |
| 2 | ok | 179.24 | not_confirmed, x1.02 vs attempt 0 |  |
| 3 | ok | - | duplicate (attempt 1) |  |
| 4 | ok | 171.58 | new_best, x1.06 vs attempt 0 |  |
| 5 | ok | 143.49 | new_best, x1.20 vs attempt 4 |  |
| 6 | chehab_error | - | failed | backend does not support plain plain evaluation |
| 7 | ok | 144.68 | slower |  |
| 8 | ok | - | duplicate (attempt 7) |  |
