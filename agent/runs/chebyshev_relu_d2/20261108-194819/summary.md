# chebyshev_relu_d2: LLM optimization run 20261108-194819

Model `claude-opus-5-5` (effort medium), 8 attempts, wall time 237 s, approximate API cost $0.0000.

| | fhe() time (ms) | levels | (depth, mult. depth) | ct-ct mul | ct-pt mul | rotations | max error |
|---|---:|---:|---|---:|---:|---:|---:|
| baseline | 24.03 | 3 | (7, 3) | 1 | 3 | 0 | 6.6e-08 |
| best (attempt 1) | 17.04 | 3 | (7, 3) | 1 | 2 | 0 | 7.9e-08 |

Speedup: x1.41 (baseline and best re-scored as in score.py: 10 interleaved processes each; 1.00 when no candidate was confirmed faster).

| attempt | outcome | fhe() time (ms) | verdict | note |
|---:|---|---:|---|---|
| 1 | ok | 17.04 | new_best, x1.40 vs attempt 0 |  |
| 2 | ok | - | same_as_best (attempt 1) |  |
| 3 | ok | 26.11 | slower |  |
| 4 | ok | - | same_as_best (attempt 1) |  |
| 5 | ok | - | same_as_best (attempt 1) |  |
| 6 | ok | - | same_as_best (attempt 1) |  |
| 7 | ok | - | same_as_best (attempt 1) |  |
| 8 | ok | 24.49 | slower |  |
