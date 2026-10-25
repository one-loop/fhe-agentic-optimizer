# extract_sparse: LLM optimization run 20261024-114925

Model `claude-sonnet-5-5` (effort medium), 8 attempts, wall time 218 s, approximate API cost $0.0000.

| | fhe() time (ms) | levels | (depth, mult. depth) | ct-ct mul | ct-pt mul | rotations | max error |
|---|---:|---:|---|---:|---:|---:|---:|
| baseline | 184.75 | 1 | (3, 1) | 0 | 3 | 3 | 1.2e-05 |
| best (attempt 7) | 128.49 | 1 | (3, 1) | 0 | 2 | 2 | 3.5e-06 |

Speedup: x1.44.

| attempt | outcome | fhe() time (ms) | note |
|---:|---|---:|---|
| 1 | ok | 133.15 | new best |
| 2 | ok | 141.81 | kernel 9e22a66690 |
| 3 | ok | 134.30 | kernel 47efba18d9 |
| 4 | ok | 143.61 | kernel 9e22a66690 |
| 5 | ok | 128.80 | new best |
| 6 | ok | 128.63 | new best |
| 7 | ok | 128.49 | new best |
| 8 | ok | 132.38 | kernel 9e22a66690 |
