# table_mult: LLM optimization run 20261025-141348

Model `claude-sonnet-5-5` (effort medium), 8 attempts, wall time 364 s, approximate API cost $0.0000.

| | fhe() time (ms) | levels | (depth, mult. depth) | ct-ct mul | ct-pt mul | rotations | max error |
|---|---:|---:|---|---:|---:|---:|---:|
| baseline | 573.38 | 1 | (17, 1) | 0 | 3 | 24 | 6.1e-05 |
| best (attempt 0) | 573.38 | 1 | (17, 1) | 0 | 3 | 24 | 6.1e-05 |

Speedup: x1.00 (baseline and best timed in 7 alternating batches; 1.00 when no candidate was confirmed faster).

| attempt | outcome | fhe() time (ms) | verdict | note |
|---:|---|---:|---|---|
| 1 | ok | - | same_as_baseline |  |
| 2 | ok | 563.29 | not_confirmed, x1.00 vs attempt 0 |  |
| 3 | ok | - | same_as_baseline |  |
| 4 | ok | 563.05 | not_confirmed, x1.00 vs attempt 0 |  |
| 5 | run_error | - | failed | result ciphertext is transparent |
| 6 | ok | - | same_as_baseline |  |
| 7 | chehab_error | - | failed | backend does not support plain plain evaluation |
| 8 | ok | - | same_as_baseline |  |
