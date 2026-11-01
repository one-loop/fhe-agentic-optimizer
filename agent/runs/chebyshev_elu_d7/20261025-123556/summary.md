# chebyshev_elu_d7: LLM optimization run 20261025-123556

Model `claude-sonnet-5-5` (effort medium), 8 attempts, wall time 351 s, approximate API cost $0.0000.

| | fhe() time (ms) | levels | (depth, mult. depth) | ct-ct mul | ct-pt mul | rotations | max error |
|---|---:|---:|---|---:|---:|---:|---:|
| baseline | 183.24 | 8 | (22, 8) | 6 | 8 | 0 | 9.7e-08 |
| best (attempt 5) | 164.14 | 6 | (20, 6) | 4 | 11 | 0 | 2.2e-07 |

Speedup: x1.10 (baseline and best timed in 7 alternating batches; 1.00 when no candidate was confirmed faster).

| attempt | outcome | fhe() time (ms) | verdict | note |
|---:|---|---:|---|---|
| 1 | chehab_error | - | failed | CKKS scale mismatch: ciphertext operands of '+' (term 37) are at the same level 4 with different scales; align |
| 2 | chehab_error | - | failed | CKKS scale mismatch: ciphertext operands of '+' (term 42) are at the same level 5 with different scales; align |
| 3 | chehab_error | - | failed | CKKS scale mismatch: ciphertext operands of '+' (term 36) are at the same level 5 with different scales; align |
| 4 | ok | 227.33 | slower |  |
| 5 | ok | 164.14 | new_best, x1.10 vs attempt 0 |  |
| 6 | wrong_output | 162.96 | failed | wrong result on case fixed (9 slots): max abs error 0.0569 > tolerance 1e-06. The kernel does not compute the  |
| 7 | wrong_output | 156.05 | failed | wrong result on case fixed (9 slots): max abs error 0.0569 > tolerance 1e-06. The kernel does not compute the  |
| 8 | wrong_output | 160.89 | failed | wrong result on case fixed (9 slots): max abs error 0.0569 > tolerance 1e-06. The kernel does not compute the  |
