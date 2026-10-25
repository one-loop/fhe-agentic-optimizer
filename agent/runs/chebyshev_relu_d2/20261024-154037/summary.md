# chebyshev_relu_d2: LLM optimization run 20261024-154037

Model `claude-sonnet-5-5` (effort medium), 8 attempts, wall time 261 s, approximate API cost $0.0000.

| | fhe() time (ms) | levels | (depth, mult. depth) | ct-ct mul | ct-pt mul | rotations | max error |
|---|---:|---:|---|---:|---:|---:|---:|
| baseline | 103.62 | 3 | (7, 3) | 1 | 3 | 0 | 7.4e-08 |
| best (attempt 1) | 82.33 | 3 | (7, 3) | 1 | 2 | 0 | 8.6e-08 |

Speedup: x1.28 (baseline and best timed in 7 alternating batches; 1.00 when no candidate was confirmed faster).

| attempt | outcome | fhe() time (ms) | verdict | note |
|---:|---|---:|---|---|
| 1 | ok | 82.33 | new_best, x1.28 vs attempt 0 |  |
| 2 | ok | - | same_as_best (attempt 1) |  |
| 3 | ok | 103.00 | slower |  |
| 4 | ok | - | same_as_best (attempt 1) |  |
| 5 | ok | - | same_as_best (attempt 1) |  |
| 6 | ok | 109.72 | slower |  |
| 7 | ok | - | same_as_best (attempt 1) |  |
| 8 | ok | - | same_as_best (attempt 1) |  |
