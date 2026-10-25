# chebyshev_hardshrink_d7: LLM optimization run 20261024-120659

Model `claude-sonnet-5-5` (effort medium), 8 attempts, wall time 246 s, approximate API cost $0.0000.

| | fhe() time (ms) | levels | (depth, mult. depth) | ct-ct mul | ct-pt mul | rotations | max error |
|---|---:|---:|---|---:|---:|---:|---:|
| baseline | 319.79 | 8 | (21, 8) | 6 | 5 | 0 | 9.3e-08 |
| best (attempt 2) | 286.11 | 5 | (13, 5) | 4 | 6 | 0 | 1.1e-07 |

Speedup: x1.12.

| attempt | outcome | fhe() time (ms) | note |
|---:|---|---:|---|
| 1 | chehab_error | - | CKKS scale mismatch: ciphertext operands of '+' (term 30) are at the same level 5 with different scales; align |
| 2 | ok | 286.11 | new best |
| 3 | ok | 292.94 | kernel 1feea3aecb |
| 4 | ok | 344.64 | kernel 511481316a |
| 5 | ok | 289.88 | kernel 511481316a |
| 6 | ok | 321.29 | kernel 511481316a |
| 7 | ok | 289.07 | kernel 511481316a |
| 8 | ok | 295.38 | kernel 511481316a |
