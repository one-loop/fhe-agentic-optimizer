# chebyshev_gelu_d7: LLM optimization run 20261025-140923

Model `claude-sonnet-5-5` (effort medium), 8 attempts, wall time 374 s, approximate API cost $0.0000.

| | fhe() time (ms) | levels | (depth, mult. depth) | ct-ct mul | ct-pt mul | rotations | max error |
|---|---:|---:|---|---:|---:|---:|---:|
| baseline | 300.37 | 7 | (19, 7) | 5 | 5 | 0 | 1.1e-07 |
| best (attempt 5) | 189.50 | 5 | (15, 5) | 3 | 3 | 0 | 1e-07 |

Speedup: x1.59 (baseline and best timed in 7 alternating batches; 1.00 when no candidate was confirmed faster).

| attempt | outcome | fhe() time (ms) | verdict | note |
|---:|---|---:|---|---|
| 1 | ok | 228.92 | new_best, x1.32 vs attempt 0 |  |
| 2 | ok | 212.41 | new_best, x1.07 vs attempt 1 |  |
| 3 | ok | 212.96 | slower |  |
| 4 | ok | 225.41 | slower |  |
| 5 | ok | 189.50 | new_best, x1.12 vs attempt 2 |  |
| 6 | ok | 190.40 | slower |  |
| 7 | ok | 191.39 | slower |  |
| 8 | ok | 190.96 | slower |  |
