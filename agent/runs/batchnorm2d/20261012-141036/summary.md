# batchnorm2d: LLM optimization run 20261012-141036

Model `claude-haiku-5-5` (effort medium), 8 attempts, wall time 75 s, approximate API cost $0.0023.

| | fhe() time (ms) | levels | (depth, mult. depth) | ct-ct mul | ct-pt mul | rotations | max error |
|---|---:|---:|---|---:|---:|---:|---:|
| baseline | 9.84 | 1 | (2, 1) | 0 | 1 | 0 | 2.8e-08 |
| best (attempt 6) | 9.69 | 1 | (2, 1) | 0 | 1 | 0 | 2.5e-08 |

Speedup: x1.02.

| attempt | outcome | fhe() time (ms) | note |
|---:|---|---:|---|
| 1 | ok | 9.79 | new best |
| 2 | ok | 9.87 | kernel 9c8c50e14d |
| 3 | ok | 9.84 | kernel 9c8c50e14d |
| 4 | ok | 9.81 | kernel 9c8c50e14d |
| 5 | ok | 9.83 | kernel 9c8c50e14d |
| 6 | ok | 9.69 | new best |
| 7 | ok | 9.83 | kernel 9c8c50e14d |
| 8 | ok | 9.81 | kernel 9c8c50e14d |
