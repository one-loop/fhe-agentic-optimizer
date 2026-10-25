# chebyshev_elu_d7: LLM optimization run 20261024-151205

Model `claude-sonnet-5-5` (effort medium), 8 attempts, wall time 431 s, approximate API cost $0.0000.

| | fhe() time (ms) | levels | (depth, mult. depth) | ct-ct mul | ct-pt mul | rotations | max error |
|---|---:|---:|---|---:|---:|---:|---:|
| baseline | 358.28 | 8 | (22, 8) | 6 | 8 | 0 | 1.1e-07 |
| best (attempt 4) | 254.67 | 8 | (27, 8) | 6 | 2 | 0 | 1e-07 |

Speedup: x1.42 (baseline and best timed in 7 alternating batches; 1.00 when no candidate was confirmed faster).

| attempt | outcome | fhe() time (ms) | verdict | note |
|---:|---|---:|---|---|
| 1 | chehab_error | - | failed | CKKS scale mismatch: ciphertext operands of '+' (term 37) are at the same level 4 with different scales; align |
| 2 | chehab_error | - | failed | CKKS scale mismatch: ciphertext operands of '+' (term 43) are at the same level 6 with different scales; align |
| 3 | ok | 386.61 | slower |  |
| 4 | ok | 254.67 | new_best, x1.42 vs attempt 0 |  |
| 5 | ok | 329.92 | slower |  |
| 6 | ok | 251.50 | not_confirmed, x1.00 vs attempt 4 |  |
| 7 | ok | 306.24 | slower |  |
| 8 | ok | 253.00 | not_confirmed, x1.00 vs attempt 4 |  |
