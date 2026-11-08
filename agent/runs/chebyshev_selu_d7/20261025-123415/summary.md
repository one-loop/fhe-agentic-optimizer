# chebyshev_selu_d7: LLM optimization run 20261025-123415

Model `claude-sonnet-5-5` (effort medium), 8 attempts, wall time 415 s, approximate API cost $0.0000.

| | fhe() time (ms) | levels | (depth, mult. depth) | ct-ct mul | ct-pt mul | rotations | max error |
|---|---:|---:|---|---:|---:|---:|---:|
| baseline | 358.61 | 8 | (22, 8) | 6 | 8 | 0 | 1e-07 |
| best (attempt 2) | 345.59 | 5 | (14, 5) | 4 | 9 | 0 | 1e-07 |

Speedup: x1.04 (baseline and best timed in 7 alternating batches; 1.00 when no candidate was confirmed faster).

| attempt | outcome | fhe() time (ms) | verdict | note |
|---:|---|---:|---|---|
| 1 | chehab_error | - | failed | CKKS scale mismatch: ciphertext operands of '+' (term 38) are at the same level 4 with different scales; align |
| 2 | ok | 345.59 | new_best, x1.04 vs attempt 0 |  |
| 3 | ok | 341.93 | not_confirmed, x1.02 vs attempt 2 |  |
| 4 | ok | 374.25 | slower |  |
| 5 | ok | 351.06 | slower |  |
| 6 | ok | 347.74 | slower |  |
| 7 | ok | - | same_as_best (attempt 2) |  |
| 8 | ok | 348.79 | slower |  |
