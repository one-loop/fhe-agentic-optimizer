# relu_composite: LLM optimization run 20261108-232638

Model `claude-opus-5-5` (effort medium), 8 attempts, wall time 1038 s, approximate API cost $0.0000.

| | fhe() time (ms) | levels | (depth, mult. depth) | ct-ct mul | ct-pt mul | rotations | max error |
|---|---:|---:|---|---:|---:|---:|---:|
| baseline | 641.15 | 10 | (17, 10) | 16 | 12 | 0 | 1.8e-07 |
| best (attempt 5) | 589.31 | 9 | (16, 9) | 19 | 12 | 0 | 1.5e-07 |

Speedup: x1.09 (baseline and best re-scored as in score.py: 10 interleaved processes each; 1.00 when no candidate was confirmed faster).

| attempt | outcome | fhe() time (ms) | verdict | note |
|---:|---|---:|---|---|
| 1 | chehab_error | - | failed | CHEHAB rejected the kernel (case fixed, 9 slots): |
| 2 | chehab_error | - | failed | CHEHAB rejected the kernel (case fixed, 9 slots): |
| 3 | chehab_error | - | failed | CKKS scale mismatch: ciphertext operands of '+' (term 54) are at the same level 9 with different scales; align |
| 4 | ok | 611.49 | new_best, x1.04 vs attempt 0 |  |
| 5 | ok | 589.31 | new_best, x1.05 vs attempt 4 |  |
| 6 | ok | 584.87 | not_confirmed, x1.00 vs attempt 5 |  |
| 7 | ok | 590.14 | slower |  |
| 8 | ok | - | duplicate (attempt 6) |  |
