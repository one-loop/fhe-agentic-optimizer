# chebyshev_mish_d7: LLM optimization run 20261025-125241

Model `claude-sonnet-5-5` (effort medium), 8 attempts, wall time 255 s, approximate API cost $0.0000.

| | fhe() time (ms) | levels | (depth, mult. depth) | ct-ct mul | ct-pt mul | rotations | max error |
|---|---:|---:|---|---:|---:|---:|---:|
| baseline | 182.30 | 8 | (22, 8) | 6 | 8 | 0 | 1.2e-07 |
| best (attempt 8) | 161.86 | 6 | (18, 6) | 4 | 7 | 0 | 1.4e-07 |

Speedup: x1.13 (baseline and best timed in 7 alternating batches; 1.00 when no candidate was confirmed faster).

| attempt | outcome | fhe() time (ms) | verdict | note |
|---:|---|---:|---|---|
| 1 | chehab_error | - | failed | CKKS scale mismatch: ciphertext operands of '+' (term 37) are at the same level 4 with different scales; align |
| 2 | chehab_error | - | failed | CKKS scale mismatch: ciphertext operands of '+' (term 35) are at the same level 5 with different scales; align |
| 3 | chehab_error | - | failed | CKKS scale mismatch: ciphertext operands of '+' (term 33) are at the same level 5 with different scales; align |
| 4 | chehab_error | - | failed | CKKS scale mismatch: ciphertext operands of '+' (term 33) are at the same level 5 with different scales; align |
| 5 | ok | 197.60 | slower |  |
| 6 | ok | 191.58 | slower |  |
| 7 | wrong_output | 161.18 | failed | wrong result on case fixed (9 slots): max abs error 0.0414 > tolerance 1e-06. The kernel does not compute the  |
| 8 | ok | 161.86 | new_best, x1.14 vs attempt 0 |  |
