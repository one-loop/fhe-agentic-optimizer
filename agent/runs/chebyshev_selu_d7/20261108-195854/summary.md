# chebyshev_selu_d7: LLM optimization run 20261108-195854

Model `claude-opus-5-5` (effort medium), 8 attempts, wall time 684 s, approximate API cost $0.0000.

| | fhe() time (ms) | levels | (depth, mult. depth) | ct-ct mul | ct-pt mul | rotations | max error |
|---|---:|---:|---|---:|---:|---:|---:|
| baseline | 241.22 | 8 | (22, 8) | 6 | 8 | 0 | 9e-08 |
| best (attempt 6) | 88.10 | 4 | (12, 4) | 5 | 5 | 0 | 1.5e-07 |

Speedup: x2.71 (baseline and best re-scored as in score.py: 10 interleaved processes each; 1.00 when no candidate was confirmed faster).

| attempt | outcome | fhe() time (ms) | verdict | note |
|---:|---|---:|---|---|
| 1 | ok | 90.21 | new_best, x2.67 vs attempt 0 |  |
| 2 | ok | 88.73 | not_confirmed, x1.00 vs attempt 1 |  |
| 3 | ok | - | duplicate (attempt 2) |  |
| 4 | ok | 87.30 | not_confirmed, x1.02 vs attempt 1 |  |
| 5 | ok | 90.73 | slower |  |
| 6 | ok | 88.10 | new_best, x1.03 vs attempt 1 |  |
| 7 | ok | - | same_as_best (attempt 6) |  |
| 8 | ok | - | same_as_best (attempt 6) |  |
