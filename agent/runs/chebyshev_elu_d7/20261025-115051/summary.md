# chebyshev_elu_d7: LLM optimization run 20261025-115051

Model `claude-sonnet-5-5` (effort medium), 8 attempts, wall time 458 s, approximate API cost $0.0000.

| | fhe() time (ms) | levels | (depth, mult. depth) | ct-ct mul | ct-pt mul | rotations | max error |
|---|---:|---:|---|---:|---:|---:|---:|
| baseline | 363.97 | 8 | (22, 8) | 6 | 8 | 0 | 1.4e-07 |
| best (attempt 6) | 334.62 | 6 | (20, 6) | 5 | 7 | 0 | 1.7e-07 |

Speedup: x1.07 (baseline and best timed in 7 alternating batches; 1.00 when no candidate was confirmed faster).

| attempt | outcome | fhe() time (ms) | verdict | note |
|---:|---|---:|---|---|
| 1 | chehab_error | - | failed | CKKS scale mismatch: ciphertext operands of '+' (term 37) are at the same level 4 with different scales; align |
| 2 | chehab_error | - | failed | CKKS scale mismatch: ciphertext operands of '+' (term 36) are at the same level 5 with different scales; align |
| 3 | chehab_error | - | failed | CKKS scale mismatch: ciphertext operands of '+' (term 33) are at the same level 4 with different scales; align |
| 4 | chehab_error | - | failed | CKKS scale mismatch: ciphertext operands of '+' (term 58) are at the same level 3 with different scales; align |
| 5 | ok | 363.27 | not_confirmed, x1.02 vs attempt 0 |  |
| 6 | ok | 334.62 | new_best, x1.07 vs attempt 0 |  |
| 7 | chehab_error | - | failed | CKKS scale mismatch: ciphertext operands of '+' (term 28) are at the same level 4 with different scales; align |
| 8 | ok | 359.72 | slower |  |
