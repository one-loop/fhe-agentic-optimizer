# chebyshev_softplus_d7: LLM optimization run 20261025-132810

Model `claude-sonnet-5-5` (effort medium), 8 attempts, wall time 352 s, approximate API cost $0.0000.

| | fhe() time (ms) | levels | (depth, mult. depth) | ct-ct mul | ct-pt mul | rotations | max error |
|---|---:|---:|---|---:|---:|---:|---:|
| baseline | 153.20 | 7 | (19, 7) | 5 | 5 | 0 | 9.8e-08 |
| best (attempt 5) | 97.66 | 5 | (18, 5) | 3 | 3 | 0 | 9.6e-08 |

Speedup: x1.56 (baseline and best timed in 7 alternating batches; 1.00 when no candidate was confirmed faster).

| attempt | outcome | fhe() time (ms) | verdict | note |
|---:|---|---:|---|---|
| 1 | ok | 115.72 | new_best, x1.32 vs attempt 0 |  |
| 2 | chehab_error | - | failed | CKKS scale mismatch: ciphertext operands of '+' (term 23) are at the same level 4 with different scales; align |
| 3 | ok | 111.95 | new_best, x1.03 vs attempt 1 |  |
| 4 | ok | - | same_as_best (attempt 3) |  |
| 5 | ok | 97.66 | new_best, x1.14 vs attempt 3 |  |
| 6 | ok | 96.07 | not_confirmed, x1.02 vs attempt 5 |  |
| 7 | ok | - | duplicate (attempt 6) |  |
| 8 | ok | 100.09 | slower |  |
