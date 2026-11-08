# relu_composite: LLM optimization run 20261025-155052

Model `claude-sonnet-5-5` (effort medium), 8 attempts, wall time 722 s, approximate API cost $0.0000.

| | fhe() time (ms) | levels | (depth, mult. depth) | ct-ct mul | ct-pt mul | rotations | max error |
|---|---:|---:|---|---:|---:|---:|---:|
| baseline | 638.16 | 10 | (17, 10) | 16 | 12 | 0 | 1.7e-07 |
| best (attempt 0) | 638.16 | 10 | (17, 10) | 16 | 12 | 0 | 1.7e-07 |

Speedup: x1.00 (baseline and best timed in 7 alternating batches; 1.00 when no candidate was confirmed faster).

| attempt | outcome | fhe() time (ms) | verdict | note |
|---:|---|---:|---|---|
| 1 | ok | 635.85 | not_confirmed, x1.00 vs attempt 0 |  |
| 2 | chehab_error | - | failed | CKKS scale mismatch: ciphertext operands of '+' (term 17) are at the same level 3 with different scales; align |
| 3 | ok | 628.97 | not_confirmed, x1.01 vs attempt 0 |  |
| 4 | chehab_error | - | failed | CKKS scale mismatch: ciphertext operands of '+' (term 16) are at the same level 3 with different scales; align |
| 5 | ok | - | duplicate (attempt 3) |  |
| 6 | ok | 634.90 | not_confirmed, x1.01 vs attempt 0 |  |
| 7 | ok | 637.18 | not_confirmed, x1.01 vs attempt 0 |  |
| 8 | ok | 631.22 | not_confirmed, x1.01 vs attempt 0 |  |
