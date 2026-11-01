# relu_composite: LLM optimization run 20261101-141531

Model `claude-opus-5-5` (effort medium), 8 attempts, wall time 992 s, approximate API cost $0.0000.

| | fhe() time (ms) | levels | (depth, mult. depth) | ct-ct mul | ct-pt mul | rotations | max error |
|---|---:|---:|---|---:|---:|---:|---:|
| baseline | 321.07 | 10 | (17, 10) | 16 | 12 | 0 | 2.1e-07 |
| best (attempt 0) | 321.07 | 10 | (17, 10) | 16 | 12 | 0 | 2.1e-07 |

Speedup: x1.00 (baseline and best timed in 7 alternating batches; 1.00 when no candidate was confirmed faster).

| attempt | outcome | fhe() time (ms) | verdict | note |
|---:|---|---:|---|---|
| 1 | chehab_error | - | failed | CKKS scale mismatch: ciphertext operands of '+' (term 16) are at the same level 3 with different scales; align |
| 2 | ok | 326.06 | slower |  |
| 3 | ok | 319.41 | not_confirmed, x1.01 vs attempt 0 |  |
| 4 | ok | 317.38 | not_confirmed, x1.01 vs attempt 0 |  |
| 5 | ok | 315.30 | not_confirmed, x1.01 vs attempt 0 |  |
| 6 | ok | 319.32 | not_confirmed, x1.01 vs attempt 0 |  |
| 7 | ok | 316.50 | not_confirmed, x1.01 vs attempt 0 |  |
| 8 | ok | - | duplicate (attempt 5) |  |
