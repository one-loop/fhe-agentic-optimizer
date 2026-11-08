# chebyshev_softplus_d7: LLM optimization run 20261108-203711

Model `claude-opus-5-5` (effort medium), 8 attempts, wall time 463 s, approximate API cost $0.0000.

| | fhe() time (ms) | levels | (depth, mult. depth) | ct-ct mul | ct-pt mul | rotations | max error |
|---|---:|---:|---|---:|---:|---:|---:|
| baseline | 166.92 | 7 | (19, 7) | 5 | 5 | 0 | 9.2e-08 |
| best (attempt 3) | 55.82 | 4 | (11, 4) | 3 | 4 | 0 | 1.1e-07 |

Speedup: x3.01 (baseline and best re-scored as in score.py: 10 interleaved processes each; 1.00 when no candidate was confirmed faster).

| attempt | outcome | fhe() time (ms) | verdict | note |
|---:|---|---:|---|---|
| 1 | ok | 63.33 | new_best, x2.88 vs attempt 0 |  |
| 2 | ok | 58.66 | not_confirmed, x1.00 vs attempt 1 |  |
| 3 | ok | 55.82 | new_best, x1.04 vs attempt 1 |  |
| 4 | ok | 56.27 | slower |  |
| 5 | ok | - | same_as_best (attempt 3) |  |
| 6 | ok | 55.20 | not_confirmed, x1.00 vs attempt 3 |  |
| 7 | ok | 55.30 | not_confirmed, x1.01 vs attempt 3 |  |
| 8 | ok | 55.38 | not_confirmed, x1.01 vs attempt 3 |  |
