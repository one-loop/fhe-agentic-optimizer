# extract_sparse: LLM optimization run 20261108-172142

Model `claude-sonnet-5-5` (effort medium), 8 attempts, wall time 162 s, approximate API cost $0.0000.

| | fhe() time (ms) | levels | (depth, mult. depth) | ct-ct mul | ct-pt mul | rotations | max error |
|---|---:|---:|---|---:|---:|---:|---:|
| baseline | 14.47 | 1 | (3, 1) | 0 | 3 | 3 | 4.6e-06 |
| best (attempt 2) | 10.99 | 1 | (4, 1) | 0 | 2 | 2 | 9.6e-06 |

Speedup: x1.29 (baseline and best re-scored as in score.py: 10 interleaved processes each; 1.00 when no candidate was confirmed faster).

| attempt | outcome | fhe() time (ms) | verdict | note |
|---:|---|---:|---|---|
| 1 | ok | 11.94 | new_best, x1.18 vs attempt 0 |  |
| 2 | ok | 10.99 | new_best, x1.09 vs attempt 1 |  |
| 3 | ok | - | same_as_best (attempt 2) |  |
| 4 | ok | - | same_as_best (attempt 2) |  |
| 5 | ok | - | same_as_best (attempt 2) |  |
| 6 | ok | - | same_as_best (attempt 2) |  |
| 7 | ok | - | same_as_best (attempt 2) |  |
| 8 | ok | - | same_as_best (attempt 2) |  |
