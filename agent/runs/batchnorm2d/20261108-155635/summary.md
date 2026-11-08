# batchnorm2d: LLM optimization run 20261108-155635

Model `claude-sonnet-5-5` (effort medium), 8 attempts, wall time 124 s, approximate API cost $0.0000.

| | fhe() time (ms) | levels | (depth, mult. depth) | ct-ct mul | ct-pt mul | rotations | max error |
|---|---:|---:|---|---:|---:|---:|---:|
| baseline | 3.77 | 1 | (2, 1) | 0 | 1 | 0 | 3.1e-08 |
| best (attempt 0) | 3.77 | 1 | (2, 1) | 0 | 1 | 0 | 3.1e-08 |

Speedup: x1.00 (baseline and best re-scored as in score.py: 10 interleaved processes each; 1.00 when no candidate was confirmed faster).

| attempt | outcome | fhe() time (ms) | verdict | note |
|---:|---|---:|---|---|
| 1 | ok | - | same_as_baseline |  |
| 2 | chehab_error | - | failed | backend does not support plain plain evaluation |
| 3 | ok | 3.95 | slower |  |
| 4 | ok | - | same_as_baseline |  |
| 5 | ok | - | duplicate (attempt 3) |  |
| 6 | ok | - | same_as_baseline |  |
| 7 | ok | - | same_as_baseline |  |
| 8 | ok | - | same_as_baseline |  |
