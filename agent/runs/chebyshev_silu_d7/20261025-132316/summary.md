# chebyshev_silu_d7: LLM optimization run 20261025-132316

Model `claude-sonnet-5-5` (effort medium), 8 attempts, wall time 293 s, approximate API cost $0.0000.

| | fhe() time (ms) | levels | (depth, mult. depth) | ct-ct mul | ct-pt mul | rotations | max error |
|---|---:|---:|---|---:|---:|---:|---:|
| baseline | 153.45 | 7 | (19, 7) | 5 | 5 | 0 | 8.3e-08 |
| best (attempt 4) | 105.86 | 5 | (15, 5) | 3 | 4 | 0 | 9.1e-08 |

Speedup: x1.44 (baseline and best timed in 7 alternating batches; 1.00 when no candidate was confirmed faster).

| attempt | outcome | fhe() time (ms) | verdict | note |
|---:|---|---:|---|---|
| 1 | ok | 115.64 | new_best, x1.32 vs attempt 0 |  |
| 2 | chehab_error | - | failed | CKKS scale mismatch: ciphertext operands of '+' (term 25) are at the same level 4 with different scales; align |
| 3 | ok | 118.22 | slower |  |
| 4 | ok | 105.86 | new_best, x1.09 vs attempt 1 |  |
| 5 | ok | 105.15 | not_confirmed, x1.01 vs attempt 4 |  |
| 6 | ok | 107.16 | slower |  |
| 7 | ok | 105.25 | not_confirmed, x1.01 vs attempt 4 |  |
| 8 | ok | 106.49 | slower |  |
