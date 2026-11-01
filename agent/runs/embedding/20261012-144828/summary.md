# embedding: LLM optimization run 20261012-144828

Model `claude-haiku-5-5` (effort medium), 8 attempts, wall time 377 s, approximate API cost $0.0175.

| | fhe() time (ms) | levels | (depth, mult. depth) | ct-ct mul | ct-pt mul | rotations | max error |
|---|---:|---:|---|---:|---:|---:|---:|
| baseline | 616.80 | 2 | (21, 2) | 0 | 6 | 26 | 1.6e-05 |
| best (attempt 3) | 615.82 | 2 | (22, 2) | 0 | 6 | 26 | 2.7e-05 |

Speedup: x1.00.

| attempt | outcome | fhe() time (ms) | note |
|---:|---|---:|---|
| 1 | ok | 616.22 | new best |
| 2 | ok | 616.85 | kernel eba34e77a9 |
| 3 | ok | 615.82 | new best |
| 4 | ok | 616.19 | kernel eba34e77a9 |
| 5 | ok | 616.48 | kernel eba34e77a9 |
| 6 | ok | 618.51 | kernel eba34e77a9 |
| 7 | ok | 617.12 | kernel eba34e77a9 |
| 8 | ok | 616.77 | kernel eba34e77a9 |
