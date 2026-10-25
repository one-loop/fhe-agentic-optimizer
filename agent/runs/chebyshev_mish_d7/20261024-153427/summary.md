# chebyshev_mish_d7: LLM optimization run 20261024-153427

Model `claude-sonnet-5-5` (effort medium), 8 attempts, wall time 370 s, approximate API cost $0.0000.

| | fhe() time (ms) | levels | (depth, mult. depth) | ct-ct mul | ct-pt mul | rotations | max error |
|---|---:|---:|---|---:|---:|---:|---:|
| baseline | 357.94 | 8 | (22, 8) | 6 | 8 | 0 | 8.4e-08 |
| best (attempt 3) | 252.46 | 8 | (27, 8) | 6 | 2 | 0 | 8.5e-08 |

Speedup: x1.42 (baseline and best timed in 7 alternating batches; 1.00 when no candidate was confirmed faster).

| attempt | outcome | fhe() time (ms) | verdict | note |
|---:|---|---:|---|---|
| 1 | chehab_error | - | failed | CKKS scale mismatch: ciphertext operands of '+' (term 37) are at the same level 4 with different scales; align |
| 2 | chehab_error | - | failed | CKKS scale mismatch: ciphertext operands of '+' (term 41) are at the same level 6 with different scales; align |
| 3 | ok | 252.46 | new_best, x1.42 vs attempt 0 |  |
| 4 | ok | 348.99 | slower |  |
| 5 | ok | 251.85 | not_confirmed, x1.00 vs attempt 3 |  |
| 6 | ok | 304.98 | slower |  |
| 7 | ok | 309.96 | slower |  |
| 8 | ok | 296.45 | slower |  |
