# chebyshev_softplus_d7: LLM optimization run 20261108-165242

Model `claude-sonnet-5-5` (effort medium), 8 attempts, wall time 311 s, approximate API cost $0.0000.

| | fhe() time (ms) | levels | (depth, mult. depth) | ct-ct mul | ct-pt mul | rotations | max error |
|---|---:|---:|---|---:|---:|---:|---:|
| baseline | 182.47 | 7 | (19, 7) | 5 | 5 | 0 | 1.3e-07 |
| best (attempt 1) | 57.72 | 4 | (11, 4) | 3 | 4 | 0 | 9.2e-08 |

Speedup: x2.97 (baseline and best re-scored as in score.py: 10 interleaved processes each; 1.00 when no candidate was confirmed faster).

| attempt | outcome | fhe() time (ms) | verdict | note |
|---:|---|---:|---|---|
| 1 | ok | 57.72 | new_best, x2.95 vs attempt 0 |  |
| 2 | ok | 56.52 | not_confirmed, x1.01 vs attempt 1 |  |
| 3 | ok | 59.31 | slower |  |
| 4 | ok | 59.35 | slower |  |
| 5 | ok | - | same_as_best (attempt 1) |  |
| 6 | ok | - | duplicate (attempt 2) |  |
| 7 | ok | 56.93 | not_confirmed, x1.01 vs attempt 1 |  |
| 8 | ok | 57.09 | not_confirmed, x1.00 vs attempt 1 |  |
