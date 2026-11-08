# extract_sparse: LLM optimization run 20261025-154311

Model `claude-sonnet-5-5` (effort medium), 8 attempts, wall time 295 s, approximate API cost $0.0000.

| | fhe() time (ms) | levels | (depth, mult. depth) | ct-ct mul | ct-pt mul | rotations | max error |
|---|---:|---:|---|---:|---:|---:|---:|
| baseline | 177.80 | 1 | (3, 1) | 0 | 3 | 3 | 1.1e-05 |
| best (attempt 1) | 128.63 | 1 | (3, 1) | 0 | 2 | 2 | 5.8e-06 |

Speedup: x1.39 (baseline and best timed in 7 alternating batches; 1.00 when no candidate was confirmed faster).

| attempt | outcome | fhe() time (ms) | verdict | note |
|---:|---|---:|---|---|
| 1 | ok | 128.63 | new_best, x1.39 vs attempt 0 |  |
| 2 | ok | 131.96 | slower |  |
| 3 | ok | 127.65 | not_confirmed, x1.00 vs attempt 1 |  |
| 4 | ok | - | same_as_best (attempt 1) |  |
| 5 | ok | - | same_as_best (attempt 1) |  |
| 6 | ok | - | same_as_best (attempt 1) |  |
| 7 | ok | - | same_as_best (attempt 1) |  |
| 8 | ok | 137.00 | slower |  |
