# chebyshev_relu_d2: LLM optimization run 20261101-110540

Model `claude-opus-5-5` (effort medium), 8 attempts, wall time 226 s, approximate API cost $0.0000.

| | fhe() time (ms) | levels | (depth, mult. depth) | ct-ct mul | ct-pt mul | rotations | max error |
|---|---:|---:|---|---:|---:|---:|---:|
| baseline | 53.55 | 3 | (7, 3) | 1 | 3 | 0 | 1.3e-07 |
| best (attempt 1) | 41.52 | 3 | (7, 3) | 1 | 2 | 0 | 1.4e-07 |

Speedup: x1.29 (baseline and best timed in 7 alternating batches; 1.00 when no candidate was confirmed faster).

| attempt | outcome | fhe() time (ms) | verdict | note |
|---:|---|---:|---|---|
| 1 | ok | 41.52 | new_best, x1.29 vs attempt 0 |  |
| 2 | ok | - | same_as_best (attempt 1) |  |
| 3 | ok | - | same_as_best (attempt 1) |  |
| 4 | ok | - | same_as_best (attempt 1) |  |
| 5 | ok | - | same_as_best (attempt 1) |  |
| 6 | ok | 46.36 | slower |  |
| 7 | ok | - | same_as_best (attempt 1) |  |
| 8 | ok | - | same_as_best (attempt 1) |  |
