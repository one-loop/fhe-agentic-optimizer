# chebyshev_hardshrink_d7: LLM optimization run 20261108-160936

Model `claude-sonnet-5-5` (effort medium), 8 attempts, wall time 393 s, approximate API cost $0.0000.

| | fhe() time (ms) | levels | (depth, mult. depth) | ct-ct mul | ct-pt mul | rotations | max error |
|---|---:|---:|---|---:|---:|---:|---:|
| baseline | 215.54 | 8 | (21, 8) | 6 | 5 | 0 | 1.6e-07 |
| best (attempt 6) | 94.14 | 5 | (14, 5) | 4 | 5 | 0 | 1.1e-07 |

Speedup: x2.33 (baseline and best re-scored as in score.py: 10 interleaved processes each; 1.00 when no candidate was confirmed faster).

| attempt | outcome | fhe() time (ms) | verdict | note |
|---:|---|---:|---|---|
| 1 | chehab_error | - | failed | CKKS scale mismatch: ciphertext operands of '+' (term 30) are at the same level 5 with different scales; align |
| 2 | chehab_error | - | failed | CKKS scale mismatch: ciphertext operands of '+' (term 31) are at the same level 4 with different scales; align |
| 3 | ok | 141.63 | new_best, x1.52 vs attempt 0 |  |
| 4 | ok | 121.28 | new_best, x1.17 vs attempt 3 |  |
| 5 | ok | 102.34 | new_best, x1.20 vs attempt 4 |  |
| 6 | ok | 94.14 | new_best, x1.09 vs attempt 5 |  |
| 7 | ok | - | same_as_best (attempt 6) |  |
| 8 | ok | 93.47 | not_confirmed, x1.02 vs attempt 6 |  |
