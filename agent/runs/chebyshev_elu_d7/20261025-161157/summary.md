# chebyshev_elu_d7: LLM optimization run 20261025-161157

Model `claude-sonnet-5-5` (effort medium), 8 attempts, wall time 359 s, approximate API cost $0.0000.

| | fhe() time (ms) | levels | (depth, mult. depth) | ct-ct mul | ct-pt mul | rotations | max error |
|---|---:|---:|---|---:|---:|---:|---:|
| baseline | 357.89 | 8 | (22, 8) | 6 | 8 | 0 | 1.1e-07 |
| best (attempt 3) | 331.26 | 5 | (13, 5) | 4 | 9 | 0 | 1.1e-07 |

Speedup: x1.08 (baseline and best timed in 7 alternating batches; 1.00 when no candidate was confirmed faster).

| attempt | outcome | fhe() time (ms) | verdict | note |
|---:|---|---:|---|---|
| 1 | chehab_error | - | failed | CKKS scale mismatch: ciphertext operands of '+' (term 37) are at the same level 4 with different scales; align |
| 2 | chehab_error | - | failed | CKKS scale mismatch: ciphertext operands of '+' (term 38) are at the same level 5 with different scales; align |
| 3 | ok | 331.26 | new_best, x1.08 vs attempt 0 |  |
| 4 | ok | 337.31 | slower |  |
| 5 | ok | 332.47 | slower |  |
| 6 | ok | 335.25 | slower |  |
| 7 | ok | 334.70 | slower |  |
| 8 | ok | 331.49 | slower |  |
