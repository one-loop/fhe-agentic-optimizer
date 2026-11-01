# relu_composite: LLM optimization run 20261025-140616

Model `claude-sonnet-5-5` (effort medium), 8 attempts, wall time 452 s, approximate API cost $0.0000.

| | fhe() time (ms) | levels | (depth, mult. depth) | ct-ct mul | ct-pt mul | rotations | max error |
|---|---:|---:|---|---:|---:|---:|---:|
| baseline | 319.24 | 10 | (17, 10) | 16 | 12 | 0 | 1.6e-07 |
| best (attempt 0) | 319.24 | 10 | (17, 10) | 16 | 12 | 0 | 1.6e-07 |

Speedup: x1.00 (baseline and best timed in 7 alternating batches; 1.00 when no candidate was confirmed faster).

| attempt | outcome | fhe() time (ms) | verdict | note |
|---:|---|---:|---|---|
| 1 | ok | 328.51 | slower |  |
| 2 | ok | 317.48 | not_confirmed, x1.01 vs attempt 0 |  |
| 3 | ok | 313.54 | not_confirmed, x1.02 vs attempt 0 |  |
| 4 | chehab_error | - | failed | CKKS scale mismatch: ciphertext operands of '+' (term 46) are at the same level 9 with different scales; align |
| 5 | ok | 315.85 | not_confirmed, x1.01 vs attempt 0 |  |
| 6 | chehab_error | - | failed | CKKS scale mismatch: ciphertext operands of '+' (term 16) are at the same level 3 with different scales; align |
| 7 | ok | 314.83 | not_confirmed, x1.01 vs attempt 0 |  |
| 8 | ok | - | duplicate (attempt 5) |  |
