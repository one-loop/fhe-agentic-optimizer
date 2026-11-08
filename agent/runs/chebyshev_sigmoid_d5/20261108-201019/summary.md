# chebyshev_sigmoid_d5: LLM optimization run 20261108-201019

Model `claude-opus-5-5` (effort medium), 8 attempts, wall time 464 s, approximate API cost $0.0000.

| | fhe() time (ms) | levels | (depth, mult. depth) | ct-ct mul | ct-pt mul | rotations | max error |
|---|---:|---:|---|---:|---:|---:|---:|
| baseline | 113.16 | 6 | (16, 6) | 4 | 4 | 0 | 4.8e-08 |
| best (attempt 4) | 55.07 | 5 | (12, 5) | 3 | 2 | 0 | 5.9e-08 |

Speedup: x2.05 (baseline and best re-scored as in score.py: 10 interleaved processes each; 1.00 when no candidate was confirmed faster).

| attempt | outcome | fhe() time (ms) | verdict | note |
|---:|---|---:|---|---|
| 1 | ok | 59.13 | new_best, x2.00 vs attempt 0 |  |
| 2 | ok | 55.33 | not_confirmed, x1.02 vs attempt 1 |  |
| 3 | ok | 67.18 | slower |  |
| 4 | ok | 55.07 | new_best, x1.03 vs attempt 1 |  |
| 5 | ok | 56.79 | slower |  |
| 6 | ok | 56.66 | slower |  |
| 7 | ok | 56.12 | slower |  |
| 8 | ok | 66.56 | slower |  |
