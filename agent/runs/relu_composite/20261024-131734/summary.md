# relu_composite: LLM optimization run 20261024-131734

Model `claude-sonnet-5-5` (effort medium), 8 attempts, wall time 358 s, approximate API cost $0.0000.

| | fhe() time (ms) | levels | (depth, mult. depth) | ct-ct mul | ct-pt mul | rotations | max error |
|---|---:|---:|---|---:|---:|---:|---:|
| baseline | 639.39 | 10 | (17, 10) | 16 | 12 | 0 | 2.3e-07 |
| best (attempt 3) | 632.87 | 10 | (17, 10) | 15 | 11 | 0 | 1.2e-07 |

Speedup: x1.01.

| attempt | outcome | fhe() time (ms) | note |
|---:|---|---:|---|
| 1 | ok | 638.62 | new best |
| 2 | ok | 652.93 | kernel 28a20ff429 |
| 3 | ok | 632.87 | new best |
| 4 | ok | 637.34 | kernel 845df698f3 |
| 5 | ok | 634.72 | kernel 2a11518d13 |
| 6 | ok | 633.08 | kernel 845df698f3 |
| 7 | ok | 643.65 | kernel 845df698f3 |
| 8 | ok | 637.46 | kernel 845df698f3 |
