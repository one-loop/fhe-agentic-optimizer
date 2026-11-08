# extract_linear: LLM optimization run 20261025-154051

Model `claude-sonnet-5-5` (effort medium), 8 attempts, wall time 140 s, approximate API cost $0.0000.

| | fhe() time (ms) | levels | (depth, mult. depth) | ct-ct mul | ct-pt mul | rotations | max error |
|---|---:|---:|---|---:|---:|---:|---:|
| baseline | 59.27 | 1 | (2, 1) | 0 | 1 | 1 | 3.1e-06 |
| best (attempt 0) | 59.27 | 1 | (2, 1) | 0 | 1 | 1 | 3.1e-06 |

Speedup: x1.00 (baseline and best timed in 7 alternating batches; 1.00 when no candidate was confirmed faster).

| attempt | outcome | fhe() time (ms) | verdict | note |
|---:|---|---:|---|---|
| 1 | ok | - | same_as_baseline |  |
| 2 | ok | 105.11 | slower |  |
| 3 | ok | - | same_as_baseline |  |
| 4 | chehab_error | - | failed | backend does not support plain plain evaluation |
| 5 | ok | - | same_as_baseline |  |
| 6 | ok | - | same_as_baseline |  |
| 7 | ok | - | same_as_baseline |  |
| 8 | ok | - | same_as_baseline |  |
