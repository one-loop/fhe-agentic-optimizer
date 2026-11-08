# batchnorm2d: LLM optimization run 20261025-114737

Model `claude-sonnet-5-5` (effort medium), 8 attempts, wall time 194 s, approximate API cost $0.0000.

| | fhe() time (ms) | levels | (depth, mult. depth) | ct-ct mul | ct-pt mul | rotations | max error |
|---|---:|---:|---|---:|---:|---:|---:|
| baseline | 22.79 | 1 | (2, 1) | 0 | 1 | 0 | 2.4e-08 |
| best (attempt 0) | 22.79 | 1 | (2, 1) | 0 | 1 | 0 | 2.4e-08 |

Speedup: x1.00 (baseline and best timed in 7 alternating batches; 1.00 when no candidate was confirmed faster).

| attempt | outcome | fhe() time (ms) | verdict | note |
|---:|---|---:|---|---|
| 1 | ok | - | same_as_baseline |  |
| 2 | ok | - | same_as_baseline |  |
| 3 | ok | 18.63 | not_confirmed, x0.95 vs attempt 0 |  |
| 4 | ok | - | duplicate (attempt 3) |  |
| 5 | ok | - | same_as_baseline |  |
| 6 | ok | - | duplicate (attempt 3) |  |
| 7 | ok | - | same_as_baseline |  |
| 8 | ok | - | same_as_baseline |  |
