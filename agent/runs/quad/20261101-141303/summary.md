# quad: LLM optimization run 20261101-141303

Model `claude-opus-5-5` (effort medium), 8 attempts, wall time 148 s, approximate API cost $0.0000.

| | fhe() time (ms) | levels | (depth, mult. depth) | ct-ct mul | ct-pt mul | rotations | max error |
|---|---:|---:|---|---:|---:|---:|---:|
| baseline | 31.51 | 1 | (1, 1) | 1 | 0 | 0 | 8.8e-08 |
| best (attempt 0) | 31.51 | 1 | (1, 1) | 1 | 0 | 0 | 8.8e-08 |

Speedup: x1.00 (baseline and best timed in 7 alternating batches; 1.00 when no candidate was confirmed faster).

| attempt | outcome | fhe() time (ms) | verdict | note |
|---:|---|---:|---|---|
| 1 | ok | 31.24 | not_confirmed, x1.01 vs attempt 0 |  |
| 2 | ok | - | duplicate (attempt 1) |  |
| 3 | ok | - | duplicate (attempt 1) |  |
| 4 | ok | - | duplicate (attempt 1) |  |
| 5 | ok | - | duplicate (attempt 1) |  |
| 6 | ok | 31.95 | slower |  |
| 7 | ok | 31.48 | not_confirmed, x1.00 vs attempt 0 |  |
| 8 | ok | 31.91 | slower |  |
