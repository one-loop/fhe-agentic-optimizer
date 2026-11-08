# quad: LLM optimization run 20261108-184451

Model `claude-sonnet-5-5` (effort medium), 8 attempts, wall time 138 s, approximate API cost $0.0000.

| | fhe() time (ms) | levels | (depth, mult. depth) | ct-ct mul | ct-pt mul | rotations | max error |
|---|---:|---:|---|---:|---:|---:|---:|
| baseline | 6.10 | 1 | (1, 1) | 1 | 0 | 0 | 1e-07 |
| best (attempt 0) | 6.10 | 1 | (1, 1) | 1 | 0 | 0 | 1e-07 |

Speedup: x1.00 (baseline and best re-scored as in score.py: 10 interleaved processes each; 1.00 when no candidate was confirmed faster).

| attempt | outcome | fhe() time (ms) | verdict | note |
|---:|---|---:|---|---|
| 1 | ok | 5.99 | not_confirmed, x1.02 vs attempt 0 |  |
| 2 | ok | - | duplicate (attempt 1) |  |
| 3 | ok | - | duplicate (attempt 1) |  |
| 4 | ok | - | duplicate (attempt 1) |  |
| 5 | ok | - | duplicate (attempt 1) |  |
| 6 | ok | - | duplicate (attempt 1) |  |
| 7 | ok | - | duplicate (attempt 1) |  |
| 8 | ok | - | duplicate (attempt 1) |  |
