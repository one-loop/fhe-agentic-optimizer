# chebyshev_selu_d7: LLM optimization run 20261024-122223

Model `claude-sonnet-5-5` (effort medium), 8 attempts, wall time 227 s, approximate API cost $0.0000.

| | fhe() time (ms) | levels | (depth, mult. depth) | ct-ct mul | ct-pt mul | rotations | max error |
|---|---:|---:|---|---:|---:|---:|---:|
| baseline | 357.52 | 8 | (22, 8) | 6 | 8 | 0 | 9.9e-08 |
| best (attempt 8) | 253.38 | 8 | (27, 8) | 6 | 2 | 0 | 1.1e-07 |

Speedup: x1.41.

| attempt | outcome | fhe() time (ms) | note |
|---:|---|---:|---|
| 1 | chehab_error | - | CKKS scale mismatch: ciphertext operands of '+' (term 37) are at the same level 4 with different scales; align |
| 2 | chehab_error | - | CKKS scale mismatch: ciphertext operands of '+' (term 39) are at the same level 5 with different scales; align |
| 3 | chehab_error | - | CKKS scale mismatch: ciphertext operands of '+' (term 31) are at the same level 5 with different scales; align |
| 4 | chehab_error | - | CKKS scale mismatch: ciphertext operands of '+' (term 32) are at the same level 5 with different scales; align |
| 5 | chehab_error | - | CKKS scale mismatch: ciphertext operands of '+' (term 36) are at the same level 4 with different scales; align |
| 6 | chehab_error | - | CKKS scale mismatch: ciphertext operands of '+' (term 38) are at the same level 5 with different scales; align |
| 7 | ok | 358.63 | kernel 93c165f152 |
| 8 | ok | 253.38 | new best |
