# extract_sparse: LLM optimization run 20261024-131120

Model `claude-sonnet-5-5` (effort medium), 8 attempts, wall time 222 s, approximate API cost $0.0000.

| | fhe() time (ms) | levels | (depth, mult. depth) | ct-ct mul | ct-pt mul | rotations | max error |
|---|---:|---:|---|---:|---:|---:|---:|
| baseline | 194.16 | 1 | (3, 1) | 0 | 3 | 3 | 5.2e-06 |
| best (attempt 7) | 128.61 | 1 | (4, 1) | 0 | 2 | 2 | 1.7e-06 |

Speedup: x1.51.

| attempt | outcome | fhe() time (ms) | note |
|---:|---|---:|---|
| 1 | ok | 130.16 | new best |
| 2 | ok | 138.84 | kernel 9eaadf5005 |
| 3 | ok | 133.16 | kernel d9532bf6d8 |
| 4 | ok | 144.64 | kernel 98b6614ce9 |
| 5 | ok | 130.80 | kernel 9eaadf5005 |
| 6 | ok | 130.35 | kernel 9eaadf5005 |
| 7 | ok | 128.61 | new best |
| 8 | ok | 128.65 | kernel 9eaadf5005 |
