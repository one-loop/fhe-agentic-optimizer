# chebyshev_mish_d7: LLM optimization run 20261101-105401

Model `claude-opus-5-5` (effort medium), 8 attempts, wall time 699 s, approximate API cost $0.0000.

| | fhe() time (ms) | levels | (depth, mult. depth) | ct-ct mul | ct-pt mul | rotations | max error |
|---|---:|---:|---|---:|---:|---:|---:|
| baseline | 182.22 | 8 | (22, 8) | 6 | 8 | 0 | 8.5e-08 |
| best (attempt 6) | 161.32 | 5 | (15, 5) | 5 | 6 | 0 | 1.3e-07 |

Speedup: x1.12 (baseline and best timed in 7 alternating batches; 1.00 when no candidate was confirmed faster).

| attempt | outcome | fhe() time (ms) | verdict | note |
|---:|---|---:|---|---|
| 1 | chehab_error | - | failed | CKKS scale mismatch: ciphertext operands of '+' (term 39) are at the same level 4 with different scales; align |
| 2 | ok | 175.50 | new_best, x1.04 vs attempt 0 |  |
| 3 | ok | 168.71 | new_best, x1.04 vs attempt 2 |  |
| 4 | ok | 165.90 | not_confirmed, x1.02 vs attempt 3 |  |
| 5 | ok | 166.67 | not_confirmed, x1.02 vs attempt 3 |  |
| 6 | ok | 161.32 | new_best, x1.04 vs attempt 3 |  |
| 7 | ok | - | same_as_best (attempt 6) |  |
| 8 | ok | 163.01 | slower |  |
