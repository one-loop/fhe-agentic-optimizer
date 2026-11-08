# embedding: LLM optimization run 20261025-174052

Model `claude-sonnet-5-5` (effort medium), 8 attempts, wall time 935 s, approximate API cost $0.0000.

| | fhe() time (ms) | levels | (depth, mult. depth) | ct-ct mul | ct-pt mul | rotations | max error |
|---|---:|---:|---|---:|---:|---:|---:|
| baseline | 1279.22 | 2 | (21, 2) | 0 | 6 | 26 | 4.9e-05 |
| best (attempt 0) | 1279.22 | 2 | (21, 2) | 0 | 6 | 26 | 4.9e-05 |

Speedup: x1.00 (baseline and best timed in 7 alternating batches; 1.00 when no candidate was confirmed faster).

| attempt | outcome | fhe() time (ms) | verdict | note |
|---:|---|---:|---|---|
| 1 | ok | 1275.11 | not_confirmed, x1.00 vs attempt 0 |  |
| 2 | ok | - | duplicate (attempt 1) |  |
| 3 | ok | - | duplicate (attempt 1) |  |
| 4 | ok | 1398.24 | slower |  |
| 5 | ok | 1439.68 | slower |  |
| 6 | ok | - | same_as_baseline |  |
| 7 | ok | 1349.34 | slower |  |
| 8 | ok | 1288.97 | slower |  |
