# extract_sparse: LLM optimization run 20261012-145743

Model `claude-haiku-5-5` (effort medium), 8 attempts, wall time 115 s, approximate API cost $0.0059.

| | fhe() time (ms) | levels | (depth, mult. depth) | ct-ct mul | ct-pt mul | rotations | max error |
|---|---:|---:|---|---:|---:|---:|---:|
| baseline | 87.72 | 1 | (3, 1) | 0 | 3 | 3 | 1.1e-05 |
| best (attempt 1) | 63.53 | 1 | (4, 1) | 0 | 2 | 2 | 5.1e-06 |

Speedup: x1.38.

| attempt | outcome | fhe() time (ms) | note |
|---:|---|---:|---|
| 1 | ok | 63.53 | new best |
| 2 | ok | 63.59 | kernel 79385db5da |
| 3 | ok | 65.58 | kernel c1c201027d |
| 4 | ok | 63.55 | kernel 79385db5da |
| 5 | ok | 63.56 | kernel 79385db5da |
| 6 | ok | 63.59 | kernel 79385db5da |
| 7 | ok | 63.60 | kernel 79385db5da |
| 8 | ok | 63.68 | kernel 79385db5da |
