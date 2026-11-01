# embedding: LLM optimization run 20261012-134110

Model `claude-haiku-5-5` (effort medium), 8 attempts, wall time 382 s, approximate API cost $0.0213.

| | fhe() time (ms) | levels | (depth, mult. depth) | ct-ct mul | ct-pt mul | rotations | max error |
|---|---:|---:|---|---:|---:|---:|---:|
| baseline | 620.08 | 2 | (21, 2) | 0 | 6 | 26 | 2.7e-05 |
| best (attempt 2) | 613.50 | 2 | (22, 2) | 0 | 6 | 26 | 2.2e-05 |

Speedup: x1.01.

| attempt | outcome | fhe() time (ms) | note |
|---:|---|---:|---|
| 1 | chehab_error | - | backend does not support plain plain evaluation |
| 2 | ok | 613.50 | new best |
| 3 | ok | 614.89 | kernel 9ef54ad120 |
| 4 | ok | 617.67 | kernel cdf158d3c2 |
| 5 | ok | 617.02 | kernel 9ef54ad120 |
| 6 | ok | 620.53 | kernel 9ef54ad120 |
| 7 | ok | 619.05 | kernel 9ef54ad120 |
| 8 | ok | 616.24 | kernel 9ef54ad120 |
