# extract_sparse: LLM optimization run 20261108-215211

Model `claude-opus-5-5` (effort medium), 8 attempts, wall time 234 s, approximate API cost $0.0000.

| | fhe() time (ms) | levels | (depth, mult. depth) | ct-ct mul | ct-pt mul | rotations | max error |
|---|---:|---:|---|---:|---:|---:|---:|
| baseline | 14.16 | 1 | (3, 1) | 0 | 3 | 3 | 6.2e-06 |
| best (attempt 2) | 12.14 | 1 | (3, 1) | 0 | 3 | 2 | 4.3e-06 |

Speedup: x1.18 (baseline and best re-scored as in score.py: 10 interleaved processes each; 1.00 when no candidate was confirmed faster).

| attempt | outcome | fhe() time (ms) | verdict | note |
|---:|---|---:|---|---|
| 1 | ok | 11.06 | not_confirmed, x0.92 vs attempt 0 |  |
| 2 | ok | 12.14 | new_best, x1.20 vs attempt 0 |  |
| 3 | ok | - | duplicate (attempt 1) |  |
| 4 | ok | - | same_as_best (attempt 2) |  |
| 5 | ok | - | same_as_best (attempt 2) |  |
| 6 | ok | - | same_as_best (attempt 2) |  |
| 7 | ok | - | same_as_best (attempt 2) |  |
| 8 | ok | - | same_as_best (attempt 2) |  |
