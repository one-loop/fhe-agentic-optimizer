# batchnorm1d: LLM optimization run 20261024-150653

Model `claude-sonnet-5-5` (effort medium), 8 attempts, wall time 157 s, approximate API cost $0.0000.

| | fhe() time (ms) | levels | (depth, mult. depth) | ct-ct mul | ct-pt mul | rotations | max error |
|---|---:|---:|---|---:|---:|---:|---:|
| baseline | 17.95 | 1 | (2, 1) | 0 | 1 | 0 | 5.9e-08 |
| best (attempt 0) | 17.95 | 1 | (2, 1) | 0 | 1 | 0 | 5.9e-08 |

Speedup: x1.00 (baseline and best timed in 7 alternating batches; 1.00 when no candidate was confirmed faster).

| attempt | outcome | fhe() time (ms) | verdict | note |
|---:|---|---:|---|---|
| 1 | ok | - | same_as_baseline |  |
| 2 | chehab_error | - | failed | CHEHAB rejected the kernel (case fixed, 8 slots): |
| 3 | ok | - | same_as_baseline |  |
| 4 | ok | - | same_as_baseline |  |
| 5 | ok | - | same_as_baseline |  |
| 6 | ok | - | same_as_baseline |  |
| 7 | ok | - | same_as_baseline |  |
| 8 | ok | - | same_as_baseline |  |
