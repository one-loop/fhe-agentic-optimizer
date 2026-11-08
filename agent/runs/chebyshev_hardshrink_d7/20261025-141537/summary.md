# chebyshev_hardshrink_d7: LLM optimization run 20261025-141537

Model `claude-sonnet-5-5` (effort medium), 8 attempts, wall time 509 s, approximate API cost $0.0000.

| | fhe() time (ms) | levels | (depth, mult. depth) | ct-ct mul | ct-pt mul | rotations | max error |
|---|---:|---:|---|---:|---:|---:|---:|
| baseline | 318.32 | 8 | (21, 8) | 6 | 5 | 0 | 7.5e-08 |
| best (attempt 3) | 268.18 | 6 | (15, 6) | 4 | 5 | 0 | 9.9e-08 |

Speedup: x1.19 (baseline and best timed in 7 alternating batches; 1.00 when no candidate was confirmed faster).

| attempt | outcome | fhe() time (ms) | verdict | note |
|---:|---|---:|---|---|
| 1 | chehab_error | - | failed | CKKS scale mismatch: ciphertext operands of '+' (term 30) are at the same level 5 with different scales; align |
| 2 | chehab_error | - | failed | CKKS scale mismatch: ciphertext operands of '+' (term 29) are at the same level 5 with different scales; align |
| 3 | ok | 268.18 | new_best, x1.19 vs attempt 0 |  |
| 4 | ok | 266.57 | not_confirmed, x1.00 vs attempt 3 |  |
| 5 | ok | 265.02 | not_confirmed, x1.01 vs attempt 3 |  |
| 6 | ok | 262.07 | not_confirmed, x1.02 vs attempt 3 |  |
| 7 | ok | 262.09 | not_confirmed, x1.02 vs attempt 3 |  |
| 8 | wrong_output | 241.60 | failed | wrong result on case fixed (9 slots): max abs error 0.0518 > tolerance 1e-06. The kernel does not compute the  |
