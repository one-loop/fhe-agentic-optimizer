# embedding: LLM optimization run 20261013-161908

Model `claude-haiku-5-5` (effort medium), 8 attempts, wall time 612 s, approximate API cost $0.0147.

| | fhe() time (ms) | levels | (depth, mult. depth) | ct-ct mul | ct-pt mul | rotations | max error |
|---|---:|---:|---|---:|---:|---:|---:|
| baseline | 1324.11 | 2 | (21, 2) | 0 | 6 | 26 | 2.9e-05 |
| best (attempt 1) | 1285.32 | 2 | (22, 2) | 0 | 6 | 26 | 1.3e-05 |

Speedup: x1.03.

| attempt | outcome | fhe() time (ms) | note |
|---:|---|---:|---|
| 1 | ok | 1285.32 | new best |
| 2 | ok | 1464.69 | kernel 5459837b1d |
| 3 | ok | 1648.65 | kernel 5459837b1d |
| 4 | ok | 1300.56 | kernel 5459837b1d |
| 5 | ok | 1323.72 | kernel 5459837b1d |
| 6 | ok | 1306.77 | kernel 5459837b1d |
| 7 | ok | 1354.22 | kernel 5459837b1d |
| 8 | ok | 1329.12 | kernel 5459837b1d |
