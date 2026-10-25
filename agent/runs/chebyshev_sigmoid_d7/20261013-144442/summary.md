# chebyshev_sigmoid_d7: LLM optimization run 20261013-144442

Model `files` (effort None), 6 attempts, wall time 69 s, approximate API cost $0.0000.

| | fhe() time (ms) | levels | (depth, mult. depth) | ct-ct mul | ct-pt mul | rotations | max error |
|---|---:|---:|---|---:|---:|---:|---:|
| baseline | 322.14 | 8 | (22, 8) | 6 | 5 | 0 | 1.1e-07 |
| best (attempt 6) | 301.18 | 5 | (14, 5) | 4 | 7 | 0 | 5.1e-08 |

Speedup: x1.07.

| attempt | outcome | fhe() time (ms) | note |
|---:|---|---:|---|
| 1 | compile_error | - | kernel:11:27: error: expected ';' at end of declaration |
| 2 | chehab_error | - | CKKS functions do not support constants yet |
| 3 | wrong_output | 259.44 | wrong result on case fixed (9 slots): max abs error 0.00565 > tolerance 1e-06. The kernel does not compute the |
| 4 | ok | 330.79 | kernel 2b16508a17 |
| 5 | chehab_error | - | CKKS scale mismatch: ciphertext operands of '+' (term 31) are at the same level 5 with different scales; align |
| 6 | ok | 301.18 | new best |
