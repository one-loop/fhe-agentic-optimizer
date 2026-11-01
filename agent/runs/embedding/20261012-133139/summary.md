# embedding: LLM optimization run 20261012-133139

Model `claude-haiku-5-5` (effort medium), 8 attempts, wall time 27 s, approximate API cost $0.0000.

| | fhe() time (ms) | levels | (depth, mult. depth) | ct-ct mul | ct-pt mul | rotations | max error |
|---|---:|---:|---|---:|---:|---:|---:|
| baseline | 615.09 | 2 | (21, 2) | 0 | 6 | 26 | 3.2e-05 |
| best (attempt 0) | 615.09 | 2 | (21, 2) | 0 | 6 | 26 | 3.2e-05 |

Speedup: x1.00.

| attempt | outcome | fhe() time (ms) | note |
|---:|---|---:|---|
| 1 | api_error | - | Error code: 401 - {'type': 'error', 'error': {'type': 'authentication_error', 'message': 'invalid x-api-key'}, |
