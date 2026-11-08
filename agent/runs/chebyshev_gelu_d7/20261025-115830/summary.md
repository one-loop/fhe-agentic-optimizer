# chebyshev_gelu_d7: LLM optimization run 20261025-115830

Model `claude-sonnet-5-5` (effort medium), 8 attempts, wall time 461 s, approximate API cost $0.0000.

| | fhe() time (ms) | levels | (depth, mult. depth) | ct-ct mul | ct-pt mul | rotations | max error |
|---|---:|---:|---|---:|---:|---:|---:|
| baseline | 320.25 | 7 | (19, 7) | 5 | 5 | 0 | 1.3e-07 |
| best (attempt 6) | 214.56 | 5 | (14, 5) | 3 | 4 | 0 | 1.1e-07 |

Speedup: x1.44 (baseline and best timed in 7 alternating batches; 1.00 when no candidate was confirmed faster).

| attempt | outcome | fhe() time (ms) | verdict | note |
|---:|---|---:|---|---|
| 1 | ok | 239.36 | new_best, x1.33 vs attempt 0 |  |
| 2 | chehab_error | - | failed | CKKS scale mismatch: ciphertext operands of '+' (term 25) are at the same level 4 with different scales; align |
| 3 | ok | 220.91 | new_best, x1.09 vs attempt 1 |  |
| 4 | ok | 250.65 | slower |  |
| 5 | ok | 256.49 | slower |  |
| 6 | ok | 214.56 | new_best, x1.07 vs attempt 3 |  |
| 7 | ok | 205.05 | not_confirmed, x1.01 vs attempt 6 |  |
| 8 | ok | 210.87 | not_confirmed, x1.01 vs attempt 6 |  |
