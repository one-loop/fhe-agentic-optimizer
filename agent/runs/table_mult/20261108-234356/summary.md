# table_mult: LLM optimization run 20261108-234356

Model `claude-opus-5-5` (effort medium), 8 attempts, wall time 246 s, approximate API cost $0.0000.

| | fhe() time (ms) | levels | (depth, mult. depth) | ct-ct mul | ct-pt mul | rotations | max error |
|---|---:|---:|---|---:|---:|---:|---:|
| baseline | 60.93 | 1 | (17, 1) | 0 | 3 | 24 | 3.3e-05 |
| best (attempt 0) | 60.93 | 1 | (17, 1) | 0 | 3 | 24 | 3.3e-05 |

Speedup: x1.00 (baseline and best re-scored as in score.py: 10 interleaved processes each; 1.00 when no candidate was confirmed faster).

| attempt | outcome | fhe() time (ms) | verdict | note |
|---:|---|---:|---|---|
| 1 | ok | - | same_as_baseline |  |
| 2 | ok | 61.27 | slower |  |
| 3 | ok | - | same_as_baseline |  |
| 4 | ok | - | same_as_baseline |  |
| 5 | ok | - | same_as_baseline |  |
| 6 | chehab_error | - | failed | backend does not support plain plain evaluation |
| 7 | ok | - | same_as_baseline |  |
| 8 | ok | - | same_as_baseline |  |
