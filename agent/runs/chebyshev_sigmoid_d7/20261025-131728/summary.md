# chebyshev_sigmoid_d7: LLM optimization run 20261025-131728

Model `claude-sonnet-5-5` (effort medium), 8 attempts, wall time 348 s, approximate API cost $0.0000.

| | fhe() time (ms) | levels | (depth, mult. depth) | ct-ct mul | ct-pt mul | rotations | max error |
|---|---:|---:|---|---:|---:|---:|---:|
| baseline | 161.92 | 8 | (22, 8) | 6 | 5 | 0 | 6.6e-08 |
| best (attempt 7) | 101.02 | 6 | (17, 6) | 4 | 2 | 0 | 1e-07 |

Speedup: x1.60 (baseline and best timed in 7 alternating batches; 1.00 when no candidate was confirmed faster).

| attempt | outcome | fhe() time (ms) | verdict | note |
|---:|---|---:|---|---|
| 1 | chehab_error | - | failed | CKKS scale mismatch: ciphertext operands of '+' (term 31) are at the same level 5 with different scales; align |
| 2 | ok | 149.85 | new_best, x1.08 vs attempt 0 |  |
| 3 | ok | 143.73 | new_best, x1.04 vs attempt 2 |  |
| 4 | ok | 111.61 | new_best, x1.29 vs attempt 3 |  |
| 5 | ok | - | same_as_best (attempt 4) |  |
| 6 | ok | 110.53 | not_confirmed, x1.01 vs attempt 4 |  |
| 7 | ok | 101.02 | new_best, x1.10 vs attempt 4 |  |
| 8 | wrong_output | 129.44 | failed | wrong result on case fixed (9 slots): max abs error 0.83 > tolerance 1e-06. The kernel does not compute the sa |
