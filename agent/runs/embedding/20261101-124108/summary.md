# embedding: LLM optimization run 20261101-124108

Model `claude-opus-5-5` (effort medium), 8 attempts, wall time 2013 s, approximate API cost $0.0000.

| | fhe() time (ms) | levels | (depth, mult. depth) | ct-ct mul | ct-pt mul | rotations | max error |
|---|---:|---:|---|---:|---:|---:|---:|
| baseline | 620.50 | 2 | (21, 2) | 0 | 6 | 26 | 1.6e-05 |
| best (attempt 2) | 107.92 | 10 | (31, 10) | 0 | 14 | 26 | 1.8e-05 |

Speedup: x5.69 (baseline and best timed in 7 alternating batches; 1.00 when no candidate was confirmed faster).

| attempt | outcome | fhe() time (ms) | verdict | note |
|---:|---|---:|---|---|
| 1 | ok | 125.70 | new_best, x4.80 vs attempt 0 |  |
| 2 | ok | 107.92 | new_best, x1.18 vs attempt 1 |  |
| 3 | ok | 108.03 | slower |  |
| 4 | ok | - | same_as_best (attempt 2) |  |
| 5 | ok | 146.70 | slower |  |
| 6 | ok | 111.08 | slower |  |
| 7 | ok | - | same_as_best (attempt 2) |  |
| 8 | ok | - | same_as_best (attempt 2) |  |
