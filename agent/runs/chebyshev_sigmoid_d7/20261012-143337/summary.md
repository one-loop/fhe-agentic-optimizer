# chebyshev_sigmoid_d7: LLM optimization run 20261012-143337

Model `claude-haiku-5-5` (effort medium), 8 attempts, wall time 177 s, approximate API cost $0.0150.

| | fhe() time (ms) | levels | (depth, mult. depth) | ct-ct mul | ct-pt mul | rotations | max error |
|---|---:|---:|---|---:|---:|---:|---:|
| baseline | 161.56 | 8 | (22, 8) | 6 | 5 | 0 | 6.2e-08 |
| best (attempt 5) | 149.40 | 5 | (14, 5) | 4 | 7 | 0 | 7.2e-08 |

Speedup: x1.08.

| attempt | outcome | fhe() time (ms) | note |
|---:|---|---:|---|
| 1 | chehab_error | - | CKKS scale mismatch: ciphertext operands of '+' (term 34) are at the same level 5 with different scales; align |
| 2 | ok | 175.78 | kernel 802100cc1c |
| 3 | ok | 173.01 | kernel 59a8d0df32 |
| 4 | ok | 149.55 | new best |
| 5 | ok | 149.40 | new best |
| 6 | ok | 149.78 | kernel 1ce8cebfea |
| 7 | ok | 149.67 | kernel 1ce8cebfea |
| 8 | ok | 149.70 | kernel 1ce8cebfea |
