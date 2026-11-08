# chebyshev_selu_d7: LLM optimization run 20261025-144134

Model `claude-sonnet-5-5` (effort medium), 8 attempts, wall time 408 s, approximate API cost $0.0000.

| | fhe() time (ms) | levels | (depth, mult. depth) | ct-ct mul | ct-pt mul | rotations | max error |
|---|---:|---:|---|---:|---:|---:|---:|
| baseline | 358.09 | 8 | (22, 8) | 6 | 8 | 0 | 6.6e-08 |
| best (attempt 5) | 330.21 | 5 | (13, 5) | 4 | 9 | 0 | 1.2e-07 |

Speedup: x1.08 (baseline and best timed in 7 alternating batches; 1.00 when no candidate was confirmed faster).

| attempt | outcome | fhe() time (ms) | verdict | note |
|---:|---|---:|---|---|
| 1 | chehab_error | - | failed | CKKS scale mismatch: ciphertext operands of '+' (term 37) are at the same level 4 with different scales; align |
| 2 | chehab_error | - | failed | CKKS scale mismatch: ciphertext operands of '+' (term 33) are at the same level 5 with different scales; align |
| 3 | chehab_error | - | failed | CKKS scale mismatch: ciphertext operands of '+' (term 33) are at the same level 5 with different scales; align |
| 4 | ok | 432.98 | slower |  |
| 5 | ok | 330.21 | new_best, x1.08 vs attempt 0 |  |
| 6 | ok | 344.73 | slower |  |
| 7 | ok | 325.34 | not_confirmed, x1.02 vs attempt 5 |  |
| 8 | ok | - | duplicate (attempt 7) |  |
