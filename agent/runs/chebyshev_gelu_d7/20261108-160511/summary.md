# chebyshev_gelu_d7: LLM optimization run 20261108-160511

Model `claude-sonnet-5-5` (effort medium), 8 attempts, wall time 265 s, approximate API cost $0.0000.

| | fhe() time (ms) | levels | (depth, mult. depth) | ct-ct mul | ct-pt mul | rotations | max error |
|---|---:|---:|---|---:|---:|---:|---:|
| baseline | 166.62 | 7 | (19, 7) | 5 | 5 | 0 | 9.4e-08 |
| best (attempt 7) | 56.68 | 4 | (12, 4) | 3 | 4 | 0 | 9.5e-08 |

Speedup: x2.86 (baseline and best re-scored as in score.py: 10 interleaved processes each; 1.00 when no candidate was confirmed faster).

| attempt | outcome | fhe() time (ms) | verdict | note |
|---:|---|---:|---|---|
| 1 | chehab_error | - | failed | CKKS scale mismatch: ciphertext operands of '+' (term 25) are at the same level 4 with different scales; align |
| 2 | chehab_error | - | failed | CKKS scale mismatch: ciphertext operands of '+' (term 26) are at the same level 4 with different scales; align |
| 3 | ok | 82.32 | new_best, x2.02 vs attempt 0 |  |
| 4 | ok | 59.01 | new_best, x1.41 vs attempt 3 |  |
| 5 | ok | 59.94 | slower |  |
| 6 | ok | 57.55 | not_confirmed, x1.02 vs attempt 4 |  |
| 7 | ok | 56.68 | new_best, x1.04 vs attempt 4 |  |
| 8 | ok | - | same_as_best (attempt 7) |  |
