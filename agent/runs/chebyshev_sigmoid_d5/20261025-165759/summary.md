# chebyshev_sigmoid_d5: LLM optimization run 20261025-165759

Model `claude-sonnet-5-5` (effort medium), 8 attempts, wall time 388 s, approximate API cost $0.0000.

| | fhe() time (ms) | levels | (depth, mult. depth) | ct-ct mul | ct-pt mul | rotations | max error |
|---|---:|---:|---|---:|---:|---:|---:|
| baseline | 256.50 | 6 | (16, 6) | 4 | 4 | 0 | 5.3e-08 |
| best (attempt 4) | 168.20 | 5 | (12, 5) | 3 | 2 | 0 | 4.3e-08 |

Speedup: x1.53 (baseline and best timed in 7 alternating batches; 1.00 when no candidate was confirmed faster).

| attempt | outcome | fhe() time (ms) | verdict | note |
|---:|---|---:|---|---|
| 1 | ok | 196.20 | new_best, x1.30 vs attempt 0 |  |
| 2 | ok | 201.35 | slower |  |
| 3 | ok | 174.26 | new_best, x1.13 vs attempt 1 |  |
| 4 | ok | 168.20 | new_best, x1.04 vs attempt 3 |  |
| 5 | ok | 214.43 | slower |  |
| 6 | ok | 176.92 | slower |  |
| 7 | ok | 167.61 | not_confirmed, x1.00 vs attempt 4 |  |
| 8 | ok | - | same_as_best (attempt 4) |  |
