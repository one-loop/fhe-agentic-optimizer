# chebyshev_softplus_d7: LLM optimization run 20261025-130025

Model `claude-sonnet-5-5` (effort medium), 8 attempts, wall time 426 s, approximate API cost $0.0000.

| | fhe() time (ms) | levels | (depth, mult. depth) | ct-ct mul | ct-pt mul | rotations | max error |
|---|---:|---:|---|---:|---:|---:|---:|
| baseline | 301.57 | 7 | (19, 7) | 5 | 5 | 0 | 1.1e-07 |
| best (attempt 4) | 211.46 | 4 | (12, 4) | 3 | 4 | 0 | 1.8e-07 |

Speedup: x1.42 (baseline and best timed in 7 alternating batches; 1.00 when no candidate was confirmed faster).

| attempt | outcome | fhe() time (ms) | verdict | note |
|---:|---|---:|---|---|
| 1 | ok | 235.75 | new_best, x1.29 vs attempt 0 |  |
| 2 | chehab_error | - | failed | CKKS scale mismatch: ciphertext operands of '+' (term 21) are at the same level 4 with different scales; align |
| 3 | ok | 238.31 | slower |  |
| 4 | ok | 211.46 | new_best, x1.11 vs attempt 1 |  |
| 5 | ok | - | same_as_best (attempt 4) |  |
| 6 | ok | 210.75 | not_confirmed, x1.01 vs attempt 4 |  |
| 7 | ok | 211.27 | not_confirmed, x1.00 vs attempt 4 |  |
| 8 | ok | 211.13 | not_confirmed, x1.00 vs attempt 4 |  |
