# extract_sparse: LLM optimization run 20261013-163327

Model `claude-haiku-5-5` (effort medium), 8 attempts, wall time 166 s, approximate API cost $0.0053.

| | fhe() time (ms) | levels | (depth, mult. depth) | ct-ct mul | ct-pt mul | rotations | max error |
|---|---:|---:|---|---:|---:|---:|---:|
| baseline | 177.95 | 1 | (3, 1) | 0 | 3 | 3 | 9.6e-06 |
| best (attempt 1) | 128.10 | 1 | (4, 1) | 0 | 2 | 2 | 3.8e-06 |

Speedup: x1.39.

| attempt | outcome | fhe() time (ms) | note |
|---:|---|---:|---|
| 1 | ok | 128.10 | new best |
| 2 | ok | 131.51 | kernel 94ad4d9645 |
| 3 | ok | 132.48 | kernel 08e7b2324b |
| 4 | ok | 129.02 | kernel 94ad4d9645 |
| 5 | ok | 130.66 | kernel 94ad4d9645 |
| 6 | ok | 141.14 | kernel 94ad4d9645 |
| 7 | ok | 133.52 | kernel 94ad4d9645 |
| 8 | ok | 131.75 | kernel 94ad4d9645 |
