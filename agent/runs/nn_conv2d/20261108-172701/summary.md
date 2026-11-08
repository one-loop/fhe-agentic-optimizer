# nn_conv2d: LLM optimization run 20261108-172701

Model `claude-sonnet-5-5` (effort medium), 8 attempts, wall time 3710 s, approximate API cost $0.0000.

| | fhe() time (ms) | levels | (depth, mult. depth) | ct-ct mul | ct-pt mul | rotations | max error |
|---|---:|---:|---|---:|---:|---:|---:|
| baseline | 6674.26 | None | (10, 1) | 144 | 0 | 0 | 0 |
| best (attempt 3) | 3223.63 | None | (10, 1) | 64 | 0 | 0 | 0 |

Speedup: x2.21 (baseline and best re-scored as in score.py: 10 interleaved processes each; 1.00 when no candidate was confirmed faster).

| attempt | outcome | fhe() time (ms) | verdict | note |
|---:|---|---:|---|---|
| 1 | ok | 5578.26 | new_best, x1.20 vs attempt 0 |  |
| 2 | ok | 4675.81 | new_best, x1.21 vs attempt 1 |  |
| 3 | ok | 3223.63 | new_best, x1.55 vs attempt 2 |  |
| 4 | ok | - | same_as_best (attempt 3) |  |
| 5 | ok | - | same_as_best (attempt 3) |  |
| 6 | ok | - | same_as_best (attempt 3) |  |
| 7 | no_code | - |  |  |
| 8 | ok | - | same_as_best (attempt 3) |  |
