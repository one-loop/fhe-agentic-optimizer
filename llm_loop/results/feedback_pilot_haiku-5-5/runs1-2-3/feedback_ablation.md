# Feedback ablation

Speedups of each run's final kernel from neutral re-scoring. Reference: `independent` (no feedback to the model; the loop still verifies every kernel and keeps the fastest correct one). 8 model calls per run. Models: claude-haiku-5-5.

## Final kernels (mean speedup over runs; runs in brackets)

| task | independent | archive | archive-explore | linear | linear-explore | linear-minimal |
|---|---:|---:|---:|---:|---:|---:|
| chebyshev_elu_d7 | x1.662 (3; 1.00-2.02) | x2.188 (3; 1.98-2.58) | x2.190 (3; 1.97-2.58) | x1.837 (3; 1.49-2.01) | x2.140 (3; 1.98-2.43) | x1.661 (3; 1.00-2.01) |
| chebyshev_gelu_d7 | x2.903 (3; 2.90-2.91) | x2.906 (3; 2.87-2.94) | x2.612 (3; 2.02-2.94) | x2.905 (3; 2.89-2.92) | x2.898 (3; 2.89-2.91) | x2.899 (3; 2.89-2.91) |
| chebyshev_sigmoid_d7 | x1.883 (3; 1.80-2.01) | x2.534 (3; 2.32-2.70) | x1.941 (3; 1.00-2.50) | x2.070 (3; 1.64-2.46) | x2.447 (3; 2.32-2.51) | x2.420 (3; 2.11-2.71) |
| coyote_tree_100_50_5_1 | x1.000 (3; 1.00-1.00) | x1.000 (3; 1.00-1.00) | x1.000 (3; 1.00-1.00) | x1.000 (3; 1.00-1.00) | x1.000 (3; 1.00-1.00) | x1.000 (3; 1.00-1.00) |
| extract_sparse | x1.287 (3; 1.29-1.29) | x1.290 (3; 1.29-1.29) | x1.287 (3; 1.29-1.29) | x1.288 (3; 1.29-1.29) | x1.288 (3; 1.28-1.29) | x1.286 (3; 1.28-1.29) |
| matmul_2x2 | x1.334 (3; 1.30-1.41) | x1.562 (3; 1.56-1.56) | x1.491 (3; 1.35-1.56) | x1.564 (3; 1.56-1.57) | x1.562 (3; 1.56-1.56) | x1.405 (3; 1.40-1.41) |
| **geometric mean (6 tasks)** | **x1.581** | **x1.786** | **x1.665** | **x1.677** | **x1.768** | **x1.662** |

## Primary comparisons (declared before the runs; Holm-corrected)

Ratio > 1: the first strategy's final kernels were faster.

| strategy | against | tasks | geo. mean ratio | 95% bootstrap interval | paired runs | sign-flip p | Holm p | runs better / worse / tied (2%) |
|---|---|---:|---:|---|---:|---:|---:|---|
| linear | independent | 6 | x1.061 | x0.997 - x1.146 | 18 | 0.2195 | 0.4390 | 6 / 2 / 10 |
| linear | linear-minimal | 6 | x1.009 | x0.946 - x1.091 | 18 | 0.7505 | 0.7505 | 4 / 2 / 12 |
| linear-explore | linear | 6 | x1.054 | x1.012 - x1.105 | 18 | 0.0507 | 0.1721 | 5 / 0 / 13 |
| archive | linear | 6 | x1.065 | x1.019 - x1.120 | 18 | 0.0291 | 0.1453 | 5 / 0 / 13 |
| archive-explore | archive | 6 | x0.932 | x0.842 - x0.999 | 18 | 0.0430 | 0.1721 | 0 / 4 / 14 |

- linear vs independent: does feedback to the model help?
- linear vs linear-minimal: does the performance content of the feedback help (beyond correctness)?
- linear-explore vs linear: does steering toward new approaches help the dialogue?
- archive vs linear: does search on top of feedback help?
- archive-explore vs archive: does steering toward new approaches help the search?

## Every other strategy vs `independent` (secondary, uncorrected)

