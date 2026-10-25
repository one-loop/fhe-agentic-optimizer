# chebyshev_sigmoid_d7: LLM optimization run 20261013-155720

Model `claude-haiku-5-5` (effort medium), 8 attempts, wall time 242 s, approximate API cost $0.0112.

| | fhe() time (ms) | levels | (depth, mult. depth) | ct-ct mul | ct-pt mul | rotations | max error |
|---|---:|---:|---|---:|---:|---:|---:|
| baseline | 330.23 | 8 | (22, 8) | 6 | 5 | 0 | 4.9e-08 |
| best (attempt 6) | 286.53 | 5 | (14, 5) | 4 | 6 | 0 | 1.1e-07 |

Speedup: x1.15.

| attempt | outcome | fhe() time (ms) | note |
|---:|---|---:|---|
| 1 | ok | 309.69 | new best |
| 2 | ok | 301.34 | new best |
| 3 | ok | 293.50 | new best |
| 4 | ok | 314.59 | kernel 698679be63 |
| 5 | ok | 287.82 | new best |
| 6 | ok | 286.53 | new best |
| 7 | ok | 294.14 | kernel 698679be63 |
| 8 | ok | 286.92 | kernel 698679be63 |
