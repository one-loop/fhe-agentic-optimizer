# nn_linear: LLM optimization run 20261108-230732

Model `claude-opus-5-5` (effort medium), 8 attempts, wall time 1008 s, approximate API cost $0.0000.

| | fhe() time (ms) | levels | (depth, mult. depth) | ct-ct mul | ct-pt mul | rotations | max error |
|---|---:|---:|---|---:|---:|---:|---:|
| baseline | 1700.50 | None | (7, 1) | 36 | 0 | 0 | 0 |
| best (attempt 0) | 1700.50 | None | (7, 1) | 36 | 0 | 0 | 0 |

Speedup: x1.00 (baseline and best re-scored as in score.py: 10 interleaved processes each; 1.00 when no candidate was confirmed faster).

| attempt | outcome | fhe() time (ms) | verdict | note |
|---:|---|---:|---|---|
| 1 | ok | 1686.80 | not_confirmed, x1.00 vs attempt 0 |  |
| 2 | ok | 1674.19 | not_confirmed, x1.00 vs attempt 0 |  |
| 3 | ok | - | duplicate (attempt 2) |  |
| 4 | ok | - | duplicate (attempt 2) |  |
| 5 | ok | - | duplicate (attempt 2) |  |
| 6 | ok | - | same_as_baseline |  |
| 7 | ok | - | duplicate (attempt 2) |  |
| 8 | ok | 1682.49 | not_confirmed, x1.00 vs attempt 0 |  |
