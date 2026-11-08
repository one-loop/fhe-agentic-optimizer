# chebyshev_sigmoid_d7: LLM optimization run 20261108-201803

Model `claude-opus-5-5` (effort medium), 8 attempts, wall time 667 s, approximate API cost $0.0000.

| | fhe() time (ms) | levels | (depth, mult. depth) | ct-ct mul | ct-pt mul | rotations | max error |
|---|---:|---:|---|---:|---:|---:|---:|
| baseline | 214.04 | 8 | (22, 8) | 6 | 5 | 0 | 7.4e-08 |
| best (attempt 3) | 86.11 | 4 | (13, 4) | 5 | 5 | 0 | 2.7e-07 |

Speedup: x2.59 (baseline and best re-scored as in score.py: 10 interleaved processes each; 1.00 when no candidate was confirmed faster).

| attempt | outcome | fhe() time (ms) | verdict | note |
|---:|---|---:|---|---|
| 1 | ok | 102.27 | new_best, x2.09 vs attempt 0 |  |
| 2 | ok | 100.20 | not_confirmed, x1.02 vs attempt 1 |  |
| 3 | ok | 86.11 | new_best, x1.19 vs attempt 1 |  |
| 4 | ok | 85.76 | not_confirmed, x1.01 vs attempt 3 |  |
| 5 | ok | 84.73 | not_confirmed, x1.02 vs attempt 3 |  |
| 6 | ok | 83.70 | not_confirmed, x1.03 vs attempt 3 |  |
| 7 | ok | 84.06 | not_confirmed, x1.02 vs attempt 3 |  |
| 8 | ok | - | duplicate (attempt 7) |  |
