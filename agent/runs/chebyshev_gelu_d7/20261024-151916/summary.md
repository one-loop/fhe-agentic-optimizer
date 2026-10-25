# chebyshev_gelu_d7: LLM optimization run 20261024-151916

Model `claude-sonnet-5-5` (effort medium), 8 attempts, wall time 367 s, approximate API cost $0.0000.

| | fhe() time (ms) | levels | (depth, mult. depth) | ct-ct mul | ct-pt mul | rotations | max error |
|---|---:|---:|---|---:|---:|---:|---:|
| baseline | 303.56 | 7 | (19, 7) | 5 | 5 | 0 | 1.2e-07 |
| best (attempt 2) | 209.96 | 5 | (16, 5) | 3 | 4 | 0 | 9.4e-08 |

Speedup: x1.44 (baseline and best timed in 7 alternating batches; 1.00 when no candidate was confirmed faster).

| attempt | outcome | fhe() time (ms) | verdict | note |
|---:|---|---:|---|---|
| 1 | ok | 229.01 | new_best, x1.32 vs attempt 0 |  |
| 2 | ok | 209.96 | new_best, x1.09 vs attempt 1 |  |
| 3 | chehab_error | - | failed | CKKS scale mismatch: ciphertext operands of '+' (term 21) are at the same level 4 with different scales; align |
| 4 | chehab_error | - | failed | CKKS scale mismatch: ciphertext operands of '+' (term 23) are at the same level 4 with different scales; align |
| 5 | ok | - | same_as_best (attempt 2) |  |
| 6 | ok | 205.12 | not_confirmed, x1.02 vs attempt 2 |  |
| 7 | chehab_error | - | failed | CKKS scale mismatch: ciphertext operands of '+' (term 25) are at the same level 4 with different scales; align |
| 8 | ok | - | duplicate (attempt 6) |  |
