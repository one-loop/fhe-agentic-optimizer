# chebyshev_elu_d7: LLM optimization run 20261108-155839

Model `claude-sonnet-5-5` (effort medium), 8 attempts, wall time 392 s, approximate API cost $0.0000.

| | fhe() time (ms) | levels | (depth, mult. depth) | ct-ct mul | ct-pt mul | rotations | max error |
|---|---:|---:|---|---:|---:|---:|---:|
| baseline | 241.78 | 8 | (22, 8) | 6 | 8 | 0 | 9.5e-08 |
| best (attempt 5) | 166.02 | 6 | (14, 6) | 4 | 10 | 0 | 1.2e-07 |

Speedup: x1.47 (baseline and best re-scored as in score.py: 10 interleaved processes each; 1.00 when no candidate was confirmed faster).

| attempt | outcome | fhe() time (ms) | verdict | note |
|---:|---|---:|---|---|
| 1 | chehab_error | - | failed | CKKS scale mismatch: ciphertext operands of '+' (term 26) are at the same level 4 with different scales; align |
| 2 | chehab_error | - | failed | CKKS scale mismatch: ciphertext operands of '+' (term 43) are at the same level 5 with different scales; align |
| 3 | ok | 189.40 | new_best, x1.38 vs attempt 0 |  |
| 4 | ok | 175.10 | new_best, x1.03 vs attempt 3 |  |
| 5 | ok | 166.02 | new_best, x1.06 vs attempt 4 |  |
| 6 | chehab_error | - | failed | CKKS scale mismatch: ciphertext operands of '+' (term 29) are at the same level 4 with different scales; align |
| 7 | ok | 165.22 | not_confirmed, x1.00 vs attempt 5 |  |
| 8 | ok | - | same_as_best (attempt 5) |  |
