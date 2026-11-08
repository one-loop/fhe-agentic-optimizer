# batchnorm1d: LLM optimization run 20261025-135629

Model `claude-sonnet-5-5` (effort medium), 8 attempts, wall time 134 s, approximate API cost $0.0000.

| | fhe() time (ms) | levels | (depth, mult. depth) | ct-ct mul | ct-pt mul | rotations | max error |
|---|---:|---:|---|---:|---:|---:|---:|
| baseline | 18.03 | 1 | (2, 1) | 0 | 1 | 0 | 6.4e-08 |
| best (attempt 0) | 18.03 | 1 | (2, 1) | 0 | 1 | 0 | 6.4e-08 |

Speedup: x1.00 (baseline and best timed in 7 alternating batches; 1.00 when no candidate was confirmed faster).

| attempt | outcome | fhe() time (ms) | verdict | note |
|---:|---|---:|---|---|
| 1 | ok | - | same_as_baseline |  |
| 2 | ok | 19.40 | slower |  |
| 3 | ok | - | same_as_baseline |  |
| 4 | ok | - | duplicate (attempt 2) |  |
| 5 | wrong_output | 17.34 | failed | wrong result on case fixed (8 slots): max abs error 2.71 > tolerance 1e-06. The kernel does not compute the sa |
| 6 | ok | - | duplicate (attempt 2) |  |
| 7 | ok | - | same_as_baseline |  |
| 8 | ok | - | same_as_baseline |  |