| strategy | against | tasks | geo. mean ratio | 95% bootstrap interval | paired runs | sign-flip p | runs better / worse / tied (2%) |
|---|---|---:|---:|---|---:|---:|---|
| archive | independent | 6 | x1.130 | x1.074 - x1.219 | 18 | 0.0022 | 8 / 0 / 10 |
| archive-explore | independent | 6 | x1.053 | x0.945 - x1.159 | 18 | 0.5041 | 6 / 3 / 9 |
| linear-explore | independent | 6 | x1.118 | x1.067 - x1.209 | 18 | 0.0106 | 7 / 0 / 11 |
| linear-minimal | independent | 6 | x1.051 | x0.967 - x1.144 | 18 | 0.4399 | 6 / 1 / 11 |

Treat a ratio whose interval includes x1.000 as "no detectable effect", not as "no effect".

## Diversity of the proposals (per run, mean over runs)

distinct kernels: proposals that were not identical to an earlier kernel; distinct structures: new operation-count fingerprints (ct x ct, ct x pt, rotations, levels, alignments, mod switches), the baseline's excluded; structure repeats: evaluated proposals with a fingerprint already seen; techniques: distinct `Technique:` lines (explore arms only).

| strategy | runs | distinct kernels | duplicates | distinct structures | structure repeats | distinct techniques |
|---|---:|---:|---:|---:|---:|---:|
| independent | 18 | 7.94 | 0.06 | 2.78 | 2.89 | 0.00 |
| archive | 18 | 6.56 | 1.44 | 2.56 | 3.61 | 0.00 |
| archive-explore | 18 | 7.67 | 0.33 | 3.50 | 2.78 | 8.00 |
| linear | 18 | 4.61 | 3.39 | 2.61 | 1.44 | 0.00 |
| linear-explore | 18 | 7.44 | 0.56 | 3.33 | 2.67 | 8.00 |
| linear-minimal | 18 | 5.39 | 2.61 | 2.39 | 2.56 | 0.00 |

## Search behaviour (in-loop timings, not neutral)

Best-so-far speedup after k calls, mean over runs and tasks (the baseline counts as found at k = 0).

| strategy | k=1 | k=2 | k=3 | k=4 | k=5 | k=6 | k=7 | k=8 |
|---|---:|---:|---:|---:|---:|---:|---:|---:|
| independent | x1.161 | x1.519 | x1.538 | x1.605 | x1.676 | x1.677 | x1.682 | x1.685 |
| archive | x1.578 | x1.751 | x1.803 | x1.862 | x1.874 | x1.910 | x1.916 | x1.931 |
| archive-explore | x1.411 | x1.573 | x1.615 | x1.638 | x1.706 | x1.722 | x1.749 | x1.761 |
| linear | x1.324 | x1.481 | x1.617 | x1.637 | x1.737 | x1.742 | x1.782 | x1.782 |
| linear-explore | x1.119 | x1.482 | x1.733 | x1.773 | x1.869 | x1.882 | x1.898 | x1.900 |
| linear-minimal | x1.411 | x1.706 | x1.775 | x1.779 | x1.781 | x1.783 | x1.783 | x1.783 |

| strategy | runs | correct attempts | duplicates | improved on baseline | failures (all runs) |
|---|---:|---:|---:|---:|---|
| independent | 18 | 81 / 144 | 1 | 14 / 18 | chehab_error 41, build_error 3, wrong_output 6, predicted_slower 12 |
| archive | 18 | 107 / 144 | 26 | 15 / 18 | chehab_error 7, build_error 1, wrong_output 1, predicted_slower 2 |
| archive-explore | 18 | 100 / 144 | 6 | 14 / 18 | compile_error 1, chehab_error 24, wrong_output 4, predicted_slower 9 |
| linear | 18 | 71 / 144 | 61 | 15 / 18 | chehab_error 10, wrong_output 2 |
| linear-explore | 18 | 102 / 144 | 10 | 15 / 18 | compile_error 1, chehab_error 25, wrong_output 1, predicted_slower 5 |
| linear-minimal | 18 | 88 / 144 | 47 | 14 / 18 | chehab_error 8, wrong_output 1 |

## Sanity check: call 1

Call 1 is the same prompt under these strategies, so they should agree up to sampling noise.

| strategy | runs | mean call-1 speedup | correct at call 1 |
|---|---:|---:|---:|
| independent | 18 | x1.161 | 11 / 18 |
| linear | 18 | x1.324 | 13 / 18 |
| linear-minimal | 18 | x1.411 | 14 / 18 |
