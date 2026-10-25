# batchnorm2d: LLM optimization run 20261024-150930

Model `claude-sonnet-5-5` (effort medium), 8 attempts, wall time 155 s, approximate API cost $0.0000.

| | fhe() time (ms) | levels | (depth, mult. depth) | ct-ct mul | ct-pt mul | rotations | max error |
|---|---:|---:|---|---:|---:|---:|---:|
| baseline | 18.01 | 1 | (2, 1) | 0 | 1 | 0 | 2.8e-08 |
| best (attempt 0) | 18.01 | 1 | (2, 1) | 0 | 1 | 0 | 2.8e-08 |

Speedup: x1.00 (baseline and best timed in 7 alternating batches; 1.00 when no candidate was confirmed faster).

| attempt | outcome | fhe() time (ms) | verdict | note |
|---:|---|---:|---|---|
| 1 | ok | - | same_as_baseline |  |
| 2 | ok | - | same_as_baseline |  |
| 3 | ok | 18.62 | slower |  |
| 4 | wrong_output | 17.44 | failed | wrong result on case fixed (8 slots): max abs error 1.41 > tolerance 1e-06. The kernel does not compute the sa |
| 5 | ok | - | duplicate (attempt 3) |  |
| 6 | ok | - | same_as_baseline |  |
| 7 | ok | - | same_as_baseline |  |
| 8 | ok | - | same_as_baseline |  |
