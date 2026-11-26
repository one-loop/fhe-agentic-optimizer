# Feedback ablation

Speedups of each run's final kernel from neutral re-scoring. Reference: `independent` (no feedback to the model; the loop still verifies every kernel and keeps the fastest correct one). 8 model calls per run. Models: claude-haiku-5-5.

## Final kernels (mean speedup over runs; runs in brackets)

| task | independent | archive | archive-explore | linear | linear-explore | linear-minimal |
|---|---:|---:|---:|---:|---:|---:|
| chebyshev_elu_d7 | x1.508 (2; 1.00-2.02) | x2.294 (2; 2.01-2.58) | x2.298 (2; 2.02-2.58) | x2.010 (2; 2.01-2.01) | x2.221 (2; 2.01-2.43) | x1.486 (2; 1.00-1.97) |
| chebyshev_gelu_d7 | x2.903 (2; 2.90-2.91) | x2.923 (2; 2.91-2.94) | x2.913 (2; 2.88-2.94) | x2.904 (2; 2.89-2.92) | x2.897 (2; 2.89-2.91) | x2.897 (2; 2.89-2.91) |
| chebyshev_sigmoid_d7 | x1.905 (2; 1.80-2.01) | x2.450 (2; 2.32-2.58) | x1.665 (2; 1.00-2.33) | x2.052 (2; 1.64-2.46) | x2.512 (2; 2.51-2.52) | x2.577 (2; 2.44-2.71) |
| coyote_tree_100_50_5_1 | x1.000 (2; 1.00-1.00) | x1.000 (2; 1.00-1.00) | x1.000 (2; 1.00-1.00) | x1.000 (2; 1.00-1.00) | x1.000 (2; 1.00-1.00) | x1.000 (2; 1.00-1.00) |
| extract_sparse | x1.281 (2; 1.28-1.28) | x1.293 (2; 1.29-1.30) | x1.292 (2; 1.29-1.30) | x1.288 (2; 1.28-1.30) | x1.285 (2; 1.29-1.29) | x1.270 (2; 1.27-1.27) |
| matmul_2x2 | x1.337 (2; 1.27-1.40) | x1.563 (2; 1.56-1.57) | x1.442 (2; 1.33-1.56) | x1.540 (2; 1.54-1.54) | x1.548 (2; 1.54-1.55) | x1.384 (2; 1.38-1.39) |
| **geometric mean (6 tasks)** | **x1.558** | **x1.793** | **x1.658** | **x1.696** | **x1.783** | **x1.641** |

## Primary comparisons (declared before the runs; Holm-corrected)

Ratio > 1: the first strategy's final kernels were faster.

| strategy | against | tasks | geo. mean ratio | 95% bootstrap interval | paired runs | sign-flip p | Holm p | runs better / worse / tied (2%) |
|---|---|---:|---:|---|---:|---:|---:|---|
| linear | independent | 6 | x1.089 | x0.999 - x1.203 | 12 | 0.1953 | 0.5684 | 4 / 1 / 7 |
| linear | linear-minimal | 6 | x1.033 | x0.949 - x1.139 | 12 | 0.5020 | 0.5684 | 4 / 1 / 7 |
| linear-explore | linear | 6 | x1.052 | x1.003 - x1.108 | 12 | 0.1309 | 0.5234 | 3 / 0 / 9 |
| archive | linear | 6 | x1.057 | x1.003 - x1.119 | 12 | 0.0098 | 0.0488 | 4 / 0 / 8 |
| archive-explore | archive | 6 | x0.925 | x0.824 - x1.009 | 12 | 0.1895 | 0.5684 | 0 / 2 / 10 |

- linear vs independent: does feedback to the model help?
- linear vs linear-minimal: does the performance content of the feedback help (beyond correctness)?
- linear-explore vs linear: does steering toward new approaches help the dialogue?
- archive vs linear: does search on top of feedback help?
- archive-explore vs archive: does steering toward new approaches help the search?

## Every other strategy vs `independent` (secondary, uncorrected)

