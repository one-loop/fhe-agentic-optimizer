# relu_composite: LLM optimization run 20261012-133206

Model `claude-haiku-5-5` (effort medium), 8 attempts, wall time 14 s, approximate API cost $0.0000.

| | fhe() time (ms) | levels | (depth, mult. depth) | ct-ct mul | ct-pt mul | rotations | max error |
|---|---:|---:|---|---:|---:|---:|---:|
| baseline | 318.60 | 10 | (17, 10) | 16 | 12 | 0 | 2.1e-07 |
| best (attempt 0) | 318.60 | 10 | (17, 10) | 16 | 12 | 0 | 2.1e-07 |

Speedup: x1.00.

| attempt | outcome | fhe() time (ms) | note |
|---:|---|---:|---|
| 1 | api_error | - | Error code: 401 - {'type': 'error', 'error': {'type': 'authentication_error', 'message': 'invalid x-api-key'}, |
