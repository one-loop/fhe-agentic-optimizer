# chebyshev_softplus_d7: LLM optimization run 20261012-143855

Model `claude-haiku-5-5` (effort medium), 8 attempts, wall time 130 s, approximate API cost $0.0080.

| | fhe() time (ms) | levels | (depth, mult. depth) | ct-ct mul | ct-pt mul | rotations | max error |
|---|---:|---:|---|---:|---:|---:|---:|
| baseline | 152.28 | 7 | (19, 7) | 5 | 5 | 0 | 5.6e-08 |
| best (attempt 4) | 115.69 | 5 | (13, 5) | 3 | 5 | 0 | 1e-07 |

Speedup: x1.32.

| attempt | outcome | fhe() time (ms) | note |
|---:|---|---:|---|
| 1 | ok | 115.84 | new best |
| 2 | ok | 116.10 | kernel 2a4c5caef7 |
| 3 | ok | 115.83 | new best |
| 4 | ok | 115.69 | new best |
| 5 | ok | 115.96 | kernel fe92edebbc |
| 6 | ok | 115.95 | kernel fe92edebbc |
| 7 | ok | 115.98 | kernel fe92edebbc |
| 8 | ok | 115.87 | kernel fe92edebbc |
