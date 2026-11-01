# chebyshev_sigmoid_d7: LLM optimization run 20261012-133825

Model `claude-haiku-5-5` (effort medium), 8 attempts, wall time 165 s, approximate API cost $0.0117.

| | fhe() time (ms) | levels | (depth, mult. depth) | ct-ct mul | ct-pt mul | rotations | max error |
|---|---:|---:|---|---:|---:|---:|---:|
| baseline | 163.59 | 8 | (22, 8) | 6 | 5 | 0 | 7.2e-08 |
| best (attempt 3) | 136.14 | 6 | (16, 6) | 4 | 5 | 0 | 5.2e-08 |

Speedup: x1.20.

| attempt | outcome | fhe() time (ms) | note |
|---:|---|---:|---|
| 1 | ok | 136.66 | new best |
| 2 | ok | 137.56 | kernel a10790dfad |
| 3 | ok | 136.14 | new best |
| 4 | ok | 137.13 | kernel 7bb60ad46b |
| 5 | ok | 136.64 | kernel 7bb60ad46b |
| 6 | ok | 136.90 | kernel 7bb60ad46b |
| 7 | ok | 137.29 | kernel 7bb60ad46b |
| 8 | ok | 136.88 | kernel 7bb60ad46b |
