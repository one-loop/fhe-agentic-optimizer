# Feedback ablation

Speedups of each run's final kernel from neutral re-scoring. Reference: `independent` (no feedback to the model; the loop still verifies every kernel and keeps the fastest correct one). 8 model calls per run. Models: claude-haiku-5-5.

## Final kernels (mean speedup over runs; runs in brackets)

| task | independent | archive | linear |
|---|---:|---:|---:|
| chebyshev_elu_d7 | x1.087 (3; 1.07-1.10) | x1.060 (3; 1.00-1.10) | x1.089 (3; 1.08-1.10) |
| chebyshev_gelu_d7 | x1.434 (3; 1.39-1.48) | x1.423 (3; 1.41-1.43) | x1.417 (3; 1.36-1.47) |
| chebyshev_sigmoid_d7 | x1.200 (3; 1.19-1.21) | x1.390 (3; 1.22-1.59) | x1.162 (3; 1.08-1.22) |
| **geometric mean (3 tasks)** | **x1.232** | **x1.280** | **x1.215** |

## Effect of feedback (strategy vs `independent`)

Ratio > 1: the strategy with feedback found faster kernels than sampling the same number of kernels without feedback.

| strategy | tasks | geo. mean ratio | 95% bootstrap interval | paired runs | sign-flip p | runs better / worse / tied (2%) |
|---|---:|---:|---|---:|---:|---|
| archive | 3 | x1.039 | x0.994 - x1.086 | 9 | 0.410 | 3 / 2 / 4 |
| linear | 3 | x0.986 | x0.960 - x1.012 | 9 | 0.344 | 2 / 2 / 5 |

With 3 runs per task the interval is wide; treat a ratio within it as "no detectable effect", not as "no effect".

## Search behaviour (in-loop timings, not neutral)

Best-so-far speedup after k calls, mean over runs and tasks (the baseline counts as found at k = 0).

| strategy | k=1 | k=2 | k=3 | k=4 | k=5 | k=6 | k=7 | k=8 |
|---|---:|---:|---:|---:|---:|---:|---:|---:|
| independent | x1.114 | x1.211 | x1.223 | x1.223 | x1.224 | x1.237 | x1.244 | x1.245 |
| archive | x1.176 | x1.219 | x1.219 | x1.246 | x1.287 | x1.290 | x1.291 | x1.297 |
| linear | x1.149 | x1.191 | x1.199 | x1.208 | x1.208 | x1.217 | x1.227 | x1.227 |

| strategy | runs | correct attempts | duplicates | improved on baseline | failures (all runs) |
|---|---:|---:|---:|---:|---|
| independent | 9 | 51 / 72 | 1 | 9 / 9 | chehab_error 12, wrong_output 2, predicted_slower 6 |
| archive | 9 | 51 / 72 | 9 | 8 / 9 | chehab_error 10, predicted_slower 2 |
| linear | 9 | 41 / 72 | 27 | 9 / 9 | chehab_error 2, wrong_output 2 |

## Sanity check: call 1

Call 1 is the same prompt under `independent` and `linear`, so these should agree up to sampling noise.

| strategy | runs | mean call-1 speedup | correct at call 1 |
|---|---:|---:|---:|
| independent | 9 | x1.114 | 7 / 9 |
| linear | 9 | x1.149 | 7 / 9 |
