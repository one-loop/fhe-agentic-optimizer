# quad: LLM optimization run 20261024-165838

Model `claude-sonnet-5-5` (effort medium), 8 attempts, wall time 155 s, approximate API cost $0.0000.

| | fhe() time (ms) | levels | (depth, mult. depth) | ct-ct mul | ct-pt mul | rotations | max error |
|---|---:|---:|---|---:|---:|---:|---:|
| baseline | 64.23 | 1 | (1, 1) | 1 | 0 | 0 | 5.8e-08 |
| best (attempt 0) | 64.23 | 1 | (1, 1) | 1 | 0 | 0 | 5.8e-08 |

Speedup: x1.00 (baseline and best timed in 7 alternating batches; 1.00 when no candidate was confirmed faster).

| attempt | outcome | fhe() time (ms) | verdict | note |
|---:|---|---:|---|---|
| 1 | ok | 63.61 | not_confirmed, x1.01 vs attempt 0 |  |
| 2 | ok | - | same_as_baseline |  |
| 3 | ok | 129.69 | slower |  |
| 4 | ok | - | same_as_baseline |  |
| 5 | ok | - | same_as_baseline |  |
| 6 | ok | - | same_as_baseline |  |
| 7 | ok | - | same_as_baseline |  |
| 8 | ok | - | same_as_baseline |  |
