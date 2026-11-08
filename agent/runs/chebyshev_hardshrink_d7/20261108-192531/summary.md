# chebyshev_hardshrink_d7: LLM optimization run 20261108-192531

Model `claude-opus-5-5` (effort medium), 8 attempts, wall time 696 s, approximate API cost $0.0000.

| | fhe() time (ms) | levels | (depth, mult. depth) | ct-ct mul | ct-pt mul | rotations | max error |
|---|---:|---:|---|---:|---:|---:|---:|
| baseline | 213.05 | 8 | (21, 8) | 6 | 5 | 0 | 1.5e-07 |
| best (attempt 4) | 84.09 | 4 | (10, 4) | 5 | 5 | 0 | 3e-07 |

Speedup: x2.40 (baseline and best re-scored as in score.py: 10 interleaved processes each; 1.00 when no candidate was confirmed faster).

| attempt | outcome | fhe() time (ms) | verdict | note |
|---:|---|---:|---|---|
| 1 | ok | 101.54 | new_best, x2.10 vs attempt 0 |  |
| 2 | ok | 92.98 | new_best, x1.09 vs attempt 1 |  |
| 3 | ok | 90.91 | new_best, x1.06 vs attempt 2 |  |
| 4 | ok | 84.09 | new_best, x1.04 vs attempt 3 |  |
| 5 | ok | 82.14 | not_confirmed, x1.01 vs attempt 4 |  |
| 6 | ok | 81.84 | not_confirmed, x1.01 vs attempt 4 |  |
| 7 | ok | 84.14 | slower |  |
| 8 | ok | 87.29 | slower |  |
