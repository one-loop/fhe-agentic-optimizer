# chebyshev_mish_d7: LLM optimization run 20261013-153812

Model `claude-haiku-5-5` (effort medium), 8 attempts, wall time 298 s, approximate API cost $0.0221.

| | fhe() time (ms) | levels | (depth, mult. depth) | ct-ct mul | ct-pt mul | rotations | max error |
|---|---:|---:|---|---:|---:|---:|---:|
| baseline | 388.14 | 8 | (22, 8) | 6 | 8 | 0 | 1.3e-07 |
| best (attempt 4) | 334.52 | 5 | (13, 5) | 4 | 9 | 0 | 1.3e-07 |

Speedup: x1.16.

| attempt | outcome | fhe() time (ms) | note |
|---:|---|---:|---|
| 1 | chehab_error | - | CKKS scale mismatch: ciphertext operands of '+' (term 40) are at the same level 4 with different scales; align |
| 2 | chehab_error | - | CKKS scale mismatch: ciphertext operands of '+' (term 45) are at the same level 5 with different scales; align |
| 3 | ok | 425.31 | kernel 04e612b2a8 |
| 4 | ok | 334.52 | new best |
| 5 | ok | 407.25 | kernel 84c772795f |
| 6 | ok | 339.39 | kernel 277131e45d |
| 7 | ok | 379.75 | kernel 277131e45d |
| 8 | ok | 339.57 | kernel 277131e45d |
