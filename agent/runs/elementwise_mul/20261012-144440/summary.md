# elementwise_mul: LLM optimization run 20261012-144440

Model `claude-haiku-5-5` (effort medium), 8 attempts, wall time 80 s, approximate API cost $0.0020.

| | fhe() time (ms) | levels | (depth, mult. depth) | ct-ct mul | ct-pt mul | rotations | max error |
|---|---:|---:|---|---:|---:|---:|---:|
| baseline | 31.49 | 1 | (1, 1) | 1 | 0 | 0 | 3.8e-08 |
| best (attempt 0) | 31.49 | 1 | (1, 1) | 1 | 0 | 0 | 3.8e-08 |

Speedup: x1.00.

| attempt | outcome | fhe() time (ms) | note |
|---:|---|---:|---|
| 1 | ok | 31.56 | kernel 24af55ad3d |
| 2 | ok | 31.51 | kernel 24af55ad3d |
| 3 | ok | 31.54 | kernel 24af55ad3d |
| 4 | ok | 31.55 | kernel 24af55ad3d |
| 5 | ok | 31.54 | kernel 24af55ad3d |
| 6 | ok | 31.50 | kernel 24af55ad3d |
| 7 | ok | 31.52 | kernel 24af55ad3d |
| 8 | ok | 31.51 | kernel 24af55ad3d |
