# quad: LLM optimization run 20261025-180813

Model `claude-sonnet-5-5` (effort medium), 8 attempts, wall time 174 s, approximate API cost $0.0000.

| | fhe() time (ms) | levels | (depth, mult. depth) | ct-ct mul | ct-pt mul | rotations | max error |
|---|---:|---:|---|---:|---:|---:|---:|
| baseline | 65.95 | 1 | (1, 1) | 1 | 0 | 0 | 8.2e-08 |
| best (attempt 0) | 65.95 | 1 | (1, 1) | 1 | 0 | 0 | 8.2e-08 |

Speedup: x1.00 (baseline and best timed in 7 alternating batches; 1.00 when no candidate was confirmed faster).

| attempt | outcome | fhe() time (ms) | verdict | note |
|---:|---|---:|---|---|
| 1 | ok | 67.70 | slower |  |
| 2 | ok | 65.34 | not_confirmed, x0.99 vs attempt 0 |  |
| 3 | ok | - | same_as_baseline |  |
| 4 | ok | - | duplicate (attempt 2) |  |
| 5 | ok | - | same_as_baseline |  |
| 6 | ok | - | same_as_baseline |  |
| 7 | ok | - | same_as_baseline |  |
| 8 | ok | - | duplicate (attempt 1) |  |
