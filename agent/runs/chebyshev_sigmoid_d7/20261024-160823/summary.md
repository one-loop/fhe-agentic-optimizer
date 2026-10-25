# chebyshev_sigmoid_d7: LLM optimization run 20261024-160823

Model `claude-sonnet-5-5` (effort medium), 8 attempts, wall time 364 s, approximate API cost $0.0000.

| | fhe() time (ms) | levels | (depth, mult. depth) | ct-ct mul | ct-pt mul | rotations | max error |
|---|---:|---:|---|---:|---:|---:|---:|
| baseline | 319.06 | 8 | (22, 8) | 6 | 5 | 0 | 4.2e-08 |
| best (attempt 4) | 253.45 | 6 | (17, 6) | 4 | 4 | 0 | 4.5e-08 |

Speedup: x1.26 (baseline and best timed in 7 alternating batches; 1.00 when no candidate was confirmed faster).

| attempt | outcome | fhe() time (ms) | verdict | note |
|---:|---|---:|---|---|
| 1 | chehab_error | - | failed | CKKS scale mismatch: ciphertext operands of '+' (term 31) are at the same level 5 with different scales; align |
| 2 | chehab_error | - | failed | CKKS scale mismatch: ciphertext operands of '+' (term 31) are at the same level 4 with different scales; align |
| 3 | ok | 398.82 | slower |  |
| 4 | ok | 253.45 | new_best, x1.26 vs attempt 0 |  |
| 5 | chehab_error | - | failed | CKKS scale mismatch: ciphertext operands of '+' (term 32) are at the same level 4 with different scales; align |
| 6 | ok | 264.87 | slower |  |
| 7 | ok | 254.32 | slower |  |
| 8 | ok | 250.90 | not_confirmed, x1.01 vs attempt 4 |  |
