# chebyshev_sigmoid_d5: LLM optimization run 20261101-113649

Model `claude-opus-5-5` (effort medium), 8 attempts, wall time 517 s, approximate API cost $0.0000.

| | fhe() time (ms) | levels | (depth, mult. depth) | ct-ct mul | ct-pt mul | rotations | max error |
|---|---:|---:|---|---:|---:|---:|---:|
| baseline | 130.28 | 6 | (16, 6) | 4 | 4 | 0 | 4.4e-08 |
| best (attempt 6) | 83.88 | 5 | (12, 5) | 3 | 2 | 0 | 3.2e-08 |

Speedup: x1.54 (baseline and best timed in 7 alternating batches; 1.00 when no candidate was confirmed faster).

| attempt | outcome | fhe() time (ms) | verdict | note |
|---:|---|---:|---|---|
| 1 | ok | 107.41 | new_best, x1.21 vs attempt 0 |  |
| 2 | ok | 107.17 | not_confirmed, x0.99 vs attempt 1 |  |
| 3 | ok | 114.35 | slower |  |
| 4 | ok | 105.90 | not_confirmed, x1.00 vs attempt 1 |  |
| 5 | ok | 93.67 | new_best, x1.14 vs attempt 1 |  |
| 6 | ok | 83.88 | new_best, x1.12 vs attempt 5 |  |
| 7 | ok | 104.45 | slower |  |
| 8 | ok | 83.87 | not_confirmed, x1.00 vs attempt 6 |  |