| strategy | against | tasks | geo. mean ratio | 95% bootstrap interval | paired runs | sign-flip p | runs better / worse / tied (2%) |
|---|---|---:|---:|---|---:|---:|---|
| archive | independent | 6 | x1.151 | x1.071 - x1.258 | 12 | 0.0039 | 6 / 0 / 6 |
| archive-explore | independent | 6 | x1.064 | x0.928 - x1.207 | 12 | 0.5039 | 4 / 2 / 6 |
| linear-explore | independent | 6 | x1.145 | x1.072 - x1.248 | 12 | 0.0293 | 5 / 0 / 7 |
| linear-minimal | independent | 6 | x1.053 | x0.939 - x1.183 | 12 | 0.6250 | 4 / 1 / 7 |

Treat a ratio whose interval includes x1.000 as "no detectable effect", not as "no effect".

## Diversity of the proposals (per run, mean over runs)

distinct kernels: proposals that were not identical to an earlier kernel; distinct structures: new operation-count fingerprints (ct x ct, ct x pt, rotations, levels, alignments, mod switches), the baseline's excluded; structure repeats: evaluated proposals with a fingerprint already seen; techniques: distinct `Technique:` lines (explore arms only).

| strategy | runs | distinct kernels | duplicates | distinct structures | structure repeats | distinct techniques |
|---|---:|---:|---:|---:|---:|---:|
| independent | 12 | 7.92 | 0.08 | 2.83 | 2.83 | 0.00 |
| archive | 12 | 6.67 | 1.33 | 2.67 | 3.67 | 0.00 |
| archive-explore | 12 | 7.75 | 0.25 | 3.58 | 3.00 | 8.00 |
| linear | 12 | 4.83 | 3.17 | 2.75 | 1.67 | 0.00 |
| linear-explore | 12 | 7.67 | 0.33 | 3.42 | 3.00 | 8.00 |
| linear-minimal | 12 | 5.17 | 2.83 | 2.17 | 2.50 | 0.00 |

## Search behaviour (in-loop timings, not neutral)

Best-so-far speedup after k calls, mean over runs and tasks (the baseline counts as found at k = 0).

| strategy | k=1 | k=2 | k=3 | k=4 | k=5 | k=6 | k=7 | k=8 |
|---|---:|---:|---:|---:|---:|---:|---:|---:|
| independent | x1.164 | x1.634 | x1.637 | x1.637 | x1.655 | x1.655 | x1.662 | x1.662 |
| archive | x1.582 | x1.813 | x1.844 | x1.845 | x1.856 | x1.910 | x1.920 | x1.941 |
| archive-explore | x1.366 | x1.573 | x1.604 | x1.640 | x1.741 | x1.758 | x1.766 | x1.776 |
| linear | x1.345 | x1.573 | x1.710 | x1.729 | x1.772 | x1.772 | x1.809 | x1.809 |
| linear-explore | x1.128 | x1.487 | x1.727 | x1.769 | x1.901 | x1.920 | x1.922 | x1.922 |
| linear-minimal | x1.333 | x1.678 | x1.768 | x1.774 | x1.776 | x1.779 | x1.779 | x1.779 |

| strategy | runs | correct attempts | duplicates | improved on baseline | failures (all runs) |
|---|---:|---:|---:|---:|---|
| independent | 12 | 52 / 96 | 1 | 9 / 12 | chehab_error 27, build_error 3, wrong_output 4, predicted_slower 9 |
| archive | 12 | 73 / 96 | 16 | 10 / 12 | chehab_error 4, build_error 1, predicted_slower 2 |
| archive-explore | 12 | 67 / 96 | 3 | 9 / 12 | chehab_error 14, wrong_output 4, predicted_slower 8 |
| linear | 12 | 51 / 96 | 38 | 10 / 12 | chehab_error 5, wrong_output 2 |
| linear-explore | 12 | 73 / 96 | 4 | 10 / 12 | compile_error 1, chehab_error 14, predicted_slower 4 |
| linear-minimal | 12 | 55 / 96 | 34 | 9 / 12 | chehab_error 6, wrong_output 1 |

## Sanity check: call 1

Call 1 is the same prompt under these strategies, so they should agree up to sampling noise.

| strategy | runs | mean call-1 speedup | correct at call 1 |
|---|---:|---:|---:|
| independent | 12 | x1.164 | 7 / 12 |
| linear | 12 | x1.345 | 9 / 12 |
| linear-minimal | 12 | x1.333 | 9 / 12 |
