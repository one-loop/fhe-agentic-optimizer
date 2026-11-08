# nn_conv2d: LLM optimization run 20261108-215918

Model `claude-opus-5-5` (effort medium), 8 attempts, wall time 4094 s, approximate API cost $0.0000.

| | fhe() time (ms) | levels | (depth, mult. depth) | ct-ct mul | ct-pt mul | rotations | max error |
|---|---:|---:|---|---:|---:|---:|---:|
| baseline | 6761.76 | None | (10, 1) | 144 | 0 | 0 | 0 |
| best (attempt 2) | 1797.29 | None | (21, 2) | 36 | 16 | 0 | 0 |

Speedup: x3.74 (baseline and best re-scored as in score.py: 10 interleaved processes each; 1.00 when no candidate was confirmed faster).

| attempt | outcome | fhe() time (ms) | verdict | note |
|---:|---|---:|---|---|
| 1 | ok | 3036.36 | new_best, x2.22 vs attempt 0 |  |
| 2 | ok | 1797.29 | new_best, x1.67 vs attempt 1 |  |
| 3 | ok | 1780.81 | not_confirmed, x1.01 vs attempt 2 |  |
| 4 | ok | 1805.25 | slower |  |
| 5 | ok | 1789.29 | not_confirmed, x1.00 vs attempt 2 |  |
| 6 | ok | 1774.77 | not_confirmed, x1.02 vs attempt 2 |  |
| 7 | ok | 1764.78 | not_confirmed, x1.02 vs attempt 2 |  |
| 8 | ok | 1765.56 | not_confirmed, x1.02 vs attempt 2 |  |
