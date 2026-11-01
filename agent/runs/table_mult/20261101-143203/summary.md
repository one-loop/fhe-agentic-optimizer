# table_mult: LLM optimization run 20261101-143203

Model `claude-opus-5-5` (effort medium), 8 attempts, wall time 493 s, approximate API cost $0.0000.

| | fhe() time (ms) | levels | (depth, mult. depth) | ct-ct mul | ct-pt mul | rotations | max error |
|---|---:|---:|---|---:|---:|---:|---:|
| baseline | 569.63 | 1 | (17, 1) | 0 | 3 | 24 | 2.7e-05 |
| best (attempt 5) | 525.92 | 2 | (18, 2) | 0 | 5 | 24 | 4.2e-05 |

Speedup: x1.07 (baseline and best timed in 7 alternating batches; 1.00 when no candidate was confirmed faster).

| attempt | outcome | fhe() time (ms) | verdict | note |
|---:|---|---:|---|---|
| 1 | wrong_output | 127.60 | failed | wrong result on case fixed (8 slots): max abs error 0.0336 > tolerance 0.0001. The kernel does not compute the |
| 2 | wrong_output | 370.38 | failed | wrong result on case dense (256 slots): max abs error 1.21 > tolerance 0.0001. The kernel does not compute the |
| 3 | wrong_output | 502.30 | failed | wrong result on case dense (256 slots): max abs error 0.0001 > tolerance 0.0001. The kernel does not compute t |
| 4 | run_error | - | failed | result ciphertext is transparent |
| 5 | ok | 525.92 | new_best, x1.06 vs attempt 0 |  |
| 6 | ok | - | same_as_best (attempt 5) |  |
| 7 | ok | - | same_as_best (attempt 5) |  |
| 8 | wrong_output | 530.19 | failed | wrong result on case dense (256 slots): max abs error 0.00309 > tolerance 0.0001. The kernel does not compute  |
