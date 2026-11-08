# quad: LLM optimization run 20261108-232420

Model `claude-opus-5-5` (effort medium), 8 attempts, wall time 138 s, approximate API cost $0.0000.

| | fhe() time (ms) | levels | (depth, mult. depth) | ct-ct mul | ct-pt mul | rotations | max error |
|---|---:|---:|---|---:|---:|---:|---:|
| baseline | 6.06 | 1 | (1, 1) | 1 | 0 | 0 | 8.2e-08 |
| best (attempt 0) | 6.06 | 1 | (1, 1) | 1 | 0 | 0 | 8.2e-08 |

Speedup: x1.00 (baseline and best re-scored as in score.py: 10 interleaved processes each; 1.00 when no candidate was confirmed faster).

| attempt | outcome | fhe() time (ms) | verdict | note |
|---:|---|---:|---|---|
| 1 | ok | 5.95 | not_confirmed, x1.02 vs attempt 0 |  |
| 2 | ok | - | duplicate (attempt 1) |  |
| 3 | ok | - | duplicate (attempt 1) |  |
| 4 | ok | - | duplicate (attempt 1) |  |
| 5 | ok | - | duplicate (attempt 1) |  |
| 6 | ok | - | duplicate (attempt 1) |  |
| 7 | ok | - | duplicate (attempt 1) |  |
| 8 | ok | - | duplicate (attempt 1) |  |
