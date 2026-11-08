# chebyshev_mish_d7: LLM optimization run 20261025-163221

Model `claude-sonnet-5-5` (effort medium), 8 attempts, wall time 518 s, approximate API cost $0.0000.

| | fhe() time (ms) | levels | (depth, mult. depth) | ct-ct mul | ct-pt mul | rotations | max error |
|---|---:|---:|---|---:|---:|---:|---:|
| baseline | 383.98 | 8 | (22, 8) | 6 | 8 | 0 | 1e-07 |
| best (attempt 5) | 338.19 | 5 | (18, 5) | 5 | 6 | 0 | 8.9e-08 |

Speedup: x1.09 (baseline and best timed in 7 alternating batches; 1.00 when no candidate was confirmed faster).

| attempt | outcome | fhe() time (ms) | verdict | note |
|---:|---|---:|---|---|
| 1 | chehab_error | - | failed | CKKS scale mismatch: ciphertext operands of '+' (term 37) are at the same level 4 with different scales; align |
| 2 | chehab_error | - | failed | CKKS scale mismatch: ciphertext operands of '+' (term 40) are at the same level 5 with different scales; align |
| 3 | ok | 371.92 | not_confirmed, x0.96 vs attempt 0 |  |
| 4 | ok | 393.69 | slower |  |
| 5 | ok | 338.19 | new_best, x1.09 vs attempt 0 |  |
| 6 | ok | 358.80 | slower |  |
| 7 | ok | 375.54 | slower |  |
| 8 | ok | 345.72 | slower |  |
