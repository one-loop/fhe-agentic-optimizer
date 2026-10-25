# chebyshev_selu_d7: LLM optimization run 20261024-155202

Model `claude-sonnet-5-5` (effort medium), 8 attempts, wall time 472 s, approximate API cost $0.0000.

| | fhe() time (ms) | levels | (depth, mult. depth) | ct-ct mul | ct-pt mul | rotations | max error |
|---|---:|---:|---|---:|---:|---:|---:|
| baseline | 358.24 | 8 | (22, 8) | 6 | 8 | 0 | 1.6e-07 |
| best (attempt 2) | 301.94 | 6 | (17, 6) | 4 | 7 | 0 | 9.9e-08 |

Speedup: x1.19 (baseline and best timed in 7 alternating batches; 1.00 when no candidate was confirmed faster).

| attempt | outcome | fhe() time (ms) | verdict | note |
|---:|---|---:|---|---|
| 1 | ok | 351.87 | not_confirmed, x1.02 vs attempt 0 |  |
| 2 | ok | 301.94 | new_best, x1.19 vs attempt 0 |  |
| 3 | chehab_error | - | failed | CKKS scale mismatch: ciphertext operands of '+' (term 42) are at the same level 5 with different scales; align |
| 4 | chehab_error | - | failed | CKKS scale mismatch: ciphertext operands of '+' (term 42) are at the same level 5 with different scales; align |
| 5 | ok | 305.23 | slower |  |
| 6 | ok | - | same_as_best (attempt 2) |  |
| 7 | ok | 333.44 | slower |  |
| 8 | ok | 310.22 | slower |  |
