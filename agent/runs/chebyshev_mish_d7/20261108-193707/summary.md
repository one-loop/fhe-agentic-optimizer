# chebyshev_mish_d7: LLM optimization run 20261108-193707

Model `claude-opus-5-5` (effort medium), 8 attempts, wall time 672 s, approximate API cost $0.0000.

| | fhe() time (ms) | levels | (depth, mult. depth) | ct-ct mul | ct-pt mul | rotations | max error |
|---|---:|---:|---|---:|---:|---:|---:|
| baseline | 242.69 | 8 | (22, 8) | 6 | 8 | 0 | 1e-07 |
| best (attempt 6) | 108.95 | 5 | (13, 5) | 5 | 6 | 0 | 9.3e-08 |

Speedup: x2.22 (baseline and best re-scored as in score.py: 10 interleaved processes each; 1.00 when no candidate was confirmed faster).

| attempt | outcome | fhe() time (ms) | verdict | note |
|---:|---|---:|---|---|
| 1 | chehab_error | - | failed | CKKS scale mismatch: ciphertext operands of '+' (term 40) are at the same level 4 with different scales; align |
| 2 | ok | 126.25 | new_best, x1.92 vs attempt 0 |  |
| 3 | ok | 118.87 | new_best, x1.06 vs attempt 2 |  |
| 4 | ok | 115.48 | not_confirmed, x1.01 vs attempt 3 |  |
| 5 | ok | 118.24 | not_confirmed, x1.00 vs attempt 3 |  |
| 6 | ok | 108.95 | new_best, x1.09 vs attempt 3 |  |
| 7 | ok | 106.08 | not_confirmed, x1.01 vs attempt 6 |  |
| 8 | ok | 106.43 | not_confirmed, x1.02 vs attempt 6 |  |
