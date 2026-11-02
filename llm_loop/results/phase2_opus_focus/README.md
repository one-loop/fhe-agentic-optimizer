# Opus 5.5 in my loop: GELU, Sigmoid d7, ELU

Archive strategy, hints none, 3 runs per task, 8 calls per run.

Not yet confirmed: the run-1 kernels of GELU (x2.63) and Sigmoid d7 (x2.55) consume more levels than the
baseline (10 against 7 and 8), and GELU's FHE error, 5.9e-07, is the closest to the 1e-06 tolerance.
Each passed all 8 cases, but once. A repeated correctness check is pending before these two are quoted.
