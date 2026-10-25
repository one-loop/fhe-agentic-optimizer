# relu_composite: LLM optimization run 20261013-163818

Model `claude-haiku-5-5` (effort medium), 8 attempts, wall time 327 s, approximate API cost $0.0173.

| | fhe() time (ms) | levels | (depth, mult. depth) | ct-ct mul | ct-pt mul | rotations | max error |
|---|---:|---:|---|---:|---:|---:|---:|
| baseline | 672.92 | 10 | (17, 10) | 16 | 12 | 0 | 1.8e-07 |
| best (attempt 1) | 639.17 | 10 | (17, 10) | 16 | 12 | 0 | 1.9e-07 |

Speedup: x1.05.

| attempt | outcome | fhe() time (ms) | note |
|---:|---|---:|---|
| 1 | ok | 639.17 | new best |
| 2 | ok | 652.99 | kernel 3ebf086b85 |
| 3 | ok | 643.95 | kernel 3ebf086b85 |
| 4 | chehab_error | - | CKKS scale mismatch: ciphertext operands of '+' (term 16) are at the same level 3 with different scales; align |
| 5 | ok | 639.97 | kernel 3ebf086b85 |
| 6 | ok | 640.42 | kernel 3ebf086b85 |
| 7 | ok | 644.06 | kernel 3ebf086b85 |
| 8 | ok | 639.63 | kernel 3ebf086b85 |
