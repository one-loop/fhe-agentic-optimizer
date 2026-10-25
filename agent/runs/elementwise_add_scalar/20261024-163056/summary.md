# elementwise_add_scalar: LLM optimization run 20261024-163056

Model `claude-sonnet-5-5` (effort medium), 8 attempts, wall time 131 s, approximate API cost $0.0000.

| | fhe() time (ms) | levels | (depth, mult. depth) | ct-ct mul | ct-pt mul | rotations | max error |
|---|---:|---:|---|---:|---:|---:|---:|
| baseline | 0.43 | 0 | (1, 0) | 0 | 0 | 0 | 2.4e-08 |
| best (attempt 0) | 0.43 | 0 | (1, 0) | 0 | 0 | 0 | 2.4e-08 |

Speedup: x1.00 (baseline and best timed in 7 alternating batches; 1.00 when no candidate was confirmed faster).

| attempt | outcome | fhe() time (ms) | verdict | note |
|---:|---|---:|---|---|
| 1 | ok | - | same_as_baseline |  |
| 2 | ok | - | same_as_baseline |  |
| 3 | ok | - | same_as_baseline |  |
| 4 | ok | 1.05 | slower |  |
| 5 | ok | - | same_as_baseline |  |
| 6 | ok | - | same_as_baseline |  |
| 7 | ok | - | same_as_baseline |  |
| 8 | ok | - | duplicate (attempt 4) |  |
