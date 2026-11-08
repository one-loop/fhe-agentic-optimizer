# embedding: LLM optimization run 20261025-152734

Model `claude-sonnet-5-5` (effort medium), 8 attempts, wall time 656 s, approximate API cost $0.0000.

| | fhe() time (ms) | levels | (depth, mult. depth) | ct-ct mul | ct-pt mul | rotations | max error |
|---|---:|---:|---|---:|---:|---:|---:|
| baseline | 1269.88 | 2 | (21, 2) | 0 | 6 | 26 | 1.8e-05 |
| best (attempt 0) | 1269.88 | 2 | (21, 2) | 0 | 6 | 26 | 1.8e-05 |

Speedup: x1.00 (baseline and best timed in 7 alternating batches; 1.00 when no candidate was confirmed faster).

| attempt | outcome | fhe() time (ms) | verdict | note |
|---:|---|---:|---|---|
| 1 | ok | - | same_as_baseline |  |
| 2 | ok | 1306.47 | slower |  |
| 3 | ok | 1274.64 | slower |  |
| 4 | ok | - | duplicate (attempt 3) |  |
| 5 | ok | 1283.31 | slower |  |
| 6 | ok | 1282.50 | slower |  |
| 7 | ok | - | duplicate (attempt 3) |  |
| 8 | ok | 1297.37 | slower |  |
