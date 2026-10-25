# chebyshev_sigmoid_d5: LLM optimization run 20261013-155303

Model `claude-haiku-5-5` (effort medium), 8 attempts, wall time 257 s, approximate API cost $0.0174.

| | fhe() time (ms) | levels | (depth, mult. depth) | ct-ct mul | ct-pt mul | rotations | max error |
|---|---:|---:|---|---:|---:|---:|---:|
| baseline | 257.56 | 6 | (16, 6) | 4 | 4 | 0 | 5.5e-08 |
| best (attempt 5) | 227.54 | 5 | (13, 5) | 3 | 4 | 0 | 5.4e-08 |

Speedup: x1.13.

| attempt | outcome | fhe() time (ms) | note |
|---:|---|---:|---|
| 1 | ok | 267.94 | kernel 8d00140b0b |
| 2 | ok | 239.91 | new best |
| 3 | ok | 239.88 | new best |
| 4 | ok | 229.15 | new best |
| 5 | ok | 227.54 | new best |
| 6 | ok | 229.05 | kernel 012db8d6c2 |
| 7 | ok | 228.34 | kernel 012db8d6c2 |
| 8 | ok | 228.02 | kernel 012db8d6c2 |
