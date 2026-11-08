# chebyshev_sigmoid_d7: LLM optimization run 20261108-164212

Model `claude-sonnet-5-5` (effort medium), 8 attempts, wall time 341 s, approximate API cost $0.0000.

| | fhe() time (ms) | levels | (depth, mult. depth) | ct-ct mul | ct-pt mul | rotations | max error |
|---|---:|---:|---|---:|---:|---:|---:|
| baseline | 217.24 | 8 | (22, 8) | 6 | 5 | 0 | 4.2e-08 |
| best (attempt 6) | 86.56 | 6 | (19, 6) | 4 | 2 | 0 | 7.2e-08 |

Speedup: x2.50 (baseline and best re-scored as in score.py: 10 interleaved processes each; 1.00 when no candidate was confirmed faster).

| attempt | outcome | fhe() time (ms) | verdict | note |
|---:|---|---:|---|---|
| 1 | chehab_error | - | failed | CKKS scale mismatch: ciphertext operands of '+' (term 34) are at the same level 5 with different scales; align |
| 2 | chehab_error | - | failed | CKKS scale mismatch: ciphertext operands of '+' (term 35) are at the same level 4 with different scales; align |
| 3 | ok | 144.33 | new_best, x1.59 vs attempt 0 |  |
| 4 | ok | 100.53 | new_best, x1.37 vs attempt 3 |  |
| 5 | ok | 95.67 | new_best, x1.07 vs attempt 4 |  |
| 6 | ok | 86.56 | new_best, x1.08 vs attempt 5 |  |
| 7 | ok | 86.74 | slower |  |
| 8 | ok | 84.92 | not_confirmed, x1.02 vs attempt 6 |  |
