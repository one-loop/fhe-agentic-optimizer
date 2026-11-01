# chebyshev_gelu_d7: LLM optimization run 20261101-103803

Model `claude-opus-5-5` (effort medium), 8 attempts, wall time 465 s, approximate API cost $0.0000.

| | fhe() time (ms) | levels | (depth, mult. depth) | ct-ct mul | ct-pt mul | rotations | max error |
|---|---:|---:|---|---:|---:|---:|---:|
| baseline | 152.45 | 7 | (19, 7) | 5 | 5 | 0 | 1.1e-07 |
| best (attempt 1) | 108.60 | 4 | (9, 4) | 3 | 4 | 0 | 1.3e-07 |

Speedup: x1.41 (baseline and best timed in 7 alternating batches; 1.00 when no candidate was confirmed faster).

| attempt | outcome | fhe() time (ms) | verdict | note |
|---:|---|---:|---|---|
| 1 | ok | 108.60 | new_best, x1.41 vs attempt 0 |  |
| 2 | ok | 108.19 | not_confirmed, x1.00 vs attempt 1 |  |
| 3 | ok | 108.05 | not_confirmed, x1.00 vs attempt 1 |  |
| 4 | ok | 107.65 | not_confirmed, x1.01 vs attempt 1 |  |
| 5 | ok | 107.27 | not_confirmed, x1.01 vs attempt 1 |  |
| 6 | ok | 105.30 | not_confirmed, x1.02 vs attempt 1 |  |
| 7 | ok | 105.56 | not_confirmed, x1.02 vs attempt 1 |  |
| 8 | ok | - | duplicate (attempt 6) |  |
