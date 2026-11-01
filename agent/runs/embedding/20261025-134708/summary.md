# embedding: LLM optimization run 20261025-134708

Model `claude-sonnet-5-5` (effort medium), 8 attempts, wall time 479 s, approximate API cost $0.0000.

| | fhe() time (ms) | levels | (depth, mult. depth) | ct-ct mul | ct-pt mul | rotations | max error |
|---|---:|---:|---|---:|---:|---:|---:|
| baseline | 619.26 | 2 | (21, 2) | 0 | 6 | 26 | 3.9e-05 |
| best (attempt 0) | 619.26 | 2 | (21, 2) | 0 | 6 | 26 | 3.9e-05 |

Speedup: x1.00 (baseline and best timed in 7 alternating batches; 1.00 when no candidate was confirmed faster).

| attempt | outcome | fhe() time (ms) | verdict | note |
|---:|---|---:|---|---|
| 1 | ok | - | same_as_baseline |  |
| 2 | ok | 618.01 | not_confirmed, x1.00 vs attempt 0 |  |
| 3 | ok | - | same_as_baseline |  |
| 4 | ok | 677.38 | slower |  |
| 5 | ok | - | same_as_baseline |  |
| 6 | ok | - | same_as_baseline |  |
| 7 | ok | - | same_as_baseline |  |
| 8 | ok | - | same_as_baseline |  |
