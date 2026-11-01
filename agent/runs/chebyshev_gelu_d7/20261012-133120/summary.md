# chebyshev_gelu_d7: LLM optimization run 20261012-133120

Model `claude-haiku-5-5` (effort medium), 8 attempts, wall time 10 s, approximate API cost $0.0000.

| | fhe() time (ms) | levels | (depth, mult. depth) | ct-ct mul | ct-pt mul | rotations | max error |
|---|---:|---:|---|---:|---:|---:|---:|
| baseline | 150.36 | 7 | (19, 7) | 5 | 5 | 0 | 9.9e-08 |
| best (attempt 0) | 150.36 | 7 | (19, 7) | 5 | 5 | 0 | 9.9e-08 |

Speedup: x1.00.

| attempt | outcome | fhe() time (ms) | note |
|---:|---|---:|---|
| 1 | api_error | - | Error code: 401 - {'type': 'error', 'error': {'type': 'authentication_error', 'message': 'invalid x-api-key'}, |
