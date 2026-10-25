# batchnorm1d: LLM optimization run 20261013-152256

Model `claude-haiku-5-5` (effort medium), 8 attempts, wall time 116 s, approximate API cost $0.0025.

| | fhe() time (ms) | levels | (depth, mult. depth) | ct-ct mul | ct-pt mul | rotations | max error |
|---|---:|---:|---|---:|---:|---:|---:|
| baseline | 17.74 | 1 | (2, 1) | 0 | 1 | 0 | 2.5e-08 |
| best (attempt 0) | 17.74 | 1 | (2, 1) | 0 | 1 | 0 | 2.5e-08 |

Speedup: x1.00.

| attempt | outcome | fhe() time (ms) | note |
|---:|---|---:|---|
| 1 | ok | 17.74 | kernel 6526d5a683 |
| 2 | ok | 19.02 | kernel 59c3249adc |
| 3 | ok | 17.89 | kernel fd2962e0c5 |
| 4 | ok | 18.05 | kernel fd2962e0c5 |
| 5 | ok | 18.05 | kernel fd2962e0c5 |
| 6 | ok | 21.02 | kernel fd2962e0c5 |
| 7 | ok | 22.04 | kernel fd2962e0c5 |
| 8 | ok | 18.06 | kernel fd2962e0c5 |
