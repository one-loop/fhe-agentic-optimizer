# chebyshev_sigmoid_d5: LLM optimization run 20261025-131219

Model `claude-sonnet-5-5` (effort medium), 8 attempts, wall time 308 s, approximate API cost $0.0000.

| | fhe() time (ms) | levels | (depth, mult. depth) | ct-ct mul | ct-pt mul | rotations | max error |
|---|---:|---:|---|---:|---:|---:|---:|
| baseline | 130.11 | 6 | (16, 6) | 4 | 4 | 0 | 4.6e-08 |
| best (attempt 1) | 110.46 | 5 | (13, 5) | 3 | 6 | 0 | 1.1e-07 |

Speedup: x1.17 (baseline and best timed in 7 alternating batches; 1.00 when no candidate was confirmed faster).

| attempt | outcome | fhe() time (ms) | verdict | note |
|---:|---|---:|---|---|
| 1 | ok | 110.46 | new_best, x1.17 vs attempt 0 |  |
| 2 | ok | 114.33 | slower |  |
| 3 | ok | 133.16 | slower |  |
| 4 | ok | 124.32 | slower |  |
| 5 | ok | 116.87 | slower |  |
| 6 | ok | 109.56 | not_confirmed, x1.00 vs attempt 1 |  |
| 7 | ok | 117.68 | slower |  |
| 8 | ok | 110.18 | not_confirmed, x1.00 vs attempt 1 |  |
