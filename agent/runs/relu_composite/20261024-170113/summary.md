# relu_composite: LLM optimization run 20261024-170113

Model `claude-sonnet-5-5` (effort medium), 8 attempts, wall time 614 s, approximate API cost $0.0000.

| | fhe() time (ms) | levels | (depth, mult. depth) | ct-ct mul | ct-pt mul | rotations | max error |
|---|---:|---:|---|---:|---:|---:|---:|
| baseline | 655.61 | 10 | (17, 10) | 16 | 12 | 0 | 3e-07 |
| best (attempt 0) | 655.61 | 10 | (17, 10) | 16 | 12 | 0 | 3e-07 |

Speedup: x1.00 (baseline and best timed in 7 alternating batches; 1.00 when no candidate was confirmed faster).

| attempt | outcome | fhe() time (ms) | verdict | note |
|---:|---|---:|---|---|
| 1 | ok | 682.39 | slower |  |
| 2 | chehab_error | - | failed | CKKS scale mismatch: ciphertext operands of '+' (term 16) are at the same level 3 with different scales; align |
| 3 | ok | - | same_as_baseline |  |
| 4 | ok | 639.35 | not_confirmed, x1.00 vs attempt 0 |  |
| 5 | ok | 649.07 | not_confirmed, x1.01 vs attempt 0 |  |
| 6 | ok | 642.86 | not_confirmed, x0.99 vs attempt 0 |  |
| 7 | ok | 665.64 | slower |  |
| 8 | ok | - | duplicate (attempt 4) |  |
