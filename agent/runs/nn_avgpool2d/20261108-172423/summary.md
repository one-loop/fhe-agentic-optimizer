# nn_avgpool2d: LLM optimization run 20261108-172423

Model `claude-sonnet-5-5` (effort medium), 8 attempts, wall time 158 s, approximate API cost $0.0000.

| | fhe() time (ms) | levels | (depth, mult. depth) | ct-ct mul | ct-pt mul | rotations | max error |
|---|---:|---:|---|---:|---:|---:|---:|
| baseline | 7.56 | None | (3, 0) | 0 | 0 | 0 | 0 |
| best (attempt 0) | 7.56 | None | (3, 0) | 0 | 0 | 0 | 0 |

Speedup: x1.00 (baseline and best re-scored as in score.py: 10 interleaved processes each; 1.00 when no candidate was confirmed faster).

| attempt | outcome | fhe() time (ms) | verdict | note |
|---:|---|---:|---|---|
| 1 | ok | 7.33 | not_confirmed, x1.02 vs attempt 0 |  |
| 2 | ok | - | duplicate (attempt 1) |  |
| 3 | ok | - | duplicate (attempt 1) |  |
| 4 | no_code | - |  |  |
| 5 | ok | - | duplicate (attempt 1) |  |
| 6 | ok | - | duplicate (attempt 1) |  |
| 7 | no_code | - |  |  |
| 8 | ok | - | duplicate (attempt 1) |  |
