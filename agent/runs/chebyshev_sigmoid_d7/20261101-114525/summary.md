# chebyshev_sigmoid_d7: LLM optimization run 20261101-114525

Model `claude-opus-5-5` (effort medium), 8 attempts, wall time 522 s, approximate API cost $0.0000.

| | fhe() time (ms) | levels | (depth, mult. depth) | ct-ct mul | ct-pt mul | rotations | max error |
|---|---:|---:|---|---:|---:|---:|---:|
| baseline | 161.85 | 8 | (22, 8) | 6 | 5 | 0 | 7.5e-08 |
| best (attempt 1) | 100.78 | 6 | (17, 6) | 4 | 2 | 0 | 5.6e-08 |

Speedup: x1.60 (baseline and best timed in 7 alternating batches; 1.00 when no candidate was confirmed faster).

| attempt | outcome | fhe() time (ms) | verdict | note |
|---:|---|---:|---|---|
| 1 | ok | 100.78 | new_best, x1.60 vs attempt 0 |  |
| 2 | ok | 123.78 | slower |  |
| 3 | ok | 102.23 | slower |  |
| 4 | ok | 100.94 | slower |  |
| 5 | ok | 102.30 | slower |  |
| 6 | ok | 99.74 | not_confirmed, x1.00 vs attempt 1 |  |
| 7 | ok | 101.28 | slower |  |
| 8 | ok | 101.85 | slower |  |
