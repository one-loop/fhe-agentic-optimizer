# chebyshev_hardshrink_d7: LLM optimization run 20261025-162527

Model `claude-sonnet-5-5` (effort medium), 8 attempts, wall time 414 s, approximate API cost $0.0000.

| | fhe() time (ms) | levels | (depth, mult. depth) | ct-ct mul | ct-pt mul | rotations | max error |
|---|---:|---:|---|---:|---:|---:|---:|
| baseline | 320.33 | 8 | (21, 8) | 6 | 5 | 0 | 1e-07 |
| best (attempt 2) | 273.65 | 6 | (15, 6) | 4 | 5 | 0 | 8.9e-08 |

Speedup: x1.18 (baseline and best timed in 7 alternating batches; 1.00 when no candidate was confirmed faster).

| attempt | outcome | fhe() time (ms) | verdict | note |
|---:|---|---:|---|---|
| 1 | chehab_error | - | failed | CKKS scale mismatch: ciphertext operands of '+' (term 30) are at the same level 5 with different scales; align |
| 2 | ok | 273.65 | new_best, x1.19 vs attempt 0 |  |
| 3 | ok | 309.29 | slower |  |
| 4 | ok | 306.69 | slower |  |
| 5 | ok | 280.84 | slower |  |
| 6 | ok | 268.04 | not_confirmed, x1.00 vs attempt 2 |  |
| 7 | ok | - | duplicate (attempt 6) |  |
| 8 | ok | 270.46 | not_confirmed, x1.02 vs attempt 2 |  |
