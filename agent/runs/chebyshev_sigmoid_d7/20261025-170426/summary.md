# chebyshev_sigmoid_d7: LLM optimization run 20261025-170426

Model `claude-sonnet-5-5` (effort medium), 8 attempts, wall time 370 s, approximate API cost $0.0000.

| | fhe() time (ms) | levels | (depth, mult. depth) | ct-ct mul | ct-pt mul | rotations | max error |
|---|---:|---:|---|---:|---:|---:|---:|
| baseline | 319.79 | 8 | (22, 8) | 6 | 5 | 0 | 6.2e-08 |
| best (attempt 4) | 297.49 | 6 | (17, 6) | 4 | 5 | 0 | 7e-08 |

Speedup: x1.09 (baseline and best timed in 7 alternating batches; 1.00 when no candidate was confirmed faster).

| attempt | outcome | fhe() time (ms) | verdict | note |
|---:|---|---:|---|---|
| 1 | chehab_error | - | failed | CKKS scale mismatch: ciphertext operands of '+' (term 31) are at the same level 5 with different scales; align |
| 2 | ok | 343.65 | slower |  |
| 3 | chehab_error | - | failed | CKKS scale mismatch: ciphertext operands of '-' (term 44) are at the same level 4 with different scales; align |
| 4 | ok | 297.49 | new_best, x1.08 vs attempt 0 |  |
| 5 | ok | 314.85 | slower |  |
| 6 | wrong_output | 316.81 | failed | wrong result on case fixed (9 slots): max abs error 1.19e-06 > tolerance 1e-06. The kernel does not compute th |
| 7 | ok | 309.07 | slower |  |
| 8 | ok | 306.14 | slower |  |
