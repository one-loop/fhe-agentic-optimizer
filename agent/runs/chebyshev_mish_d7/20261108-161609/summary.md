# chebyshev_mish_d7: LLM optimization run 20261108-161609

Model `claude-sonnet-5-5` (effort medium), 8 attempts, wall time 377 s, approximate API cost $0.0000.

| | fhe() time (ms) | levels | (depth, mult. depth) | ct-ct mul | ct-pt mul | rotations | max error |
|---|---:|---:|---|---:|---:|---:|---:|
| baseline | 242.94 | 8 | (22, 8) | 6 | 8 | 0 | 9e-08 |
| best (attempt 1) | 122.56 | 5 | (13, 5) | 4 | 9 | 0 | 1.5e-07 |

Speedup: x2.00 (baseline and best re-scored as in score.py: 10 interleaved processes each; 1.00 when no candidate was confirmed faster).

| attempt | outcome | fhe() time (ms) | verdict | note |
|---:|---|---:|---|---|
| 1 | ok | 122.56 | new_best, x2.01 vs attempt 0 |  |
| 2 | wrong_output | 123.88 | failed | wrong result on case fixed (9 slots): max abs error 0.164 > tolerance 1e-06. The kernel does not compute the s |
| 3 | ok | 125.95 | slower |  |
| 4 | ok | 124.23 | slower |  |
| 5 | ok | 146.41 | slower |  |
| 6 | ok | 132.68 | slower |  |
| 7 | ok | 133.80 | slower |  |
| 8 | ok | 122.55 | not_confirmed, x1.02 vs attempt 1 |  |
