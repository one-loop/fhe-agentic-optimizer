# nn_linear: LLM optimization run 20261101-140732

Model `claude-opus-5-5` (effort medium), 8 attempts, wall time 331 s, approximate API cost $0.0000.

| | fhe() time (ms) | levels | (depth, mult. depth) | ct-ct mul | ct-pt mul | rotations | max error |
|---|---:|---:|---|---:|---:|---:|---:|
| baseline | 866.74 | None | (7, 1) | 36 | 0 | 0 | 0 |
| best (attempt 0) | 866.74 | None | (7, 1) | 36 | 0 | 0 | 0 |

Speedup: x1.00 (baseline and best timed in 7 alternating batches; 1.00 when no candidate was confirmed faster).

| attempt | outcome | fhe() time (ms) | verdict | note |
|---:|---|---:|---|---|
| 1 | ok | 865.89 | not_confirmed, x1.00 vs attempt 0 |  |
| 2 | ok | - | same_as_baseline |  |
| 3 | ok | - | duplicate (attempt 1) |  |
| 4 | ok | - | same_as_baseline |  |
| 5 | ok | - | same_as_baseline |  |
| 6 | ok | - | same_as_baseline |  |
| 7 | ok | - | same_as_baseline |  |
| 8 | ok | - | same_as_baseline |  |
