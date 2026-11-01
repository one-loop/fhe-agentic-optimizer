# chebyshev_relu_d2: LLM optimization run 20261025-125656

Model `claude-sonnet-5-5` (effort medium), 8 attempts, wall time 234 s, approximate API cost $0.0000.

| | fhe() time (ms) | levels | (depth, mult. depth) | ct-ct mul | ct-pt mul | rotations | max error |
|---|---:|---:|---|---:|---:|---:|---:|
| baseline | 53.79 | 3 | (7, 3) | 1 | 3 | 0 | 9.3e-08 |
| best (attempt 1) | 41.54 | 3 | (7, 3) | 1 | 2 | 0 | 9.1e-08 |

Speedup: x1.29 (baseline and best timed in 7 alternating batches; 1.00 when no candidate was confirmed faster).

| attempt | outcome | fhe() time (ms) | verdict | note |
|---:|---|---:|---|---|
| 1 | ok | 41.54 | new_best, x1.29 vs attempt 0 |  |
| 2 | ok | 49.80 | slower |  |
| 3 | ok | - | same_as_baseline |  |
| 4 | ok | 49.73 | slower |  |
| 5 | ok | - | same_as_best (attempt 1) |  |
| 6 | ok | 52.74 | slower |  |
| 7 | ok | - | same_as_best (attempt 1) |  |
| 8 | ok | - | same_as_baseline |  |
