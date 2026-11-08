# chebyshev_selu_d7: LLM optimization run 20261025-165201

Model `claude-sonnet-5-5` (effort medium), 8 attempts, wall time 357 s, approximate API cost $0.0000.

| | fhe() time (ms) | levels | (depth, mult. depth) | ct-ct mul | ct-pt mul | rotations | max error |
|---|---:|---:|---|---:|---:|---:|---:|
| baseline | 358.66 | 8 | (22, 8) | 6 | 8 | 0 | 1.3e-07 |
| best (attempt 4) | 251.57 | 8 | (22, 8) | 6 | 2 | 0 | 7.7e-08 |

Speedup: x1.42 (baseline and best timed in 7 alternating batches; 1.00 when no candidate was confirmed faster).

| attempt | outcome | fhe() time (ms) | verdict | note |
|---:|---|---:|---|---|
| 1 | chehab_error | - | failed | CKKS scale mismatch: ciphertext operands of '+' (term 37) are at the same level 4 with different scales; align |
| 2 | chehab_error | - | failed | CKKS scale mismatch: ciphertext operands of '+' (term 40) are at the same level 5 with different scales; align |
| 3 | chehab_error | - | failed | CKKS scale mismatch: ciphertext operands of '+' (term 47) are at the same level 3 with different scales; align |
| 4 | ok | 251.57 | new_best, x1.43 vs attempt 0 |  |
| 5 | ok | 330.30 | slower |  |
| 6 | ok | 289.11 | slower |  |
| 7 | ok | 318.21 | slower |  |
| 8 | compile_error | - | failed | kernel:9:14: error: no viable conversion from 'Plaintext' to 'Ciphertext' |
