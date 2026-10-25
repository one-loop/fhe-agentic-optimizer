# chebyshev_gelu_d7: LLM optimization run 20261024-120321

Model `claude-sonnet-5-5` (effort medium), 8 attempts, wall time 218 s, approximate API cost $0.0000.

| | fhe() time (ms) | levels | (depth, mult. depth) | ct-ct mul | ct-pt mul | rotations | max error |
|---|---:|---:|---|---:|---:|---:|---:|
| baseline | 314.70 | 7 | (19, 7) | 5 | 5 | 0 | 1.4e-07 |
| best (attempt 8) | 214.06 | 4 | (10, 4) | 3 | 4 | 0 | 1.1e-07 |

Speedup: x1.47.

| attempt | outcome | fhe() time (ms) | note |
|---:|---|---:|---|
| 1 | chehab_error | - | CKKS scale mismatch: ciphertext operands of '+' (term 25) are at the same level 4 with different scales; align |
| 2 | ok | 234.63 | new best |
| 3 | ok | 230.40 | new best |
| 4 | ok | 217.78 | new best |
| 5 | ok | 216.32 | new best |
| 6 | ok | 223.31 | kernel 164f8bd1c6 |
| 7 | ok | 215.97 | new best |
| 8 | ok | 214.06 | new best |
