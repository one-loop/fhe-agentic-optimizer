# chebyshev_softplus_d7: LLM optimization run 20261025-171630

Model `claude-sonnet-5-5` (effort medium), 8 attempts, wall time 562 s, approximate API cost $0.0000.

| | fhe() time (ms) | levels | (depth, mult. depth) | ct-ct mul | ct-pt mul | rotations | max error |
|---|---:|---:|---|---:|---:|---:|---:|
| baseline | 301.50 | 7 | (19, 7) | 5 | 5 | 0 | 7.4e-08 |
| best (attempt 5) | 219.68 | 4 | (11, 4) | 3 | 4 | 0 | 7.2e-08 |

Speedup: x1.42 (baseline and best timed in 7 alternating batches; 1.00 when no candidate was confirmed faster).

| attempt | outcome | fhe() time (ms) | verdict | note |
|---:|---|---:|---|---|
| 1 | ok | 228.19 | new_best, x1.33 vs attempt 0 |  |
| 2 | ok | 227.15 | new_best, x1.11 vs attempt 1 |  |
| 3 | ok | 224.50 | not_confirmed, x0.97 vs attempt 2 |  |
| 4 | ok | 226.36 | not_confirmed, x1.03 vs attempt 2 |  |
| 5 | ok | 219.68 | new_best, x1.06 vs attempt 2 |  |
| 6 | ok | 215.34 | not_confirmed, x1.00 vs attempt 5 |  |
| 7 | ok | 211.57 | not_confirmed, x1.01 vs attempt 5 |  |
| 8 | ok | 218.11 | not_confirmed, x1.03 vs attempt 5 |  |
