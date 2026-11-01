# chebyshev_sigmoid_d7: LLM optimization run 20261012-133130

Model `claude-haiku-5-5` (effort medium), 8 attempts, wall time 9 s, approximate API cost $0.0000.

| | fhe() time (ms) | levels | (depth, mult. depth) | ct-ct mul | ct-pt mul | rotations | max error |
|---|---:|---:|---|---:|---:|---:|---:|
| baseline | 160.65 | 8 | (22, 8) | 6 | 5 | 0 | 6.2e-08 |
| best (attempt 0) | 160.65 | 8 | (22, 8) | 6 | 5 | 0 | 6.2e-08 |

Speedup: x1.00.

| attempt | outcome | fhe() time (ms) | note |
|---:|---|---:|---|
| 1 | api_error | - | Error code: 401 - {'type': 'error', 'error': {'type': 'authentication_error', 'message': 'invalid x-api-key'}, |
