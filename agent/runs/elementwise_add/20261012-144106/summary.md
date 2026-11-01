# elementwise_add: LLM optimization run 20261012-144106

Model `claude-haiku-5-5` (effort medium), 8 attempts, wall time 70 s, approximate API cost $0.0018.

| | fhe() time (ms) | levels | (depth, mult. depth) | ct-ct mul | ct-pt mul | rotations | max error |
|---|---:|---:|---|---:|---:|---:|---:|
| baseline | 0.46 | 0 | (1, 0) | 0 | 0 | 0 | 3.3e-08 |
| best (attempt 8) | 0.45 | 0 | (1, 0) | 0 | 0 | 0 | 2.8e-08 |

Speedup: x1.01.

| attempt | outcome | fhe() time (ms) | note |
|---:|---|---:|---|
| 1 | ok | 0.45 | new best |
| 2 | ok | 0.46 | kernel b5b82a8eaf |
| 3 | ok | 0.45 | new best |
| 4 | ok | 0.46 | kernel b5b82a8eaf |
| 5 | ok | 0.45 | kernel b5b82a8eaf |
| 6 | ok | 0.46 | kernel b5b82a8eaf |
| 7 | ok | 0.45 | kernel b5b82a8eaf |
| 8 | ok | 0.45 | new best |
