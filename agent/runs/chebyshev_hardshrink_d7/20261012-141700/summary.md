# chebyshev_hardshrink_d7: LLM optimization run 20261012-141700

Model `claude-haiku-5-5` (effort medium), 8 attempts, wall time 173 s, approximate API cost $0.0131.

| | fhe() time (ms) | levels | (depth, mult. depth) | ct-ct mul | ct-pt mul | rotations | max error |
|---|---:|---:|---|---:|---:|---:|---:|
| baseline | 161.19 | 8 | (21, 8) | 6 | 5 | 0 | 1.1e-07 |
| best (attempt 2) | 136.17 | 6 | (18, 6) | 4 | 5 | 0 | 9.3e-08 |

Speedup: x1.18.

| attempt | outcome | fhe() time (ms) | note |
|---:|---|---:|---|
| 1 | ok | 136.44 | new best |
| 2 | ok | 136.17 | new best |
| 3 | ok | 136.41 | kernel b8e9098c75 |
| 4 | ok | 136.26 | kernel b8e9098c75 |
| 5 | ok | 136.61 | kernel b8e9098c75 |
| 6 | ok | 136.34 | kernel b8e9098c75 |
| 7 | ok | 136.70 | kernel b8e9098c75 |
| 8 | ok | 136.42 | kernel b8e9098c75 |
