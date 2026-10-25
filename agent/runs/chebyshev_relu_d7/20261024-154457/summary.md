# chebyshev_relu_d7: LLM optimization run 20261024-154457

Model `claude-sonnet-5-5` (effort medium), 8 attempts, wall time 425 s, approximate API cost $0.0000.

| | fhe() time (ms) | levels | (depth, mult. depth) | ct-ct mul | ct-pt mul | rotations | max error |
|---|---:|---:|---|---:|---:|---:|---:|
| baseline | 300.04 | 7 | (19, 7) | 5 | 5 | 0 | 9e-08 |
| best (attempt 3) | 212.85 | 4 | (10, 4) | 3 | 4 | 0 | 9.3e-08 |

Speedup: x1.41 (baseline and best timed in 7 alternating batches; 1.00 when no candidate was confirmed faster).

| attempt | outcome | fhe() time (ms) | verdict | note |
|---:|---|---:|---|---|
| 1 | ok | 233.95 | new_best, x1.31 vs attempt 0 |  |
| 2 | chehab_error | - | failed | CKKS scale mismatch: ciphertext operands of '+' (term 25) are at the same level 4 with different scales; align |
| 3 | ok | 212.85 | new_best, x1.08 vs attempt 1 |  |
| 4 | ok | - | same_as_best (attempt 3) |  |
| 5 | ok | 210.98 | not_confirmed, x1.01 vs attempt 3 |  |
| 6 | ok | 212.93 | slower |  |
| 7 | ok | 210.91 | not_confirmed, x1.01 vs attempt 3 |  |
| 8 | ok | 214.15 | slower |  |
