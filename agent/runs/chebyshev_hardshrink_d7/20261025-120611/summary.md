# chebyshev_hardshrink_d7: LLM optimization run 20261025-120611

Model `claude-sonnet-5-5` (effort medium), 8 attempts, wall time 465 s, approximate API cost $0.0000.

| | fhe() time (ms) | levels | (depth, mult. depth) | ct-ct mul | ct-pt mul | rotations | max error |
|---|---:|---:|---|---:|---:|---:|---:|
| baseline | 335.88 | 8 | (21, 8) | 6 | 5 | 0 | 1.2e-07 |
| best (attempt 4) | 257.24 | 5 | (15, 5) | 4 | 4 | 0 | 1.4e-07 |

Speedup: x1.28 (baseline and best timed in 7 alternating batches; 1.00 when no candidate was confirmed faster).

| attempt | outcome | fhe() time (ms) | verdict | note |
|---:|---|---:|---|---|
| 1 | chehab_error | - | failed | CKKS scale mismatch: ciphertext operands of '+' (term 30) are at the same level 5 with different scales; align |
| 2 | chehab_error | - | failed | CKKS scale mismatch: ciphertext operands of '+' (term 26) are at the same level 5 with different scales; align |
| 3 | ok | 391.45 | slower |  |
| 4 | ok | 257.24 | new_best, x1.28 vs attempt 0 |  |
| 5 | ok | 247.15 | not_confirmed, x1.01 vs attempt 4 |  |
| 6 | ok | 244.92 | not_confirmed, x1.00 vs attempt 4 |  |
| 7 | ok | 252.00 | not_confirmed, x0.98 vs attempt 4 |  |
| 8 | ok | - | duplicate (attempt 6) |  |
