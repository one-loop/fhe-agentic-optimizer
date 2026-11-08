# extract_linear: LLM optimization run 20261108-171948

Model `claude-sonnet-5-5` (effort medium), 8 attempts, wall time 114 s, approximate API cost $0.0000.

| | fhe() time (ms) | levels | (depth, mult. depth) | ct-ct mul | ct-pt mul | rotations | max error |
|---|---:|---:|---|---:|---:|---:|---:|
| baseline | 4.75 | 1 | (2, 1) | 0 | 1 | 1 | 4.2e-06 |
| best (attempt 0) | 4.75 | 1 | (2, 1) | 0 | 1 | 1 | 4.2e-06 |

Speedup: x1.00 (baseline and best re-scored as in score.py: 10 interleaved processes each; 1.00 when no candidate was confirmed faster).

| attempt | outcome | fhe() time (ms) | verdict | note |
|---:|---|---:|---|---|
| 1 | ok | - | same_as_baseline |  |
| 2 | chehab_error | - | failed | backend does not support plain plain evaluation |
| 3 | ok | - | same_as_baseline |  |
| 4 | wrong_output | 3.40 | failed | wrong result on case fixed (8 slots): max abs error 0.921 > tolerance 0.0001. The kernel does not compute the  |
| 5 | ok | - | same_as_baseline |  |
| 6 | ok | - | same_as_baseline |  |
| 7 | no_code | - |  |  |
| 8 | ok | - | same_as_baseline |  |
