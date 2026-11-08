# chebyshev_elu_d7: LLM optimization run 20261108-190717

Model `claude-opus-5-5` (effort medium), 8 attempts, wall time 651 s, approximate API cost $0.0000.

| | fhe() time (ms) | levels | (depth, mult. depth) | ct-ct mul | ct-pt mul | rotations | max error |
|---|---:|---:|---|---:|---:|---:|---:|
| baseline | 241.48 | 8 | (22, 8) | 6 | 8 | 0 | 1.1e-07 |
| best (attempt 2) | 91.03 | 4 | (9, 4) | 5 | 5 | 0 | 2.1e-07 |

Speedup: x2.65 (baseline and best re-scored as in score.py: 10 interleaved processes each; 1.00 when no candidate was confirmed faster).

| attempt | outcome | fhe() time (ms) | verdict | note |
|---:|---|---:|---|---|
| 1 | ok | 121.13 | new_best, x1.99 vs attempt 0 |  |
| 2 | ok | 91.03 | new_best, x1.33 vs attempt 1 |  |
| 3 | ok | 104.41 | slower |  |
| 4 | ok | 91.23 | slower |  |
| 5 | ok | 90.69 | not_confirmed, x1.01 vs attempt 2 |  |
| 6 | ok | 90.04 | not_confirmed, x1.02 vs attempt 2 |  |
| 7 | ok | 88.63 | not_confirmed, x1.03 vs attempt 2 |  |
| 8 | ok | - | duplicate (attempt 7) |  |
