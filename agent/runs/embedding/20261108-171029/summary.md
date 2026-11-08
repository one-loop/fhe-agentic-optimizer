# embedding: LLM optimization run 20261108-171029

Model `claude-sonnet-5-5` (effort medium), 8 attempts, wall time 415 s, approximate API cost $0.0000.

| | fhe() time (ms) | levels | (depth, mult. depth) | ct-ct mul | ct-pt mul | rotations | max error |
|---|---:|---:|---|---:|---:|---:|---:|
| baseline | 135.31 | 2 | (21, 2) | 0 | 6 | 26 | 1.3e-05 |
| best (attempt 0) | 135.31 | 2 | (21, 2) | 0 | 6 | 26 | 1.3e-05 |

Speedup: x1.00 (baseline and best re-scored as in score.py: 10 interleaved processes each; 1.00 when no candidate was confirmed faster).

| attempt | outcome | fhe() time (ms) | verdict | note |
|---:|---|---:|---|---|
| 1 | ok | - | same_as_baseline |  |
| 2 | ok | 130.81 | not_confirmed, x1.00 vs attempt 0 |  |
| 3 | ok | - | duplicate (attempt 2) |  |
| 4 | ok | - | duplicate (attempt 2) |  |
| 5 | ok | 137.29 | slower |  |
| 6 | ok | - | duplicate (attempt 2) |  |
| 7 | ok | 131.89 | not_confirmed, x0.99 vs attempt 0 |  |
| 8 | ok | - | duplicate (attempt 2) |  |
