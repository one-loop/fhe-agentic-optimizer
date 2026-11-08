# chebyshev_silu_d7: LLM optimization run 20261108-164753

Model `claude-sonnet-5-5` (effort medium), 8 attempts, wall time 290 s, approximate API cost $0.0000.

| | fhe() time (ms) | levels | (depth, mult. depth) | ct-ct mul | ct-pt mul | rotations | max error |
|---|---:|---:|---|---:|---:|---:|---:|
| baseline | 167.76 | 7 | (19, 7) | 5 | 5 | 0 | 1.2e-07 |
| best (attempt 6) | 55.60 | 4 | (12, 4) | 3 | 4 | 0 | 8.3e-08 |

Speedup: x3.05 (baseline and best re-scored as in score.py: 10 interleaved processes each; 1.00 when no candidate was confirmed faster).

| attempt | outcome | fhe() time (ms) | verdict | note |
|---:|---|---:|---|---|
| 1 | chehab_error | - | failed | CKKS scale mismatch: ciphertext operands of '+' (term 21) are at the same level 4 with different scales; align |
| 2 | ok | 57.14 | new_best, x2.93 vs attempt 0 |  |
| 3 | wrong_output | 57.29 | failed | wrong result on case fixed (9 slots): max abs error 0.0368 > tolerance 1e-06. The kernel does not compute the  |
| 4 | ok | 56.37 | not_confirmed, x1.02 vs attempt 2 |  |
| 5 | ok | 56.02 | not_confirmed, x1.02 vs attempt 2 |  |
| 6 | ok | 55.60 | new_best, x1.04 vs attempt 2 |  |
| 7 | ok | - | same_as_best (attempt 6) |  |
| 8 | ok | 55.81 | slower |  |
