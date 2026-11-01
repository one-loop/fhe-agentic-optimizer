# extract_sparse: LLM optimization run 20261025-122541

Model `claude-sonnet-5-5` (effort medium), 8 attempts, wall time 237 s, approximate API cost $0.0000.

| | fhe() time (ms) | levels | (depth, mult. depth) | ct-ct mul | ct-pt mul | rotations | max error |
|---|---:|---:|---|---:|---:|---:|---:|
| baseline | 87.81 | 1 | (3, 1) | 0 | 3 | 3 | 1.8e-05 |
| best (attempt 2) | 63.47 | 1 | (4, 1) | 0 | 2 | 2 | 7.2e-06 |

Speedup: x1.39 (baseline and best timed in 7 alternating batches; 1.00 when no candidate was confirmed faster).

| attempt | outcome | fhe() time (ms) | verdict | note |
|---:|---|---:|---|---|
| 1 | ok | 65.86 | new_best, x1.34 vs attempt 0 |  |
| 2 | ok | 63.47 | new_best, x1.03 vs attempt 1 |  |
| 3 | ok | - | same_as_best (attempt 2) |  |
| 4 | ok | 64.07 | slower |  |
| 5 | ok | - | same_as_best (attempt 2) |  |
| 6 | ok | - | same_as_best (attempt 2) |  |
| 7 | ok | - | same_as_best (attempt 2) |  |
| 8 | ok | - | same_as_best (attempt 2) |  |
