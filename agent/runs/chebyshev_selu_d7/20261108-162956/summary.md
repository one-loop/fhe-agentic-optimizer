# chebyshev_selu_d7: LLM optimization run 20261108-162956

Model `claude-sonnet-5-5` (effort medium), 8 attempts, wall time 450 s, approximate API cost $0.0000.

| | fhe() time (ms) | levels | (depth, mult. depth) | ct-ct mul | ct-pt mul | rotations | max error |
|---|---:|---:|---|---:|---:|---:|---:|
| baseline | 243.78 | 8 | (22, 8) | 6 | 8 | 0 | 1e-07 |
| best (attempt 4) | 140.26 | 5 | (13, 5) | 5 | 10 | 0 | 1.2e-07 |

Speedup: x1.73 (baseline and best re-scored as in score.py: 10 interleaved processes each; 1.00 when no candidate was confirmed faster).

| attempt | outcome | fhe() time (ms) | verdict | note |
|---:|---|---:|---|---|
| 1 | chehab_error | - | failed | CKKS scale mismatch: ciphertext operands of '+' (term 37) are at the same level 4 with different scales; align |
| 2 | chehab_error | - | failed | CKKS scale mismatch: ciphertext operands of '+' (term 38) are at the same level 5 with different scales; align |
| 3 | chehab_error | - | failed | CKKS scale mismatch: ciphertext operands of '+' (term 30) are at the same level 4 with different scales; align |
| 4 | ok | 140.26 | new_best, x1.73 vs attempt 0 |  |
| 5 | ok | 144.68 | slower |  |
| 6 | chehab_error | - | failed | CKKS scale mismatch: ciphertext operands of '+' (term 30) are at the same level 4 with different scales; align |
| 7 | chehab_error | - | failed | CKKS scale mismatch: ciphertext operands of '+' (term 47) are at the same level 5 with different scales; align |
| 8 | ok | 136.69 | not_confirmed, x1.03 vs attempt 4 |  |
