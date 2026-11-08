# nn_linear: LLM optimization run 20261108-182851

Model `claude-sonnet-5-5` (effort medium), 8 attempts, wall time 959 s, approximate API cost $0.0000.

| | fhe() time (ms) | levels | (depth, mult. depth) | ct-ct mul | ct-pt mul | rotations | max error |
|---|---:|---:|---|---:|---:|---:|---:|
| baseline | 1782.60 | None | (7, 1) | 36 | 0 | 0 | 0 |
| best (attempt 0) | 1782.60 | None | (7, 1) | 36 | 0 | 0 | 0 |

Speedup: x1.00 (baseline and best re-scored as in score.py: 10 interleaved processes each; 1.00 when no candidate was confirmed faster).

| attempt | outcome | fhe() time (ms) | verdict | note |
|---:|---|---:|---|---|
| 1 | ok | 1678.56 | not_confirmed, x0.98 vs attempt 0 |  |
| 2 | ok | 1760.05 | not_confirmed, x1.02 vs attempt 0 |  |
| 3 | ok | - | duplicate (attempt 1) |  |
| 4 | ok | - | duplicate (attempt 1) |  |
| 5 | ok | 1826.49 | slower |  |
| 6 | ok | 1671.97 | not_confirmed, x1.00 vs attempt 0 |  |
| 7 | ok | - | duplicate (attempt 6) |  |
| 8 | ok | - | duplicate (attempt 6) |  |
