# chebyshev_silu_d7: LLM optimization run 20261024-123429

Model `claude-sonnet-5-5` (effort medium), 8 attempts, wall time 242 s, approximate API cost $0.0000.

| | fhe() time (ms) | levels | (depth, mult. depth) | ct-ct mul | ct-pt mul | rotations | max error |
|---|---:|---:|---|---:|---:|---:|---:|
| baseline | 307.11 | 7 | (19, 7) | 5 | 5 | 0 | 1.1e-07 |
| best (attempt 7) | 199.19 | 5 | (18, 5) | 3 | 3 | 0 | 1.5e-07 |

Speedup: x1.54.

| attempt | outcome | fhe() time (ms) | note |
|---:|---|---:|---|
| 1 | ok | 260.76 | new best |
| 2 | ok | 232.73 | new best |
| 3 | ok | 216.22 | new best |
| 4 | ok | 201.50 | new best |
| 5 | ok | 204.45 | kernel 70b7505dae |
| 6 | ok | 217.06 | kernel 212cfe00e2 |
| 7 | ok | 199.19 | new best |
| 8 | ok | 203.27 | kernel 212cfe00e2 |
