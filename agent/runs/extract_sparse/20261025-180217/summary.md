# extract_sparse: LLM optimization run 20261025-180217

Model `claude-sonnet-5-5` (effort medium), 8 attempts, wall time 357 s, approximate API cost $0.0000.

| | fhe() time (ms) | levels | (depth, mult. depth) | ct-ct mul | ct-pt mul | rotations | max error |
|---|---:|---:|---|---:|---:|---:|---:|
| baseline | 220.39 | 1 | (3, 1) | 0 | 3 | 3 | 6.5e-06 |
| best (attempt 1) | 131.97 | 1 | (4, 1) | 0 | 2 | 2 | 6.4e-06 |

Speedup: x1.43 (baseline and best timed in 7 alternating batches; 1.00 when no candidate was confirmed faster).

| attempt | outcome | fhe() time (ms) | verdict | note |
|---:|---|---:|---|---|
| 1 | ok | 131.97 | new_best, x1.38 vs attempt 0 |  |
| 2 | ok | - | same_as_best (attempt 1) |  |
| 3 | ok | 133.59 | slower |  |
| 4 | ok | - | same_as_best (attempt 1) |  |
| 5 | ok | 128.83 | not_confirmed, x1.00 vs attempt 1 |  |
| 6 | ok | - | same_as_best (attempt 1) |  |
| 7 | ok | 186.79 | slower |  |
| 8 | ok | - | same_as_best (attempt 1) |  |
