# chebyshev_hardshrink_d7: LLM optimization run 20261024-152523

Model `claude-sonnet-5-5` (effort medium), 8 attempts, wall time 544 s, approximate API cost $0.0000.

| | fhe() time (ms) | levels | (depth, mult. depth) | ct-ct mul | ct-pt mul | rotations | max error |
|---|---:|---:|---|---:|---:|---:|---:|
| baseline | 318.57 | 8 | (21, 8) | 6 | 5 | 0 | 9.3e-08 |
| best (attempt 1) | 264.24 | 5 | (16, 5) | 4 | 5 | 0 | 1.3e-07 |

Speedup: x1.21 (baseline and best timed in 7 alternating batches; 1.00 when no candidate was confirmed faster).

| attempt | outcome | fhe() time (ms) | verdict | note |
|---:|---|---:|---|---|
| 1 | ok | 264.24 | new_best, x1.21 vs attempt 0 |  |
| 2 | ok | 322.87 | slower |  |
| 3 | ok | 279.86 | slower |  |
| 4 | ok | - | same_as_best (attempt 1) |  |
| 5 | ok | 256.60 | not_confirmed, x1.02 vs attempt 1 |  |
| 6 | ok | 259.91 | not_confirmed, x1.02 vs attempt 1 |  |
| 7 | ok | 259.60 | not_confirmed, x1.02 vs attempt 1 |  |
| 8 | chehab_error | - | failed | CKKS scale mismatch: ciphertext operands of '+' (term 42) are at the same level 5 with different scales; align |
