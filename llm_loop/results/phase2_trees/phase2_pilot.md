# Phase-2 pilot: neutral re-scoring

Commit dabcfaa0d3 (dirty), Darwin arm64 arm, Apple clang version 17.0.0 (clang-1700.3.19.1), SEAL 4.1. Re-scored 2026-11-15T19:13:41.

Speedup = baseline eval-only median / kernel eval-only median, both from the same neutral session (1 warmup + 10 measured processes of ./main --repeats 10, interleaved). The final kernel of each run is the strategy's own selection; re-scoring never re-selects. E2E = CHEHAB execution_time_(ms) (parse + encode + encrypt + fhe()). Compile time: median of 3, reported only.

## Every run

| task | strategy | run | final | speedup | eval (ms) | e2e (ms) | FHE err | depth | xdepth | levels | ct x ct | ct x pt | align | rescale | mod sw | C (ms) | correct / 8 | dup | failures | out tok | think tok | search (s) |
|---|---|---:|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---|---:|---:|---:|
| tree_100-50_10_1 | haiku | 1 | #0 (baseline) | x1.000 | 13879.8 | 18536.3 | 0.00e+00 | 10 | 9 |  | 512 | 0 | 0 | 0 | 0 | 2.41 | 0 / 8 | 6 | no-code 2 | 66671 | 18176 | 272.2 |
| tree_100-50_10_1 | opus | 1 | #0 (baseline) | x1.000 | 13879.8 | 18536.3 | 0.00e+00 | 10 | 9 |  | 512 | 0 | 0 | 0 | 0 | 2.41 | 7 / 8 | 1 | - | 81981 | 9569 | 5271.9 |
| tree_100-50_10_1 | sonnet | 1 | #6 (k1) | x1.000 | 13874.7 | 18500.0 | 0.00e+00 | 16 | 8 |  | 512 | 0 | 0 | 0 | 0 | 2.78 | 3 / 8 | 2 | no-code 3 | 53539 | 8124 | 2256.6 |
| tree_100-50_5_1 | haiku | 1 | #0 (baseline) | x1.000 | 317.8 | 470.6 | 0.00e+00 | 5 | 3 |  | 12 | 0 | 0 | 0 | 0 | 0.38 | 7 / 8 | 1 | - | 37319 | 32367 | 437.2 |
| tree_100-50_5_1 | opus | 1 | #0 (baseline) | x1.000 | 317.8 | 470.6 | 0.00e+00 | 5 | 3 |  | 12 | 0 | 0 | 0 | 0 | 0.38 | 6 / 8 | 2 | - | 12425 | 7370 | 353.7 |
| tree_100-50_5_1 | sonnet | 1 | #0 (baseline) | x1.000 | 317.8 | 470.6 | 0.00e+00 | 5 | 3 |  | 12 | 0 | 0 | 0 | 0 | 0.38 | 5 / 8 | 3 | - | 10311 | 5836 | 264.1 |
| tree_50-50_10_1 | haiku | 1 | #0 (baseline) | x1.000 | 449.4 | 612.7 | 0.00e+00 | 8 | 5 |  | 17 | 0 | 0 | 0 | 0 | 0.54 | 6 / 8 | 2 | - | 37445 | 31448 | 423.4 |
| tree_50-50_10_1 | opus | 1 | #0 (baseline) | x1.000 | 449.4 | 612.7 | 0.00e+00 | 8 | 5 |  | 17 | 0 | 0 | 0 | 0 | 0.54 | 8 / 8 | 0 | - | 14723 | 9255 | 461.0 |
| tree_50-50_10_1 | sonnet | 1 | #0 (baseline) | x1.000 | 449.4 | 612.7 | 0.00e+00 | 8 | 5 |  | 17 | 0 | 0 | 0 | 0 | 0.54 | 6 / 8 | 2 | - | 12376 | 7727 | 313.2 |
| tree_50-50_5_1 | haiku | 1 | #0 (baseline) | x1.000 | 53.3 | 92.1 | 0.00e+00 | 4 | 2 |  | 2 | 0 | 0 | 0 | 0 | 0.36 | 6 / 8 | 2 | - | 17730 | 14456 | 260.7 |
| tree_50-50_5_1 | opus | 1 | #0 (baseline) | x1.000 | 53.3 | 92.1 | 0.00e+00 | 4 | 2 |  | 2 | 0 | 0 | 0 | 0 | 0.36 | 6 / 8 | 2 | - | 3825 | 1238 | 204.4 |
| tree_50-50_5_1 | sonnet | 1 | #0 (baseline) | x1.000 | 53.3 | 92.1 | 0.00e+00 | 4 | 2 |  | 2 | 0 | 0 | 0 | 0 | 0.36 | 6 / 8 | 2 | - | 5537 | 3190 | 193.6 |

