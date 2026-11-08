# chebyshev_sigmoid_d5: LLM optimization run 20261108-163726

Model `claude-sonnet-5-5` (effort medium), 8 attempts, wall time 286 s, approximate API cost $0.0000.

| | fhe() time (ms) | levels | (depth, mult. depth) | ct-ct mul | ct-pt mul | rotations | max error |
|---|---:|---:|---|---:|---:|---:|---:|
| baseline | 113.40 | 6 | (16, 6) | 4 | 4 | 0 | 4.4e-08 |
| best (attempt 1) | 57.12 | 4 | (12, 4) | 3 | 4 | 0 | 1.9e-07 |

Speedup: x1.99 (baseline and best re-scored as in score.py: 10 interleaved processes each; 1.00 when no candidate was confirmed faster).

| attempt | outcome | fhe() time (ms) | verdict | note |
|---:|---|---:|---|---|
| 1 | ok | 57.12 | new_best, x1.98 vs attempt 0 |  |
| 2 | ok | 59.44 | slower |  |
| 3 | ok | 57.32 | slower |  |
| 4 | ok | - | duplicate (attempt 3) |  |
| 5 | ok | 58.00 | slower |  |
| 6 | ok | 57.37 | slower |  |
| 7 | ok | 57.81 | slower |  |
| 8 | ok | - | same_as_best (attempt 1) |  |
