# chebyshev_relu_d7: LLM optimization run 20261025-143620

Model `claude-sonnet-5-5` (effort medium), 8 attempts, wall time 315 s, approximate API cost $0.0000.

| | fhe() time (ms) | levels | (depth, mult. depth) | ct-ct mul | ct-pt mul | rotations | max error |
|---|---:|---:|---|---:|---:|---:|---:|
| baseline | 301.79 | 7 | (19, 7) | 5 | 5 | 0 | 9.4e-08 |
| best (attempt 2) | 213.56 | 4 | (11, 4) | 3 | 4 | 0 | 1.1e-07 |

Speedup: x1.42 (baseline and best timed in 7 alternating batches; 1.00 when no candidate was confirmed faster).

| attempt | outcome | fhe() time (ms) | verdict | note |
|---:|---|---:|---|---|
| 1 | chehab_error | - | failed | CKKS scale mismatch: ciphertext operands of '+' (term 24) are at the same level 4 with different scales; align |
| 2 | ok | 213.56 | new_best, x1.42 vs attempt 0 |  |
| 3 | ok | - | same_as_best (attempt 2) |  |
| 4 | ok | 215.09 | slower |  |
| 5 | ok | 219.69 | slower |  |
| 6 | ok | - | same_as_best (attempt 2) |  |
| 7 | ok | - | same_as_best (attempt 2) |  |
| 8 | ok | - | same_as_best (attempt 2) |  |
