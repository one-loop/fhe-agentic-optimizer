# chebyshev_selu_d7: LLM optimization run 20261012-142804

Model `claude-haiku-5-5` (effort medium), 8 attempts, wall time 165 s, approximate API cost $0.0136.

| | fhe() time (ms) | levels | (depth, mult. depth) | ct-ct mul | ct-pt mul | rotations | max error |
|---|---:|---:|---|---:|---:|---:|---:|
| baseline | 181.90 | 8 | (22, 8) | 6 | 8 | 0 | 1.3e-07 |
| best (attempt 3) | 167.67 | 5 | (14, 5) | 4 | 9 | 0 | 1.6e-07 |

Speedup: x1.08.

| attempt | outcome | fhe() time (ms) | note |
|---:|---|---:|---|
| 1 | wrong_output | 148.84 | wrong result on case fixed (9 slots): max abs error 0.948 > tolerance 1e-06. The kernel does not compute the s |
| 2 | chehab_error | - | CKKS scale mismatch: ciphertext operands of '+' (term 28) are at the same level 4 with different scales; align |
| 3 | ok | 167.67 | new best |
| 4 | ok | 167.80 | kernel b2b30b0f82 |
| 5 | ok | 168.12 | kernel b2b30b0f82 |
| 6 | ok | 170.35 | kernel b2b30b0f82 |
| 7 | ok | 170.67 | kernel b2b30b0f82 |
| 8 | ok | 169.43 | kernel b2b30b0f82 |
