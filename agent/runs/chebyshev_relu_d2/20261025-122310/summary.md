# chebyshev_relu_d2: LLM optimization run 20261025-122310

Model `claude-sonnet-5-5` (effort medium), 8 attempts, wall time 261 s, approximate API cost $0.0000.

| | fhe() time (ms) | levels | (depth, mult. depth) | ct-ct mul | ct-pt mul | rotations | max error |
|---|---:|---:|---|---:|---:|---:|---:|
| baseline | 104.55 | 3 | (7, 3) | 1 | 3 | 0 | 6.4e-08 |
| best (attempt 2) | 81.23 | 3 | (7, 3) | 1 | 2 | 0 | 1e-07 |

Speedup: x1.28 (baseline and best timed in 7 alternating batches; 1.00 when no candidate was confirmed faster).

| attempt | outcome | fhe() time (ms) | verdict | note |
|---:|---|---:|---|---|
| 1 | chehab_error | - | failed | CKKS scale mismatch: ciphertext operands of '+' (term 11) are at the same level 2 with different scales; align |
| 2 | ok | 81.23 | new_best, x1.26 vs attempt 0 |  |
| 3 | ok | 94.01 | slower |  |
| 4 | ok | - | same_as_best (attempt 2) |  |
| 5 | ok | 102.88 | slower |  |
| 6 | ok | - | same_as_best (attempt 2) |  |
| 7 | ok | - | same_as_best (attempt 2) |  |
| 8 | ok | - | same_as_best (attempt 2) |  |
