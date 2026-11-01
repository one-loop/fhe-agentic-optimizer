# nn_avgpool2d: LLM optimization run 20261101-132202

Model `claude-opus-5-5` (effort medium), 8 attempts, wall time 141 s, approximate API cost $0.0000.

| | fhe() time (ms) | levels | (depth, mult. depth) | ct-ct mul | ct-pt mul | rotations | max error |
|---|---:|---:|---|---:|---:|---:|---:|
| baseline | 3.92 | None | (3, 0) | 0 | 0 | 0 | 0 |
| best (attempt 0) | 3.92 | None | (3, 0) | 0 | 0 | 0 | 0 |

Speedup: x1.00 (baseline and best timed in 7 alternating batches; 1.00 when no candidate was confirmed faster).

| attempt | outcome | fhe() time (ms) | verdict | note |
|---:|---|---:|---|---|
| 1 | ok | 3.94 | slower |  |
| 2 | ok | - | same_as_baseline |  |
| 3 | ok | - | duplicate (attempt 1) |  |
| 4 | ok | - | same_as_baseline |  |
| 5 | ok | - | same_as_baseline |  |
| 6 | ok | - | same_as_baseline |  |
| 7 | ok | - | same_as_baseline |  |
| 8 | ok | - | same_as_baseline |  |
