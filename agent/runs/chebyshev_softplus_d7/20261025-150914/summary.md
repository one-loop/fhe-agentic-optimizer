# chebyshev_softplus_d7: LLM optimization run 20261025-150914

Model `claude-sonnet-5-5` (effort medium), 8 attempts, wall time 331 s, approximate API cost $0.0000.

| | fhe() time (ms) | levels | (depth, mult. depth) | ct-ct mul | ct-pt mul | rotations | max error |
|---|---:|---:|---|---:|---:|---:|---:|
| baseline | 301.13 | 7 | (19, 7) | 5 | 5 | 0 | 9.1e-08 |
| best (attempt 2) | 210.38 | 4 | (10, 4) | 3 | 4 | 0 | 1.1e-07 |

Speedup: x1.42 (baseline and best timed in 7 alternating batches; 1.00 when no candidate was confirmed faster).

| attempt | outcome | fhe() time (ms) | verdict | note |
|---:|---|---:|---|---|
| 1 | ok | 229.53 | new_best, x1.30 vs attempt 0 |  |
| 2 | ok | 210.38 | new_best, x1.08 vs attempt 1 |  |
| 3 | ok | 219.36 | slower |  |
| 4 | ok | 211.91 | slower |  |
| 5 | ok | 214.97 | slower |  |
| 6 | ok | - | same_as_best (attempt 2) |  |
| 7 | ok | 215.56 | slower |  |
| 8 | ok | 213.36 | slower |  |
