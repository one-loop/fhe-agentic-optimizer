# chebyshev_gelu_d7: LLM optimization run 20261108-191808

Model `claude-opus-5-5` (effort medium), 8 attempts, wall time 443 s, approximate API cost $0.0000.

| | fhe() time (ms) | levels | (depth, mult. depth) | ct-ct mul | ct-pt mul | rotations | max error |
|---|---:|---:|---|---:|---:|---:|---:|
| baseline | 167.42 | 7 | (19, 7) | 5 | 5 | 0 | 8.6e-08 |
| best (attempt 1) | 57.25 | 4 | (11, 4) | 3 | 4 | 0 | 1.1e-07 |

Speedup: x2.96 (baseline and best re-scored as in score.py: 10 interleaved processes each; 1.00 when no candidate was confirmed faster).

| attempt | outcome | fhe() time (ms) | verdict | note |
|---:|---|---:|---|---|
| 1 | ok | 57.25 | new_best, x2.96 vs attempt 0 |  |
| 2 | ok | 57.10 | not_confirmed, x0.99 vs attempt 1 |  |
| 3 | ok | 55.92 | not_confirmed, x1.01 vs attempt 1 |  |
| 4 | ok | 55.15 | not_confirmed, x1.02 vs attempt 1 |  |
| 5 | ok | 56.71 | not_confirmed, x1.02 vs attempt 1 |  |
| 6 | ok | 56.52 | not_confirmed, x1.00 vs attempt 1 |  |
| 7 | ok | 55.16 | not_confirmed, x1.02 vs attempt 1 |  |
| 8 | ok | - | same_as_best (attempt 1) |  |