## Per task and strategy (3 runs each)

| task | strategy | runs | speedup |
|---|---|---:|---|
| tree_100-50_10_1 | haiku | 1 | median x1.000, mean x1.000, min x1.000, max x1.000, sd 0.000 |
| tree_100-50_5_1 | haiku | 1 | median x1.000, mean x1.000, min x1.000, max x1.000, sd 0.000 |
| tree_50-50_10_1 | haiku | 1 | median x1.000, mean x1.000, min x1.000, max x1.000, sd 0.000 |
| tree_50-50_5_1 | haiku | 1 | median x1.000, mean x1.000, min x1.000, max x1.000, sd 0.000 |
| tree_100-50_10_1 | opus | 1 | median x1.000, mean x1.000, min x1.000, max x1.000, sd 0.000 |
| tree_100-50_5_1 | opus | 1 | median x1.000, mean x1.000, min x1.000, max x1.000, sd 0.000 |
| tree_50-50_10_1 | opus | 1 | median x1.000, mean x1.000, min x1.000, max x1.000, sd 0.000 |
| tree_50-50_5_1 | opus | 1 | median x1.000, mean x1.000, min x1.000, max x1.000, sd 0.000 |
| tree_100-50_10_1 | sonnet | 1 | median x1.000, mean x1.000, min x1.000, max x1.000, sd 0.000 |
| tree_100-50_5_1 | sonnet | 1 | median x1.000, mean x1.000, min x1.000, max x1.000, sd 0.000 |
| tree_50-50_10_1 | sonnet | 1 | median x1.000, mean x1.000, min x1.000, max x1.000, sd 0.000 |
| tree_50-50_5_1 | sonnet | 1 | median x1.000, mean x1.000, min x1.000, max x1.000, sd 0.000 |

## Across tasks (geometric mean of the per-task mean speedups)

| strategy | tasks | geometric mean |
|---|---:|---:|
| haiku | 4 | x1.000 |
| opus | 4 | x1.000 |
| sonnet | 4 | x1.000 |

## Approximation error (task property, separate from FHE error)

| task | max_fit_error (fitting grid) | approx. error, fixed points | approx. error, dense points |
|---|---:|---:|---:|
| tree_100-50_10_1 | - | - | - |
| tree_100-50_5_1 | - | - | - |
| tree_50-50_10_1 | - | - | - |
| tree_50-50_5_1 | - | - | - |

## Re-scored kernels

| task | kernel | used by | correct | eval (ms) | speedup |
|---|---|---|---|---:|---:|
| tree_100-50_10_1 | baseline | haiku/run1 opus/run1 | True | 13879.75 | 1.0 |
| tree_100-50_10_1 | k1 | sonnet/run1 | True | 13874.7 | 1.0 |
| tree_100-50_5_1 | baseline | haiku/run1 opus/run1 sonnet/run1 | True | 317.76 | 1.0 |
| tree_50-50_10_1 | baseline | haiku/run1 opus/run1 sonnet/run1 | True | 449.37 | 1.0 |
| tree_50-50_5_1 | baseline | haiku/run1 opus/run1 sonnet/run1 | True | 53.28 | 1.0 |
