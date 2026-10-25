# quad: LLM optimization run 20261024-131501

Model `claude-sonnet-5-5` (effort medium), 8 attempts, wall time 152 s, approximate API cost $0.0000.

| | fhe() time (ms) | levels | (depth, mult. depth) | ct-ct mul | ct-pt mul | rotations | max error |
|---|---:|---:|---|---:|---:|---:|---:|
| baseline | 64.64 | 1 | (1, 1) | 1 | 0 | 0 | 6.6e-08 |
| best (attempt 0) | 64.64 | 1 | (1, 1) | 1 | 0 | 0 | 6.6e-08 |

Speedup: x1.00.

| attempt | outcome | fhe() time (ms) | note |
|---:|---|---:|---|
| 1 | ok | 67.50 | kernel dc35be324a |
| 2 | ok | 65.06 | kernel 8d886e302d |
| 3 | ok | 64.80 | kernel 8d886e302d |
| 4 | ok | 65.13 | kernel 8d886e302d |
| 5 | ok | 64.96 | kernel 8d886e302d |
| 6 | ok | 67.49 | kernel 8d886e302d |
| 7 | ok | 65.10 | kernel 8d886e302d |
| 8 | ok | 66.16 | kernel 8d886e302d |
