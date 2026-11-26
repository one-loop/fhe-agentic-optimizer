# Feedback ablation

Speedups of each run's final kernel from neutral re-scoring. Reference: `independent` (no feedback to the model; the loop still verifies every kernel and keeps the fastest correct one). 8 model calls per run. Models: claude-haiku-5-5.

## Final kernels (mean speedup over runs; runs in brackets)

| task | independent | archive | archive-explore | linear | linear-explore | linear-minimal |
|---|---:|---:|---:|---:|---:|---:|
| chebyshev_elu_d7 | x2.014 (1; 2.01-2.01) | x2.577 (1; 2.58-2.58) | x2.581 (1; 2.58-2.58) | x2.009 (1; 2.01-2.01) | x2.009 (1; 2.01-2.01) | x1.000 (1; 1.00-1.00) |
| chebyshev_gelu_d7 | x2.896 (1; 2.90-2.90) | x2.938 (1; 2.94-2.94) | x2.882 (1; 2.88-2.88) | x2.889 (1; 2.89-2.89) | x2.908 (1; 2.91-2.91) | x2.889 (1; 2.89-2.89) |
| chebyshev_sigmoid_d7 | x2.013 (1; 2.01-2.01) | x2.323 (1; 2.32-2.32) | x2.328 (1; 2.33-2.33) | x1.644 (1; 1.64-1.64) | x2.512 (1; 2.51-2.51) | x2.711 (1; 2.71-2.71) |
| coyote_tree_100_50_5_1 | x1.000 (1; 1.00-1.00) | x1.000 (1; 1.00-1.00) | x1.000 (1; 1.00-1.00) | x1.000 (1; 1.00-1.00) | x1.000 (1; 1.00-1.00) | x1.000 (1; 1.00-1.00) |
| extract_sparse | x1.283 (1; 1.28-1.28) | x1.287 (1; 1.29-1.29) | x1.290 (1; 1.29-1.29) | x1.283 (1; 1.28-1.28) | x1.290 (1; 1.29-1.29) | x1.287 (1; 1.29-1.29) |
| matmul_2x2 | x1.407 (1; 1.41-1.41) | x1.565 (1; 1.56-1.56) | x1.349 (1; 1.35-1.35) | x1.569 (1; 1.57-1.57) | x1.564 (1; 1.56-1.56) | x1.406 (1; 1.41-1.41) |
| **geometric mean (6 tasks)** | **x1.664** | **x1.812** | **x1.764** | **x1.636** | **x1.759** | **x1.556** |

## Primary comparisons (declared before the runs; Holm-corrected)

Ratio > 1: the first strategy's final kernels were faster.

| strategy | against | tasks | geo. mean ratio | 95% bootstrap interval | paired runs | sign-flip p | Holm p | runs better / worse / tied (2%) |
|---|---|---:|---:|---|---:|---:|---:|---|
| linear | independent | 6 | x0.984 | x0.984 - x0.984 | 6 | 0.5625 | 1.0000 | 1 / 1 / 4 |
| linear | linear-minimal | 6 | x1.052 | x1.052 - x1.052 | 6 | 0.7500 | 1.0000 | 2 / 1 / 3 |
| linear-explore | linear | 6 | x1.075 | x1.075 - x1.075 | 6 | 0.1875 | 0.7500 | 1 / 0 / 5 |
| archive | linear | 6 | x1.108 | x1.108 - x1.108 | 6 | 0.1250 | 0.6250 | 2 / 0 / 4 |
| archive-explore | archive | 6 | x0.973 | x0.973 - x0.973 | 6 | 0.5000 | 1.0000 | 0 / 1 / 5 |

- linear vs independent: does feedback to the model help?
- linear vs linear-minimal: does the performance content of the feedback help (beyond correctness)?
- linear-explore vs linear: does steering toward new approaches help the dialogue?
- archive vs linear: does search on top of feedback help?
- archive-explore vs archive: does steering toward new approaches help the search?

## Every other strategy vs `independent` (secondary, uncorrected)

