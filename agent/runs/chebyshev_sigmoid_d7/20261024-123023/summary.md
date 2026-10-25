# chebyshev_sigmoid_d7: LLM optimization run 20261024-123023

Model `claude-sonnet-5-5` (effort medium), 8 attempts, wall time 247 s, approximate API cost $0.0000.

| | fhe() time (ms) | levels | (depth, mult. depth) | ct-ct mul | ct-pt mul | rotations | max error |
|---|---:|---:|---|---:|---:|---:|---:|
| baseline | 323.76 | 8 | (22, 8) | 6 | 5 | 0 | 4.7e-08 |
| best (attempt 7) | 220.88 | 6 | (17, 6) | 3 | 3 | 0 | 7.9e-08 |

Speedup: x1.47.

| attempt | outcome | fhe() time (ms) | note |
|---:|---|---:|---|
| 1 | chehab_error | - | CKKS scale mismatch: ciphertext operands of '+' (term 31) are at the same level 5 with different scales; align |
| 2 | ok | 454.44 | kernel d910801f27 |
| 3 | ok | 255.25 | new best |
| 4 | ok | 282.19 | kernel cc4a266da6 |
| 5 | ok | 245.95 | new best |
| 6 | ok | 235.29 | new best |
| 7 | ok | 220.88 | new best |
| 8 | ok | 238.59 | kernel d36ed227c4 |
