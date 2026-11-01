# chebyshev_hardshrink_d7: LLM optimization run 20261101-104548

Model `claude-opus-5-5` (effort medium), 8 attempts, wall time 493 s, approximate API cost $0.0000.

| | fhe() time (ms) | levels | (depth, mult. depth) | ct-ct mul | ct-pt mul | rotations | max error |
|---|---:|---:|---|---:|---:|---:|---:|
| baseline | 161.07 | 8 | (21, 8) | 6 | 5 | 0 | 9.8e-08 |
| best (attempt 8) | 99.77 | 6 | (16, 6) | 4 | 2 | 0 | 1.5e-07 |

Speedup: x1.62 (baseline and best timed in 7 alternating batches; 1.00 when no candidate was confirmed faster).

| attempt | outcome | fhe() time (ms) | verdict | note |
|---:|---|---:|---|---|
| 1 | ok | 139.81 | new_best, x1.15 vs attempt 0 |  |
| 2 | ok | 110.77 | new_best, x1.26 vs attempt 1 |  |
| 3 | ok | 103.37 | new_best, x1.08 vs attempt 2 |  |
| 4 | ok | 126.55 | slower |  |
| 5 | ok | 100.17 | not_confirmed, x1.03 vs attempt 3 |  |
| 6 | ok | 100.28 | not_confirmed, x1.03 vs attempt 3 |  |
| 7 | ok | 101.19 | not_confirmed, x1.02 vs attempt 3 |  |
| 8 | ok | 99.77 | new_best, x1.03 vs attempt 3 |  |
