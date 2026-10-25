# chebyshev_sigmoid_d5: LLM optimization run 20261024-122610

Model `claude-sonnet-5-5` (effort medium), 8 attempts, wall time 253 s, approximate API cost $0.0000.

| | fhe() time (ms) | levels | (depth, mult. depth) | ct-ct mul | ct-pt mul | rotations | max error |
|---|---:|---:|---|---:|---:|---:|---:|
| baseline | 255.23 | 6 | (16, 6) | 4 | 4 | 0 | 4.7e-08 |
| best (attempt 3) | 227.53 | 5 | (13, 5) | 3 | 4 | 0 | 5.3e-08 |

Speedup: x1.12.

| attempt | outcome | fhe() time (ms) | note |
|---:|---|---:|---|
| 1 | ok | 267.48 | kernel 4099c393ad |
| 2 | chehab_error | - | CKKS scale mismatch: ciphertext operands of '+' (term 32) are at the same level 4 with different scales; align |
| 3 | ok | 227.53 | new best |
| 4 | ok | 234.48 | kernel e2a366694c |
| 5 | ok | 233.93 | kernel 49ef9f2511 |
| 6 | ok | 233.25 | kernel 14a5a7eb2c |
| 7 | ok | 244.02 | kernel 14a5a7eb2c |
| 8 | ok | 233.39 | kernel 14a5a7eb2c |
