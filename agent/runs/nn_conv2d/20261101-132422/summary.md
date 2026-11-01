# nn_conv2d: LLM optimization run 20261101-132422

Model `claude-opus-5-5` (effort medium), 8 attempts, wall time 2590 s, approximate API cost $0.0000.

| | fhe() time (ms) | levels | (depth, mult. depth) | ct-ct mul | ct-pt mul | rotations | max error |
|---|---:|---:|---|---:|---:|---:|---:|
| baseline | 3425.20 | None | (10, 1) | 144 | 0 | 0 | 0 |
| best (attempt 4) | 938.34 | None | (23, 2) | 36 | 16 | 0 | 0 |

Speedup: x3.69 (baseline and best timed in 7 alternating batches; 1.00 when no candidate was confirmed faster).

| attempt | outcome | fhe() time (ms) | verdict | note |
|---:|---|---:|---|---|
| 1 | ok | 2633.89 | new_best, x1.30 vs attempt 0 |  |
| 2 | ok | 1550.98 | new_best, x1.70 vs attempt 1 |  |
| 3 | ok | 1543.25 | not_confirmed, x1.00 vs attempt 2 |  |
| 4 | ok | 938.34 | new_best, x1.67 vs attempt 2 |  |
| 5 | ok | 931.52 | not_confirmed, x1.00 vs attempt 4 |  |
| 6 | ok | 920.20 | not_confirmed, x1.02 vs attempt 4 |  |
| 7 | ok | 914.14 | not_confirmed, x1.01 vs attempt 4 |  |
| 8 | ok | 920.55 | not_confirmed, x1.02 vs attempt 4 |  |
