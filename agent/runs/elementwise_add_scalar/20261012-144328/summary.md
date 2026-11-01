# elementwise_add_scalar: LLM optimization run 20261012-144328

Model `claude-haiku-5-5` (effort medium), 8 attempts, wall time 71 s, approximate API cost $0.0018.

| | fhe() time (ms) | levels | (depth, mult. depth) | ct-ct mul | ct-pt mul | rotations | max error |
|---|---:|---:|---|---:|---:|---:|---:|
| baseline | 0.26 | 0 | (1, 0) | 0 | 0 | 0 | 1.8e-08 |
| best (attempt 3) | 0.25 | 0 | (1, 0) | 0 | 0 | 0 | 2.5e-08 |

Speedup: x1.05.

| attempt | outcome | fhe() time (ms) | note |
|---:|---|---:|---|
| 1 | ok | 0.26 | new best |
| 2 | ok | 0.27 | kernel 16c0c9f13f |
| 3 | ok | 0.25 | new best |
| 4 | ok | 0.27 | kernel 16c0c9f13f |
| 5 | ok | 0.26 | kernel 16c0c9f13f |
| 6 | ok | 0.26 | kernel 16c0c9f13f |
| 7 | ok | 0.26 | kernel 16c0c9f13f |
| 8 | ok | 0.26 | kernel 16c0c9f13f |
