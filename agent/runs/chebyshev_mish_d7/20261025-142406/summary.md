# chebyshev_mish_d7: LLM optimization run 20261025-142406

Model `claude-sonnet-5-5` (effort medium), 8 attempts, wall time 455 s, approximate API cost $0.0000.

| | fhe() time (ms) | levels | (depth, mult. depth) | ct-ct mul | ct-pt mul | rotations | max error |
|---|---:|---:|---|---:|---:|---:|---:|
| baseline | 358.66 | 8 | (22, 8) | 6 | 8 | 0 | 1e-07 |
| best (attempt 7) | 186.56 | 10 | (24, 10) | 6 | 4 | 0 | 9.7e-08 |

Speedup: x1.92 (baseline and best timed in 7 alternating batches; 1.00 when no candidate was confirmed faster).

| attempt | outcome | fhe() time (ms) | verdict | note |
|---:|---|---:|---|---|
| 1 | chehab_error | - | failed | CKKS scale mismatch: ciphertext operands of '+' (term 37) are at the same level 4 with different scales; align |
| 2 | chehab_error | - | failed | CKKS scale mismatch: ciphertext operands of '+' (term 35) are at the same level 4 with different scales; align |
| 3 | ok | 251.79 | new_best, x1.42 vs attempt 0 |  |
| 4 | ok | 331.46 | slower |  |
| 5 | ok | 306.67 | slower |  |
| 6 | ok | - | same_as_best (attempt 3) |  |
| 7 | ok | 186.56 | new_best, x1.35 vs attempt 3 |  |
| 8 | ok | - | same_as_best (attempt 7) |  |
