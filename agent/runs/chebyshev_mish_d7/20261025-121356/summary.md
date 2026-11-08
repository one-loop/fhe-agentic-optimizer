# chebyshev_mish_d7: LLM optimization run 20261025-121356

Model `claude-sonnet-5-5` (effort medium), 8 attempts, wall time 554 s, approximate API cost $0.0000.

| | fhe() time (ms) | levels | (depth, mult. depth) | ct-ct mul | ct-pt mul | rotations | max error |
|---|---:|---:|---|---:|---:|---:|---:|
| baseline | 359.77 | 8 | (22, 8) | 6 | 8 | 0 | 1e-07 |
| best (attempt 2) | 346.62 | 5 | (14, 5) | 4 | 9 | 0 | 1.1e-07 |

Speedup: x1.04 (baseline and best timed in 7 alternating batches; 1.00 when no candidate was confirmed faster).

| attempt | outcome | fhe() time (ms) | verdict | note |
|---:|---|---:|---|---|
| 1 | chehab_error | - | failed | CKKS scale mismatch: ciphertext operands of '+' (term 39) are at the same level 4 with different scales; align |
| 2 | ok | 346.62 | new_best, x1.05 vs attempt 0 |  |
| 3 | ok | 340.87 | not_confirmed, x1.00 vs attempt 2 |  |
| 4 | ok | 352.00 | slower |  |
| 5 | ok | 338.96 | not_confirmed, x1.02 vs attempt 2 |  |
| 6 | ok | 347.34 | slower |  |
| 7 | ok | 336.47 | not_confirmed, x1.03 vs attempt 2 |  |
| 8 | ok | - | duplicate (attempt 5) |  |
