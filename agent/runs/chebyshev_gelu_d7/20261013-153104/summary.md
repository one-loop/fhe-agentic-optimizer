# chebyshev_gelu_d7: LLM optimization run 20261013-153104

Model `claude-haiku-5-5` (effort medium), 8 attempts, wall time 197 s, approximate API cost $0.0082.

| | fhe() time (ms) | levels | (depth, mult. depth) | ct-ct mul | ct-pt mul | rotations | max error |
|---|---:|---:|---|---:|---:|---:|---:|
| baseline | 304.31 | 7 | (19, 7) | 5 | 5 | 0 | 1.3e-07 |
| best (attempt 4) | 231.47 | 5 | (12, 5) | 3 | 5 | 0 | 1.2e-07 |

Speedup: x1.31.

| attempt | outcome | fhe() time (ms) | note |
|---:|---|---:|---|
| 1 | ok | 237.64 | new best |
| 2 | ok | 231.69 | new best |
| 3 | ok | 231.68 | new best |
| 4 | ok | 231.47 | new best |
| 5 | ok | 231.69 | kernel 7c4540ee51 |
| 6 | ok | 237.55 | kernel 7c4540ee51 |
| 7 | ok | 232.53 | kernel 7c4540ee51 |
| 8 | ok | 231.84 | kernel 7c4540ee51 |
