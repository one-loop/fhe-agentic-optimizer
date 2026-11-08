# chebyshev_sigmoid_d7: LLM optimization run 20261025-124755

Model `claude-sonnet-5-5` (effort medium), 8 attempts, wall time 386 s, approximate API cost $0.0000.

| | fhe() time (ms) | levels | (depth, mult. depth) | ct-ct mul | ct-pt mul | rotations | max error |
|---|---:|---:|---|---:|---:|---:|---:|
| baseline | 339.73 | 8 | (22, 8) | 6 | 5 | 0 | 4.3e-08 |
| best (attempt 3) | 225.45 | 6 | (16, 6) | 4 | 3 | 0 | 4.3e-08 |

Speedup: x1.42 (baseline and best timed in 7 alternating batches; 1.00 when no candidate was confirmed faster).

| attempt | outcome | fhe() time (ms) | verdict | note |
|---:|---|---:|---|---|
| 1 | chehab_error | - | failed | CKKS scale mismatch: ciphertext operands of '+' (term 31) are at the same level 5 with different scales; align |
| 2 | ok | 247.18 | new_best, x1.31 vs attempt 0 |  |
| 3 | ok | 225.45 | new_best, x1.09 vs attempt 2 |  |
| 4 | ok | 225.46 | slower |  |
| 5 | ok | 252.09 | slower |  |
| 6 | chehab_error | - | failed | CKKS scale mismatch: ciphertext operands of '+' (term 29) are at the same level 5 with different scales; align |
| 7 | ok | 230.25 | slower |  |
| 8 | ok | 225.21 | not_confirmed, x1.00 vs attempt 3 |  |
