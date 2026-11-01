# chebyshev_selu_d7: LLM optimization run 20261101-111733

Model `claude-opus-5-5` (effort medium), 8 attempts, wall time 1155 s, approximate API cost $0.0000.

| | fhe() time (ms) | levels | (depth, mult. depth) | ct-ct mul | ct-pt mul | rotations | max error |
|---|---:|---:|---|---:|---:|---:|---:|
| baseline | 182.14 | 8 | (22, 8) | 6 | 8 | 0 | 1.3e-07 |
| best (attempt 8) | 158.72 | 5 | (18, 5) | 4 | 9 | 0 | 1.3e-07 |

Speedup: x1.17 (baseline and best timed in 7 alternating batches; 1.00 when no candidate was confirmed faster).

| attempt | outcome | fhe() time (ms) | verdict | note |
|---:|---|---:|---|---|
| 1 | ok | 169.61 | new_best, x1.07 vs attempt 0 |  |
| 2 | ok | 167.16 | not_confirmed, x1.01 vs attempt 1 |  |
| 3 | chehab_error | - | failed | CKKS scale mismatch: ciphertext operands of '+' (term 32) are at the same level 3 with different scales; align |
| 4 | ok | 178.21 | slower |  |
| 5 | ok | 163.78 | not_confirmed, x1.02 vs attempt 1 |  |
| 6 | ok | 163.40 | not_confirmed, x1.03 vs attempt 1 |  |
| 7 | ok | 161.78 | not_confirmed, x1.03 vs attempt 1 |  |
| 8 | ok | 158.72 | new_best, x1.09 vs attempt 1 |  |
