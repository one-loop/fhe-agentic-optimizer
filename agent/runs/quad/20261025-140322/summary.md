# quad: LLM optimization run 20261025-140322

Model `claude-sonnet-5-5` (effort medium), 8 attempts, wall time 173 s, approximate API cost $0.0000.

| | fhe() time (ms) | levels | (depth, mult. depth) | ct-ct mul | ct-pt mul | rotations | max error |
|---|---:|---:|---|---:|---:|---:|---:|
| baseline | 32.11 | 1 | (1, 1) | 1 | 0 | 0 | 5.1e-08 |
| best (attempt 0) | 32.11 | 1 | (1, 1) | 1 | 0 | 0 | 5.1e-08 |

Speedup: x1.00 (baseline and best timed in 7 alternating batches; 1.00 when no candidate was confirmed faster).

| attempt | outcome | fhe() time (ms) | verdict | note |
|---:|---|---:|---|---|
| 1 | ok | 31.14 | not_confirmed, x1.01 vs attempt 0 |  |
| 2 | ok | - | duplicate (attempt 1) |  |
| 3 | ok | - | same_as_baseline |  |
| 4 | ok | 31.71 | not_confirmed, x1.00 vs attempt 0 |  |
| 5 | ok | 34.07 | slower |  |
| 6 | ok | - | duplicate (attempt 1) |  |
| 7 | ok | 32.90 | slower |  |
| 8 | ok | - | duplicate (attempt 1) |  |
