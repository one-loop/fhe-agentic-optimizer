# relu_composite: LLM optimization run 20261025-134507

Model `claude-sonnet-5-5` (effort medium), 8 attempts, wall time 433 s, approximate API cost $0.0000.

| | fhe() time (ms) | levels | (depth, mult. depth) | ct-ct mul | ct-pt mul | rotations | max error |
|---|---:|---:|---|---:|---:|---:|---:|
| baseline | 643.20 | 10 | (17, 10) | 16 | 12 | 0 | 1.5e-07 |
| best (attempt 0) | 643.20 | 10 | (17, 10) | 16 | 12 | 0 | 1.5e-07 |

Speedup: x1.00 (baseline and best timed in 7 alternating batches; 1.00 when no candidate was confirmed faster).

| attempt | outcome | fhe() time (ms) | verdict | note |
|---:|---|---:|---|---|
| 1 | ok | - | same_as_baseline |  |
| 2 | chehab_error | - | failed | CKKS scale mismatch: ciphertext operands of '+' (term 16) are at the same level 3 with different scales; align |
| 3 | chehab_error | - | failed | CKKS scale mismatch: ciphertext operands of '+' (term 17) are at the same level 3 with different scales; align |
| 4 | ok | 653.99 | slower |  |
| 5 | ok | 637.03 | not_confirmed, x1.00 vs attempt 0 |  |
| 6 | chehab_error | - | failed | CKKS scale mismatch: ciphertext operands of '+' (term 17) are at the same level 3 with different scales; align |
| 7 | ok | - | duplicate (attempt 5) |  |
| 8 | ok | 631.78 | not_confirmed, x1.01 vs attempt 0 |  |
