# chebyshev_gelu_d7: LLM optimization run 20261025-124147

Model `claude-sonnet-5-5` (effort medium), 8 attempts, wall time 334 s, approximate API cost $0.0000.

| | fhe() time (ms) | levels | (depth, mult. depth) | ct-ct mul | ct-pt mul | rotations | max error |
|---|---:|---:|---|---:|---:|---:|---:|
| baseline | 152.72 | 7 | (19, 7) | 5 | 5 | 0 | 1.1e-07 |
| best (attempt 4) | 97.94 | 5 | (16, 5) | 3 | 3 | 0 | 1.2e-07 |

Speedup: x1.55 (baseline and best timed in 7 alternating batches; 1.00 when no candidate was confirmed faster).

| attempt | outcome | fhe() time (ms) | verdict | note |
|---:|---|---:|---|---|
| 1 | chehab_error | - | failed | CKKS scale mismatch: ciphertext operands of '+' (term 23) are at the same level 4 with different scales; align |
| 2 | ok | 107.58 | new_best, x1.41 vs attempt 0 |  |
| 3 | ok | 106.38 | not_confirmed, x1.01 vs attempt 2 |  |
| 4 | ok | 97.94 | new_best, x1.10 vs attempt 2 |  |
| 5 | ok | 96.37 | not_confirmed, x1.02 vs attempt 4 |  |
| 6 | ok | 96.23 | not_confirmed, x1.02 vs attempt 4 |  |
| 7 | ok | 96.89 | not_confirmed, x1.01 vs attempt 4 |  |
| 8 | ok | - | duplicate (attempt 7) |  |
