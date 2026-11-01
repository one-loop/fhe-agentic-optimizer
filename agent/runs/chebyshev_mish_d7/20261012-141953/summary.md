# chebyshev_mish_d7: LLM optimization run 20261012-141953

Model `claude-haiku-5-5` (effort medium), 8 attempts, wall time 210 s, approximate API cost $0.0208.

| | fhe() time (ms) | levels | (depth, mult. depth) | ct-ct mul | ct-pt mul | rotations | max error |
|---|---:|---:|---|---:|---:|---:|---:|
| baseline | 181.92 | 8 | (22, 8) | 6 | 8 | 0 | 9.8e-08 |
| best (attempt 3) | 168.22 | 5 | (13, 5) | 4 | 9 | 0 | 1.2e-07 |

Speedup: x1.08.

| attempt | outcome | fhe() time (ms) | note |
|---:|---|---:|---|
| 1 | chehab_error | - | CKKS scale mismatch: ciphertext operands of '+' (term 34) are at the same level 4 with different scales; align |
| 2 | chehab_error | - | CKKS scale mismatch: ciphertext operands of '+' (term 34) are at the same level 4 with different scales; align |
| 3 | ok | 168.22 | new best |
| 4 | ok | 168.51 | kernel aa5fab115a |
| 5 | ok | 169.17 | kernel aa5fab115a |
| 6 | ok | 176.01 | kernel aa5fab115a |
| 7 | ok | 169.49 | kernel aa5fab115a |
| 8 | ok | 169.57 | kernel aa5fab115a |
