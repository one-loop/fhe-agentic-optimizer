# quad: LLM optimization run 20261025-134203

Model `claude-sonnet-5-5` (effort medium), 8 attempts, wall time 184 s, approximate API cost $0.0000.

| | fhe() time (ms) | levels | (depth, mult. depth) | ct-ct mul | ct-pt mul | rotations | max error |
|---|---:|---:|---|---:|---:|---:|---:|
| baseline | 64.31 | 1 | (1, 1) | 1 | 0 | 0 | 5.1e-08 |
| best (attempt 0) | 64.31 | 1 | (1, 1) | 1 | 0 | 0 | 5.1e-08 |

Speedup: x1.00 (baseline and best timed in 7 alternating batches; 1.00 when no candidate was confirmed faster).

| attempt | outcome | fhe() time (ms) | verdict | note |
|---:|---|---:|---|---|
| 1 | ok | 64.04 | not_confirmed, x1.01 vs attempt 0 |  |
| 2 | ok | - | duplicate (attempt 1) |  |
| 3 | ok | 128.82 | slower |  |
| 4 | ok | - | duplicate (attempt 1) |  |
| 5 | ok | - | same_as_baseline |  |
| 6 | ok | - | same_as_baseline |  |
| 7 | ok | - | same_as_baseline |  |
| 8 | ok | - | duplicate (attempt 1) |  |
