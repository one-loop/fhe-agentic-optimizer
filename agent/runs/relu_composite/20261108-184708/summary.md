# relu_composite: LLM optimization run 20261108-184708

Model `claude-sonnet-5-5` (effort medium), 8 attempts, wall time 717 s, approximate API cost $0.0000.

| | fhe() time (ms) | levels | (depth, mult. depth) | ct-ct mul | ct-pt mul | rotations | max error |
|---|---:|---:|---|---:|---:|---:|---:|
| baseline | 641.09 | 10 | (17, 10) | 16 | 12 | 0 | 2.8e-07 |
| best (attempt 0) | 641.09 | 10 | (17, 10) | 16 | 12 | 0 | 2.8e-07 |

Speedup: x1.00 (baseline and best re-scored as in score.py: 10 interleaved processes each; 1.00 when no candidate was confirmed faster).

| attempt | outcome | fhe() time (ms) | verdict | note |
|---:|---|---:|---|---|
| 1 | ok | 635.64 | not_confirmed, x1.00 vs attempt 0 |  |
| 2 | chehab_error | - | failed | CKKS scale mismatch: ciphertext operands of '+' (term 16) are at the same level 3 with different scales; align |
| 3 | ok | - | duplicate (attempt 1) |  |
| 4 | chehab_error | - | failed | CKKS scale mismatch: ciphertext operands of '+' (term 17) are at the same level 3 with different scales; align |
| 5 | ok | 637.73 | not_confirmed, x1.01 vs attempt 0 |  |
| 6 | ok | 645.84 | slower |  |
| 7 | ok | 631.23 | not_confirmed, x1.01 vs attempt 0 |  |
| 8 | ok | 634.69 | not_confirmed, x1.01 vs attempt 0 |  |
