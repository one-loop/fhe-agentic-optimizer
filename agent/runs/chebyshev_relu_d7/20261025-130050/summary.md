# chebyshev_relu_d7: LLM optimization run 20261025-130050

Model `claude-sonnet-5-5` (effort medium), 8 attempts, wall time 306 s, approximate API cost $0.0000.

| | fhe() time (ms) | levels | (depth, mult. depth) | ct-ct mul | ct-pt mul | rotations | max error |
|---|---:|---:|---|---:|---:|---:|---:|
| baseline | 152.34 | 7 | (19, 7) | 5 | 5 | 0 | 1.1e-07 |
| best (attempt 2) | 106.17 | 5 | (16, 5) | 3 | 4 | 0 | 1.2e-07 |

Speedup: x1.43 (baseline and best timed in 7 alternating batches; 1.00 when no candidate was confirmed faster).

| attempt | outcome | fhe() time (ms) | verdict | note |
|---:|---|---:|---|---|
| 1 | ok | 115.49 | new_best, x1.32 vs attempt 0 |  |
| 2 | ok | 106.17 | new_best, x1.09 vs attempt 1 |  |
| 3 | chehab_error | - | failed | CKKS scale mismatch: ciphertext operands of '+' (term 23) are at the same level 4 with different scales; align |
| 4 | ok | 108.77 | slower |  |
| 5 | ok | 118.44 | slower |  |
| 6 | ok | 102.98 | not_confirmed, x1.03 vs attempt 2 |  |
| 7 | ok | 107.16 | slower |  |
| 8 | ok | - | duplicate (attempt 6) |  |
