# chebyshev_silu_d7: LLM optimization run 20261108-202910

Model `claude-opus-5-5` (effort medium), 8 attempts, wall time 481 s, approximate API cost $0.0000.

| | fhe() time (ms) | levels | (depth, mult. depth) | ct-ct mul | ct-pt mul | rotations | max error |
|---|---:|---:|---|---:|---:|---:|---:|
| baseline | 177.99 | 7 | (19, 7) | 5 | 5 | 0 | 1.1e-07 |
| best (attempt 1) | 62.13 | 4 | (11, 4) | 3 | 4 | 0 | 8.1e-08 |

Speedup: x2.99 (baseline and best re-scored as in score.py: 10 interleaved processes each; 1.00 when no candidate was confirmed faster).

| attempt | outcome | fhe() time (ms) | verdict | note |
|---:|---|---:|---|---|
| 1 | ok | 62.13 | new_best, x2.97 vs attempt 0 |  |
| 2 | ok | 57.61 | not_confirmed, x1.00 vs attempt 1 |  |
| 3 | ok | 56.20 | not_confirmed, x1.01 vs attempt 1 |  |
| 4 | ok | 55.91 | not_confirmed, x1.01 vs attempt 1 |  |
| 5 | ok | 60.83 | not_confirmed, x0.93 vs attempt 1 |  |
| 6 | ok | - | duplicate (attempt 4) |  |
| 7 | ok | 57.16 | not_confirmed, x0.99 vs attempt 1 |  |
| 8 | ok | 55.73 | not_confirmed, x1.02 vs attempt 1 |  |
