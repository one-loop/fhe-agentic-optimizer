# chebyshev_sigmoid_d5: LLM optimization run 20261024-155954

Model `claude-sonnet-5-5` (effort medium), 8 attempts, wall time 509 s, approximate API cost $0.0000.

| | fhe() time (ms) | levels | (depth, mult. depth) | ct-ct mul | ct-pt mul | rotations | max error |
|---|---:|---:|---|---:|---:|---:|---:|
| baseline | 255.72 | 6 | (16, 6) | 4 | 4 | 0 | 4.2e-08 |
| best (attempt 1) | 211.44 | 4 | (13, 4) | 3 | 4 | 0 | 5.9e-08 |

Speedup: x1.21 (baseline and best timed in 7 alternating batches; 1.00 when no candidate was confirmed faster).

| attempt | outcome | fhe() time (ms) | verdict | note |
|---:|---|---:|---|---|
| 1 | ok | 211.44 | new_best, x1.21 vs attempt 0 |  |
| 2 | ok | 211.55 | slower |  |
| 3 | ok | 210.02 | not_confirmed, x1.01 vs attempt 1 |  |
| 4 | ok | 208.41 | not_confirmed, x1.01 vs attempt 1 |  |
| 5 | ok | 208.99 | not_confirmed, x1.01 vs attempt 1 |  |
| 6 | ok | 208.68 | not_confirmed, x1.01 vs attempt 1 |  |
| 7 | ok | 208.86 | not_confirmed, x1.01 vs attempt 1 |  |
| 8 | ok | 208.35 | not_confirmed, x1.01 vs attempt 1 |  |
