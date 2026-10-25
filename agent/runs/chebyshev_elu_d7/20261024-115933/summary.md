# chebyshev_elu_d7: LLM optimization run 20261024-115933

Model `claude-sonnet-5-5` (effort medium), 8 attempts, wall time 228 s, approximate API cost $0.0000.

| | fhe() time (ms) | levels | (depth, mult. depth) | ct-ct mul | ct-pt mul | rotations | max error |
|---|---:|---:|---|---:|---:|---:|---:|
| baseline | 394.26 | 8 | (22, 8) | 6 | 8 | 0 | 1.3e-07 |
| best (attempt 7) | 369.60 | 8 | (22, 8) | 6 | 8 | 0 | 8.8e-08 |

Speedup: x1.07.

| attempt | outcome | fhe() time (ms) | note |
|---:|---|---:|---|
| 1 | chehab_error | - | CKKS scale mismatch: ciphertext operands of '+' (term 37) are at the same level 4 with different scales; align |
| 2 | chehab_error | - | CKKS scale mismatch: ciphertext operands of '+' (term 37) are at the same level 4 with different scales; align |
| 3 | chehab_error | - | CKKS scale mismatch: ciphertext operands of '+' (term 43) are at the same level 6 with different scales; align |
| 4 | chehab_error | - | CKKS scale mismatch: ciphertext operands of '-' (term 22) are at the same level 5 with different scales; align |
| 5 | chehab_error | - | CKKS scale mismatch: ciphertext operands of '+' (term 39) are at the same level 5 with different scales; align |
| 6 | chehab_error | - | CKKS scale mismatch: ciphertext operands of '+' (term 39) are at the same level 5 with different scales; align |
| 7 | ok | 369.60 | new best |
| 8 | ok | 423.28 | kernel 37196ab2f9 |
