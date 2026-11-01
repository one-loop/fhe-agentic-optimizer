# extract_sparse: LLM optimization run 20261025-135942

Model `claude-sonnet-5-5` (effort medium), 8 attempts, wall time 220 s, approximate API cost $0.0000.

| | fhe() time (ms) | levels | (depth, mult. depth) | ct-ct mul | ct-pt mul | rotations | max error |
|---|---:|---:|---|---:|---:|---:|---:|
| baseline | 87.88 | 1 | (3, 1) | 0 | 3 | 3 | 1.1e-05 |
| best (attempt 1) | 63.55 | 1 | (3, 1) | 0 | 2 | 2 | 6.8e-06 |

Speedup: x1.38 (baseline and best timed in 7 alternating batches; 1.00 when no candidate was confirmed faster).

| attempt | outcome | fhe() time (ms) | verdict | note |
|---:|---|---:|---|---|
| 1 | ok | 63.55 | new_best, x1.38 vs attempt 0 |  |
| 2 | ok | 64.04 | slower |  |
| 3 | ok | - | duplicate (attempt 2) |  |
| 4 | ok | - | duplicate (attempt 2) |  |
| 5 | ok | 65.80 | slower |  |
| 6 | ok | - | same_as_best (attempt 1) |  |
| 7 | ok | - | same_as_best (attempt 1) |  |
| 8 | ok | - | same_as_best (attempt 1) |  |
