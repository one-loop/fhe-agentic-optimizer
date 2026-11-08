# chebyshev_sigmoid_d7: LLM optimization run 20261025-145427

Model `claude-sonnet-5-5` (effort medium), 8 attempts, wall time 525 s, approximate API cost $0.0000.

| | fhe() time (ms) | levels | (depth, mult. depth) | ct-ct mul | ct-pt mul | rotations | max error |
|---|---:|---:|---|---:|---:|---:|---:|
| baseline | 319.56 | 8 | (22, 8) | 6 | 5 | 0 | 7.4e-08 |
| best (attempt 7) | 244.32 | 6 | (16, 6) | 4 | 4 | 0 | 2.7e-08 |

Speedup: x1.31 (baseline and best timed in 7 alternating batches; 1.00 when no candidate was confirmed faster).

| attempt | outcome | fhe() time (ms) | verdict | note |
|---:|---|---:|---|---|
| 1 | chehab_error | - | failed | CKKS scale mismatch: ciphertext operands of '+' (term 31) are at the same level 5 with different scales; align |
| 2 | chehab_error | - | failed | CKKS scale mismatch: ciphertext operands of '+' (term 27) are at the same level 5 with different scales; align |
| 3 | ok | 403.29 | slower |  |
| 4 | ok | 294.99 | new_best, x1.08 vs attempt 0 |  |
| 5 | ok | 292.87 | not_confirmed, x1.01 vs attempt 4 |  |
| 6 | ok | 293.63 | not_confirmed, x1.01 vs attempt 4 |  |
| 7 | ok | 244.32 | new_best, x1.21 vs attempt 4 |  |
| 8 | chehab_error | - | failed | CKKS scale mismatch: ciphertext operands of '+' (term 25) are at the same level 4 with different scales; align |
