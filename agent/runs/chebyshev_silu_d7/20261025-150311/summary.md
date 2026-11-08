# chebyshev_silu_d7: LLM optimization run 20261025-150311

Model `claude-sonnet-5-5` (effort medium), 8 attempts, wall time 362 s, approximate API cost $0.0000.

| | fhe() time (ms) | levels | (depth, mult. depth) | ct-ct mul | ct-pt mul | rotations | max error |
|---|---:|---:|---|---:|---:|---:|---:|
| baseline | 300.65 | 7 | (19, 7) | 5 | 5 | 0 | 1e-07 |
| best (attempt 3) | 211.95 | 4 | (9, 4) | 3 | 4 | 0 | 1.1e-07 |

Speedup: x1.41 (baseline and best timed in 7 alternating batches; 1.00 when no candidate was confirmed faster).

| attempt | outcome | fhe() time (ms) | verdict | note |
|---:|---|---:|---|---|
| 1 | ok | 228.35 | new_best, x1.32 vs attempt 0 |  |
| 2 | chehab_error | - | failed | CKKS scale mismatch: ciphertext operands of '+' (term 25) are at the same level 4 with different scales; align |
| 3 | ok | 211.95 | new_best, x1.07 vs attempt 1 |  |
| 4 | ok | 213.39 | slower |  |
| 5 | ok | - | same_as_best (attempt 3) |  |
| 6 | ok | 213.26 | slower |  |
| 7 | ok | - | same_as_best (attempt 3) |  |
| 8 | ok | 216.51 | slower |  |
