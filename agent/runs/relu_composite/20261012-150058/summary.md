# relu_composite: LLM optimization run 20261012-150058

Model `claude-haiku-5-5` (effort medium), 8 attempts, wall time 216 s, approximate API cost $0.0134.

| | fhe() time (ms) | levels | (depth, mult. depth) | ct-ct mul | ct-pt mul | rotations | max error |
|---|---:|---:|---|---:|---:|---:|---:|
| baseline | 318.50 | 10 | (17, 10) | 16 | 12 | 0 | 1.9e-07 |
| best (attempt 3) | 317.94 | 10 | (17, 10) | 16 | 12 | 0 | 1.6e-07 |

Speedup: x1.00.

| attempt | outcome | fhe() time (ms) | note |
|---:|---|---:|---|
| 1 | ok | 317.94 | new best |
| 2 | ok | 327.31 | kernel ad876963ea |
| 3 | ok | 317.94 | new best |
| 4 | ok | 317.94 | kernel 940201c016 |
| 5 | ok | 318.16 | kernel 940201c016 |
| 6 | ok | 318.30 | kernel 940201c016 |
| 7 | ok | 318.41 | kernel 940201c016 |
| 8 | ok | 318.45 | kernel 940201c016 |
