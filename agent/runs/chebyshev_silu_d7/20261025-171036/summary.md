# chebyshev_silu_d7: LLM optimization run 20261025-171036

Model `claude-sonnet-5-5` (effort medium), 8 attempts, wall time 354 s, approximate API cost $0.0000.

| | fhe() time (ms) | levels | (depth, mult. depth) | ct-ct mul | ct-pt mul | rotations | max error |
|---|---:|---:|---|---:|---:|---:|---:|
| baseline | 335.25 | 7 | (19, 7) | 5 | 5 | 0 | 1e-07 |
| best (attempt 7) | 209.77 | 5 | (16, 5) | 3 | 4 | 0 | 1e-07 |

Speedup: x1.43 (baseline and best timed in 7 alternating batches; 1.00 when no candidate was confirmed faster).

| attempt | outcome | fhe() time (ms) | verdict | note |
|---:|---|---:|---|---|
| 1 | ok | 257.60 | new_best, x1.33 vs attempt 0 |  |
| 2 | chehab_error | - | failed | CKKS scale mismatch: ciphertext operands of '+' (term 25) are at the same level 4 with different scales; align |
| 3 | ok | 216.53 | not_confirmed, x1.05 vs attempt 1 |  |
| 4 | chehab_error | - | failed | CKKS scale mismatch: ciphertext operands of '+' (term 23) are at the same level 4 with different scales; align |
| 5 | wrong_output | 215.00 | failed | wrong result on case fixed (9 slots): max abs error 0.143 > tolerance 1e-06. The kernel does not compute the s |
| 6 | ok | - | duplicate (attempt 3) |  |
| 7 | ok | 209.77 | new_best, x1.09 vs attempt 1 |  |
| 8 | ok | 210.55 | slower |  |
