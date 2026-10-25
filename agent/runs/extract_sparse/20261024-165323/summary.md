# extract_sparse: LLM optimization run 20261024-165323

Model `claude-sonnet-5-5` (effort medium), 8 attempts, wall time 315 s, approximate API cost $0.0000.

| | fhe() time (ms) | levels | (depth, mult. depth) | ct-ct mul | ct-pt mul | rotations | max error |
|---|---:|---:|---|---:|---:|---:|---:|
| baseline | 178.98 | 1 | (3, 1) | 0 | 3 | 3 | 1e-05 |
| best (attempt 5) | 129.28 | 1 | (3, 1) | 0 | 2 | 2 | 1.1e-05 |

Speedup: x1.41 (baseline and best timed in 7 alternating batches; 1.00 when no candidate was confirmed faster).

| attempt | outcome | fhe() time (ms) | verdict | note |
|---:|---|---:|---|---|
| 1 | ok | 138.50 | new_best, x1.35 vs attempt 0 |  |
| 2 | ok | 127.89 | not_confirmed, x1.03 vs attempt 1 |  |
| 3 | ok | - | duplicate (attempt 2) |  |
| 4 | ok | - | duplicate (attempt 2) |  |
| 5 | ok | 129.28 | new_best, x1.03 vs attempt 1 |  |
| 6 | ok | - | same_as_best (attempt 5) |  |
| 7 | ok | - | same_as_best (attempt 5) |  |
| 8 | ok | - | same_as_best (attempt 5) |  |
