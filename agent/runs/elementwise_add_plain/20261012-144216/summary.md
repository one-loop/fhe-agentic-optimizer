# elementwise_add_plain: LLM optimization run 20261012-144216

Model `claude-haiku-5-5` (effort medium), 8 attempts, wall time 72 s, approximate API cost $0.0016.

| | fhe() time (ms) | levels | (depth, mult. depth) | ct-ct mul | ct-pt mul | rotations | max error |
|---|---:|---:|---|---:|---:|---:|---:|
| baseline | 2.69 | 0 | (1, 0) | 0 | 0 | 0 | 2.9e-08 |
| best (attempt 6) | 2.49 | 0 | (1, 0) | 0 | 0 | 0 | 1.8e-08 |

Speedup: x1.08.

| attempt | outcome | fhe() time (ms) | note |
|---:|---|---:|---|
| 1 | ok | 2.69 | new best |
| 2 | ok | 2.71 | kernel dd71079cef |
| 3 | ok | 2.51 | new best |
| 4 | ok | 2.64 | kernel dd71079cef |
| 5 | ok | 2.71 | kernel dd71079cef |
| 6 | ok | 2.49 | new best |
| 7 | ok | 2.70 | kernel dd71079cef |
| 8 | ok | 2.69 | kernel dd71079cef |
