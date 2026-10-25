# chebyshev_softplus_d7: LLM optimization run 20261024-162113

Model `claude-sonnet-5-5` (effort medium), 8 attempts, wall time 344 s, approximate API cost $0.0000.

| | fhe() time (ms) | levels | (depth, mult. depth) | ct-ct mul | ct-pt mul | rotations | max error |
|---|---:|---:|---|---:|---:|---:|---:|
| baseline | 299.96 | 7 | (19, 7) | 5 | 5 | 0 | 8.5e-08 |
| best (attempt 2) | 205.17 | 5 | (14, 5) | 3 | 4 | 0 | 9.2e-08 |

Speedup: x1.47 (baseline and best timed in 7 alternating batches; 1.00 when no candidate was confirmed faster).

| attempt | outcome | fhe() time (ms) | verdict | note |
|---:|---|---:|---|---|
| 1 | ok | 228.31 | new_best, x1.32 vs attempt 0 |  |
| 2 | ok | 205.17 | new_best, x1.12 vs attempt 1 |  |
| 3 | chehab_error | - | failed | CKKS scale mismatch: ciphertext operands of '+' (term 25) are at the same level 4 with different scales; align |
| 4 | ok | 211.45 | slower |  |
| 5 | ok | - | duplicate (attempt 4) |  |
| 6 | ok | 208.51 | slower |  |
| 7 | ok | 212.17 | slower |  |
| 8 | ok | - | same_as_best (attempt 2) |  |
