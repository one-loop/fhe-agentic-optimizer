# chebyshev_gelu_d7: LLM optimization run 20261012-141454

Model `claude-haiku-5-5` (effort medium), 8 attempts, wall time 126 s, approximate API cost $0.0074.

| | fhe() time (ms) | levels | (depth, mult. depth) | ct-ct mul | ct-pt mul | rotations | max error |
|---|---:|---:|---|---:|---:|---:|---:|
| baseline | 152.48 | 7 | (19, 7) | 5 | 5 | 0 | 9.4e-08 |
| best (attempt 3) | 115.88 | 5 | (13, 5) | 3 | 5 | 0 | 1.6e-07 |

Speedup: x1.32.

| attempt | outcome | fhe() time (ms) | note |
|---:|---|---:|---|
| 1 | ok | 115.98 | new best |
| 2 | ok | 116.25 | kernel 50904cac9b |
| 3 | ok | 115.88 | new best |
| 4 | ok | 115.99 | kernel cedc865271 |
| 5 | ok | 115.96 | kernel cedc865271 |
| 6 | ok | 116.09 | kernel cedc865271 |
| 7 | ok | 115.99 | kernel cedc865271 |
| 8 | ok | 116.07 | kernel cedc865271 |
