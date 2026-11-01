# chebyshev_hardshrink_d7: LLM optimization run 20261025-124721

Model `claude-sonnet-5-5` (effort medium), 8 attempts, wall time 320 s, approximate API cost $0.0000.

| | fhe() time (ms) | levels | (depth, mult. depth) | ct-ct mul | ct-pt mul | rotations | max error |
|---|---:|---:|---|---:|---:|---:|---:|
| baseline | 161.53 | 8 | (21, 8) | 6 | 5 | 0 | 9.8e-08 |
| best (attempt 2) | 122.95 | 6 | (15, 6) | 4 | 4 | 0 | 7.9e-08 |

Speedup: x1.31 (baseline and best timed in 7 alternating batches; 1.00 when no candidate was confirmed faster).

| attempt | outcome | fhe() time (ms) | verdict | note |
|---:|---|---:|---|---|
| 1 | ok | 134.53 | new_best, x1.20 vs attempt 0 |  |
| 2 | ok | 122.95 | new_best, x1.10 vs attempt 1 |  |
| 3 | ok | 122.49 | not_confirmed, x1.00 vs attempt 2 |  |
| 4 | chehab_error | - | failed | CKKS scale mismatch: ciphertext operands of '+' (term 24) are at the same level 4 with different scales; align |
| 5 | ok | 122.65 | not_confirmed, x1.00 vs attempt 2 |  |
| 6 | ok | 129.50 | slower |  |
| 7 | ok | 122.33 | not_confirmed, x1.00 vs attempt 2 |  |
| 8 | ok | 132.22 | slower |  |
