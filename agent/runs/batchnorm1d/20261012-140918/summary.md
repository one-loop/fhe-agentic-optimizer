# batchnorm1d: LLM optimization run 20261012-140918

Model `claude-haiku-5-5` (effort medium), 8 attempts, wall time 78 s, approximate API cost $0.0026.

| | fhe() time (ms) | levels | (depth, mult. depth) | ct-ct mul | ct-pt mul | rotations | max error |
|---|---:|---:|---|---:|---:|---:|---:|
| baseline | 9.59 | 1 | (2, 1) | 0 | 1 | 0 | 3.6e-08 |
| best (attempt 0) | 9.59 | 1 | (2, 1) | 0 | 1 | 0 | 3.6e-08 |

Speedup: x1.00.

| attempt | outcome | fhe() time (ms) | note |
|---:|---|---:|---|
| 1 | ok | 9.66 | kernel 7348c69a96 |
| 2 | ok | 9.77 | kernel 5f055ea03d |
| 3 | ok | 9.81 | kernel 5f055ea03d |
| 4 | ok | 9.84 | kernel 5f055ea03d |
| 5 | ok | 9.86 | kernel 5f055ea03d |
| 6 | ok | 9.87 | kernel 5f055ea03d |
| 7 | ok | 9.88 | kernel 5f055ea03d |
| 8 | ok | 9.84 | kernel 5f055ea03d |
