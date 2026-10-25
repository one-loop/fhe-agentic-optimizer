# chebyshev_softplus_d7: LLM optimization run 20261024-123831

Model `claude-sonnet-5-5` (effort medium), 8 attempts, wall time 216 s, approximate API cost $0.0000.

| | fhe() time (ms) | levels | (depth, mult. depth) | ct-ct mul | ct-pt mul | rotations | max error |
|---|---:|---:|---|---:|---:|---:|---:|
| baseline | 313.32 | 7 | (19, 7) | 5 | 5 | 0 | 1.2e-07 |
| best (attempt 8) | 217.91 | 5 | (17, 5) | 1 | 4 | 0 | 7.4e-08 |

Speedup: x1.44.

| attempt | outcome | fhe() time (ms) | note |
|---:|---|---:|---|
| 1 | ok | 241.34 | new best |
| 2 | chehab_error | - | CKKS scale mismatch: ciphertext operands of '+' (term 25) are at the same level 4 with different scales; align |
| 3 | chehab_error | - | CKKS scale mismatch: ciphertext operands of '+' (term 28) are at the same level 4 with different scales; align |
| 4 | ok | 228.46 | new best |
| 5 | ok | 222.85 | new best |
| 6 | ok | 221.76 | new best |
| 7 | ok | 219.28 | new best |
| 8 | ok | 217.91 | new best |
