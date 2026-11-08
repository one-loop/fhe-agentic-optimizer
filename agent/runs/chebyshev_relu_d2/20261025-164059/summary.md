# chebyshev_relu_d2: LLM optimization run 20261025-164059

Model `claude-sonnet-5-5` (effort medium), 8 attempts, wall time 260 s, approximate API cost $0.0000.

| | fhe() time (ms) | levels | (depth, mult. depth) | ct-ct mul | ct-pt mul | rotations | max error |
|---|---:|---:|---|---:|---:|---:|---:|
| baseline | 104.12 | 3 | (7, 3) | 1 | 3 | 0 | 1.6e-07 |
| best (attempt 1) | 81.17 | 3 | (7, 3) | 1 | 2 | 0 | 6.1e-08 |

Speedup: x1.28 (baseline and best timed in 7 alternating batches; 1.00 when no candidate was confirmed faster).

| attempt | outcome | fhe() time (ms) | verdict | note |
|---:|---|---:|---|---|
| 1 | ok | 81.17 | new_best, x1.28 vs attempt 0 |  |
| 2 | ok | 109.61 | slower |  |
| 3 | ok | 102.98 | slower |  |
| 4 | ok | - | same_as_best (attempt 1) |  |
| 5 | ok | 116.15 | slower |  |
| 6 | ok | - | same_as_best (attempt 1) |  |
| 7 | ok | 81.78 | slower |  |
| 8 | ok | - | same_as_best (attempt 1) |  |