| strategy | against | tasks | geo. mean ratio | 95% bootstrap interval | paired runs | sign-flip p | runs better / worse / tied (2%) |
|---|---|---:|---:|---|---:|---:|---|
| archive | independent | 6 | x1.089 | x1.089 - x1.089 | 6 | 0.0625 | 3 / 0 / 3 |
| archive-explore | independent | 6 | x1.060 | x1.060 - x1.060 | 6 | 0.3750 | 2 / 1 / 3 |
| linear-explore | independent | 6 | x1.057 | x1.057 - x1.057 | 6 | 0.1250 | 2 / 0 / 4 |
| linear-minimal | independent | 6 | x0.935 | x0.935 - x0.935 | 6 | 0.7500 | 1 / 1 / 4 |

Treat a ratio whose interval includes x1.000 as "no detectable effect", not as "no effect".

## Diversity of the proposals (per run, mean over runs)

distinct kernels: proposals that were not identical to an earlier kernel; distinct structures: new operation-count fingerprints (ct x ct, ct x pt, rotations, levels, alignments, mod switches), the baseline's excluded; structure repeats: evaluated proposals with a fingerprint already seen; techniques: distinct `Technique:` lines (explore arms only).

| strategy | runs | distinct kernels | duplicates | distinct structures | structure repeats | distinct techniques |
|---|---:|---:|---:|---:|---:|---:|
| independent | 6 | 8.00 | 0.00 | 3.17 | 2.83 | 0.00 |
| archive | 6 | 6.33 | 1.67 | 2.50 | 3.50 | 0.00 |
| archive-explore | 6 | 7.83 | 0.17 | 3.83 | 3.33 | 8.00 |
| linear | 6 | 5.50 | 2.50 | 2.83 | 2.50 | 0.00 |
| linear-explore | 6 | 7.50 | 0.50 | 3.67 | 2.67 | 8.00 |
| linear-minimal | 6 | 5.33 | 2.67 | 2.17 | 2.67 | 0.00 |

## Search behaviour (in-loop timings, not neutral)

Best-so-far speedup after k calls, mean over runs and tasks (the baseline counts as found at k = 0).

| strategy | k=1 | k=2 | k=3 | k=4 | k=5 | k=6 | k=7 | k=8 |
|---|---:|---:|---:|---:|---:|---:|---:|---:|
| independent | x1.251 | x1.732 | x1.740 | x1.740 | x1.758 | x1.758 | x1.773 | x1.773 |
| archive | x1.602 | x1.807 | x1.849 | x1.850 | x1.850 | x1.957 | x1.960 | x1.966 |
| archive-explore | x1.336 | x1.695 | x1.758 | x1.796 | x1.905 | x1.905 | x1.905 | x1.913 |
| linear | x1.308 | x1.487 | x1.668 | x1.692 | x1.735 | x1.735 | x1.744 | x1.744 |
| linear-explore | x1.174 | x1.352 | x1.652 | x1.737 | x1.865 | x1.882 | x1.886 | x1.886 |
| linear-minimal | x1.302 | x1.549 | x1.717 | x1.717 | x1.717 | x1.722 | x1.722 | x1.722 |

| strategy | runs | correct attempts | duplicates | improved on baseline | failures (all runs) |
|---|---:|---:|---:|---:|---|
| independent | 6 | 29 / 48 | 0 | 5 / 6 | chehab_error 12, wrong_output 3, predicted_slower 4 |
| archive | 6 | 34 / 48 | 10 | 5 / 6 | chehab_error 2, predicted_slower 2 |
| archive-explore | 6 | 35 / 48 | 1 | 5 / 6 | chehab_error 4, wrong_output 2, predicted_slower 6 |
| linear | 6 | 31 / 48 | 15 | 5 / 6 | chehab_error 1, wrong_output 1 |
| linear-explore | 6 | 37 / 48 | 3 | 5 / 6 | compile_error 1, chehab_error 6, predicted_slower 1 |
| linear-minimal | 6 | 29 / 48 | 16 | 4 / 6 | chehab_error 3 |

## Sanity check: call 1

Call 1 is the same prompt under these strategies, so they should agree up to sampling noise.

| strategy | runs | mean call-1 speedup | correct at call 1 |
|---|---:|---:|---:|
| independent | 6 | x1.251 | 4 / 6 |
| linear | 6 | x1.308 | 4 / 6 |
| linear-minimal | 6 | x1.302 | 4 / 6 |
