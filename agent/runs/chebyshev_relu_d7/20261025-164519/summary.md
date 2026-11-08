# chebyshev_relu_d7: LLM optimization run 20261025-164519

Model `claude-sonnet-5-5` (effort medium), 8 attempts, wall time 402 s, approximate API cost $0.0000.

| | fhe() time (ms) | levels | (depth, mult. depth) | ct-ct mul | ct-pt mul | rotations | max error |
|---|---:|---:|---|---:|---:|---:|---:|
| baseline | 300.57 | 7 | (19, 7) | 5 | 5 | 0 | 9.3e-08 |
| best (attempt 3) | 196.50 | 5 | (18, 5) | 3 | 3 | 0 | 1.2e-07 |

Speedup: x1.55 (baseline and best timed in 7 alternating batches; 1.00 when no candidate was confirmed faster).

| attempt | outcome | fhe() time (ms) | verdict | note |
|---:|---|---:|---|---|
| 1 | ok | 228.28 | new_best, x1.32 vs attempt 0 |  |
| 2 | wrong_output | 181.20 | failed | wrong result on case fixed (9 slots): max abs error 1.35 > tolerance 1e-06. The kernel does not compute the sa |
| 3 | ok | 196.50 | new_best, x1.18 vs attempt 1 |  |
| 4 | ok | 227.72 | slower |  |
| 5 | ok | 190.36 | not_confirmed, x1.02 vs attempt 3 |  |
| 6 | ok | 196.75 | slower |  |
| 7 | ok | 193.03 | not_confirmed, x1.01 vs attempt 3 |  |
| 8 | ok | - | same_as_best (attempt 3) |  |
