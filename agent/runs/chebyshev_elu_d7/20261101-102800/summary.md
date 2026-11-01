# chebyshev_elu_d7: LLM optimization run 20261101-102800

Model `claude-opus-5-5` (effort medium), 8 attempts, wall time 603 s, approximate API cost $0.0000.

| | fhe() time (ms) | levels | (depth, mult. depth) | ct-ct mul | ct-pt mul | rotations | max error |
|---|---:|---:|---|---:|---:|---:|---:|
| baseline | 182.57 | 8 | (22, 8) | 6 | 8 | 0 | 9.3e-08 |
| best (attempt 1) | 168.64 | 5 | (13, 5) | 4 | 9 | 0 | 1.1e-07 |

Speedup: x1.08 (baseline and best timed in 7 alternating batches; 1.00 when no candidate was confirmed faster).

| attempt | outcome | fhe() time (ms) | verdict | note |
|---:|---|---:|---|---|
| 1 | ok | 168.64 | new_best, x1.08 vs attempt 0 |  |
| 2 | ok | 173.54 | slower |  |
| 3 | ok | 165.35 | not_confirmed, x1.02 vs attempt 1 |  |
| 4 | ok | 166.76 | not_confirmed, x1.01 vs attempt 1 |  |
| 5 | ok | 165.80 | not_confirmed, x1.01 vs attempt 1 |  |
| 6 | ok | 164.98 | not_confirmed, x1.02 vs attempt 1 |  |
| 7 | ok | 165.51 | not_confirmed, x1.02 vs attempt 1 |  |
| 8 | ok | 165.65 | not_confirmed, x1.02 vs attempt 1 |  |
