# embedding: LLM optimization run 20261024-163940

Model `claude-sonnet-5-5` (effort medium), 8 attempts, wall time 518 s, approximate API cost $0.0000.

| | fhe() time (ms) | levels | (depth, mult. depth) | ct-ct mul | ct-pt mul | rotations | max error |
|---|---:|---:|---|---:|---:|---:|---:|
| baseline | 1285.55 | 2 | (21, 2) | 0 | 6 | 26 | 2.6e-05 |
| best (attempt 0) | 1285.55 | 2 | (21, 2) | 0 | 6 | 26 | 2.6e-05 |

Speedup: x1.00 (baseline and best timed in 7 alternating batches; 1.00 when no candidate was confirmed faster).

| attempt | outcome | fhe() time (ms) | verdict | note |
|---:|---|---:|---|---|
| 1 | ok | - | same_as_baseline |  |
| 2 | ok | 1306.07 | slower |  |
| 3 | ok | - | duplicate (attempt 2) |  |
| 4 | ok | - | same_as_baseline |  |
| 5 | ok | - | same_as_baseline |  |
| 6 | ok | - | same_as_baseline |  |
| 7 | chehab_error | - | failed | backend does not support plain plain evaluation |
| 8 | ok | - | same_as_baseline |  |
