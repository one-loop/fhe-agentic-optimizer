# chebyshev_relu_d7: LLM optimization run 20261108-195216

Model `claude-opus-5-5` (effort medium), 8 attempts, wall time 398 s, approximate API cost $0.0000.

| | fhe() time (ms) | levels | (depth, mult. depth) | ct-ct mul | ct-pt mul | rotations | max error |
|---|---:|---:|---|---:|---:|---:|---:|
| baseline | 166.90 | 7 | (19, 7) | 5 | 5 | 0 | 8.5e-08 |
| best (attempt 1) | 57.04 | 4 | (11, 4) | 3 | 4 | 0 | 8.6e-08 |

Speedup: x2.96 (baseline and best re-scored as in score.py: 10 interleaved processes each; 1.00 when no candidate was confirmed faster).

| attempt | outcome | fhe() time (ms) | verdict | note |
|---:|---|---:|---|---|
| 1 | ok | 57.04 | new_best, x2.95 vs attempt 0 |  |
| 2 | ok | 55.67 | not_confirmed, x1.01 vs attempt 1 |  |
| 3 | ok | 56.12 | not_confirmed, x1.01 vs attempt 1 |  |
| 4 | ok | 69.75 | slower |  |
| 5 | ok | - | duplicate (attempt 2) |  |
| 6 | ok | 55.45 | not_confirmed, x1.02 vs attempt 1 |  |
| 7 | ok | 55.46 | not_confirmed, x1.01 vs attempt 1 |  |
| 8 | ok | 56.01 | not_confirmed, x1.01 vs attempt 1 |  |
