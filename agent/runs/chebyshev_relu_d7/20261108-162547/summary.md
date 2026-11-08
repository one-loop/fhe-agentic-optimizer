# chebyshev_relu_d7: LLM optimization run 20261108-162547

Model `claude-sonnet-5-5` (effort medium), 8 attempts, wall time 250 s, approximate API cost $0.0000.

| | fhe() time (ms) | levels | (depth, mult. depth) | ct-ct mul | ct-pt mul | rotations | max error |
|---|---:|---:|---|---:|---:|---:|---:|
| baseline | 168.65 | 7 | (19, 7) | 5 | 5 | 0 | 1.2e-07 |
| best (attempt 2) | 56.43 | 4 | (10, 4) | 3 | 4 | 0 | 2e-07 |

Speedup: x2.97 (baseline and best re-scored as in score.py: 10 interleaved processes each; 1.00 when no candidate was confirmed faster).

| attempt | outcome | fhe() time (ms) | verdict | note |
|---:|---|---:|---|---|
| 1 | chehab_error | - | failed | CKKS scale mismatch: ciphertext operands of '+' (term 23) are at the same level 4 with different scales; align |
| 2 | ok | 56.43 | new_best, x2.97 vs attempt 0 |  |
| 3 | ok | 55.71 | not_confirmed, x1.02 vs attempt 2 |  |
| 4 | ok | - | same_as_best (attempt 2) |  |
| 5 | ok | - | same_as_best (attempt 2) |  |
| 6 | ok | 55.30 | not_confirmed, x1.02 vs attempt 2 |  |
| 7 | ok | 55.69 | not_confirmed, x1.02 vs attempt 2 |  |
| 8 | ok | - | same_as_best (attempt 2) |  |
