# elementwise_add_scalar: LLM optimization run 20261024-124619

Model `claude-sonnet-5-5` (effort medium), 8 attempts, wall time 123 s, approximate API cost $0.0000.

| | fhe() time (ms) | levels | (depth, mult. depth) | ct-ct mul | ct-pt mul | rotations | max error |
|---|---:|---:|---|---:|---:|---:|---:|
| baseline | 0.55 | 0 | (1, 0) | 0 | 0 | 0 | 2.3e-08 |
| best (attempt 2) | 0.38 | 0 | (1, 0) | 0 | 0 | 0 | 2.5e-08 |

Speedup: x1.43.

| attempt | outcome | fhe() time (ms) | note |
|---:|---|---:|---|
| 1 | ok | 0.54 | new best |
| 2 | ok | 0.38 | new best |
| 3 | ok | 0.44 | kernel 16c0c9f13f |
| 4 | ok | 0.43 | kernel 16c0c9f13f |
| 5 | ok | 0.45 | kernel 16c0c9f13f |
| 6 | ok | 0.43 | kernel 16c0c9f13f |
| 7 | ok | 0.45 | kernel 16c0c9f13f |
| 8 | ok | 0.43 | kernel 16c0c9f13f |
