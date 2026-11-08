# chebyshev_relu_d2: LLM optimization run 20261025-143140

Model `claude-sonnet-5-5` (effort medium), 8 attempts, wall time 279 s, approximate API cost $0.0000.

| | fhe() time (ms) | levels | (depth, mult. depth) | ct-ct mul | ct-pt mul | rotations | max error |
|---|---:|---:|---|---:|---:|---:|---:|
| baseline | 104.34 | 3 | (7, 3) | 1 | 3 | 0 | 9.3e-08 |
| best (attempt 1) | 81.44 | 3 | (7, 3) | 1 | 2 | 0 | 1.5e-07 |

Speedup: x1.28 (baseline and best timed in 7 alternating batches; 1.00 when no candidate was confirmed faster).

| attempt | outcome | fhe() time (ms) | verdict | note |
|---:|---|---:|---|---|
| 1 | ok | 81.44 | new_best, x1.28 vs attempt 0 |  |
| 2 | ok | - | same_as_best (attempt 1) |  |
| 3 | ok | - | same_as_best (attempt 1) |  |
| 4 | ok | 96.44 | slower |  |
| 5 | ok | - | same_as_best (attempt 1) |  |
| 6 | ok | 94.64 | slower |  |
| 7 | ok | - | same_as_best (attempt 1) |  |
| 8 | ok | - | same_as_best (attempt 1) |  |
