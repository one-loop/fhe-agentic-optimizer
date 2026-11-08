# chebyshev_elu_d7: LLM optimization run 20261025-140110

Model `claude-sonnet-5-5` (effort medium), 8 attempts, wall time 493 s, approximate API cost $0.0000.

| | fhe() time (ms) | levels | (depth, mult. depth) | ct-ct mul | ct-pt mul | rotations | max error |
|---|---:|---:|---|---:|---:|---:|---:|
| baseline | 380.88 | 8 | (22, 8) | 6 | 8 | 0 | 7.3e-08 |
| best (attempt 4) | 251.73 | 8 | (22, 8) | 6 | 2 | 0 | 1.1e-07 |

Speedup: x1.42 (baseline and best timed in 7 alternating batches; 1.00 when no candidate was confirmed faster).

| attempt | outcome | fhe() time (ms) | verdict | note |
|---:|---|---:|---|---|
| 1 | ok | 354.95 | not_confirmed, x1.01 vs attempt 0 |  |
| 2 | ok | 351.14 | not_confirmed, x1.02 vs attempt 0 |  |
| 3 | ok | - | duplicate (attempt 2) |  |
| 4 | ok | 251.73 | new_best, x1.42 vs attempt 0 |  |
| 5 | ok | 310.93 | slower |  |
| 6 | ok | 251.88 | slower |  |
| 7 | ok | 252.34 | slower |  |
| 8 | chehab_error | - | failed | CKKS scale mismatch: ciphertext operands of '+' (term 36) are at the same level 5 with different scales; align |
