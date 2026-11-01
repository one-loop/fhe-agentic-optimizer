# chebyshev_elu_d7: LLM optimization run 20261012-141151

Model `claude-haiku-5-5` (effort medium), 8 attempts, wall time 183 s, approximate API cost $0.0159.

| | fhe() time (ms) | levels | (depth, mult. depth) | ct-ct mul | ct-pt mul | rotations | max error |
|---|---:|---:|---|---:|---:|---:|---:|
| baseline | 181.82 | 8 | (22, 8) | 6 | 8 | 0 | 9.9e-08 |
| best (attempt 5) | 175.04 | 5 | (14, 5) | 4 | 9 | 0 | 1.9e-07 |

Speedup: x1.04.

| attempt | outcome | fhe() time (ms) | note |
|---:|---|---:|---|
| 1 | chehab_error | - | CKKS scale mismatch: ciphertext operands of '+' (term 39) are at the same level 4 with different scales; align |
| 2 | ok | 181.23 | new best |
| 3 | wrong_output | 148.69 | wrong result on case fixed (9 slots): max abs error 0.899 > tolerance 1e-06. The kernel does not compute the s |
| 4 | ok | 175.23 | new best |
| 5 | ok | 175.04 | new best |
| 6 | ok | 175.80 | kernel 24db10eba6 |
| 7 | ok | 175.54 | kernel 24db10eba6 |
| 8 | ok | 176.07 | kernel 24db10eba6 |
