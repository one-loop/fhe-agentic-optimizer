# embedding: LLM optimization run 20261025-132212

Model `claude-sonnet-5-5` (effort medium), 8 attempts, wall time 527 s, approximate API cost $0.0000.

| | fhe() time (ms) | levels | (depth, mult. depth) | ct-ct mul | ct-pt mul | rotations | max error |
|---|---:|---:|---|---:|---:|---:|---:|
| baseline | 1288.55 | 2 | (21, 2) | 0 | 6 | 26 | 1.6e-05 |
| best (attempt 0) | 1288.55 | 2 | (21, 2) | 0 | 6 | 26 | 1.6e-05 |

Speedup: x1.00 (baseline and best timed in 7 alternating batches; 1.00 when no candidate was confirmed faster).

| attempt | outcome | fhe() time (ms) | verdict | note |
|---:|---|---:|---|---|
| 1 | ok | - | same_as_baseline |  |
| 2 | ok | 1525.20 | slower |  |
| 3 | ok | - | duplicate (attempt 2) |  |
| 4 | ok | - | same_as_baseline |  |
| 5 | ok | 1309.88 | slower |  |
| 6 | ok | - | same_as_baseline |  |
| 7 | chehab_error | - | failed | backend does not support plain plain evaluation |
| 8 | ok | - | same_as_baseline |  |
