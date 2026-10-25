# Run comparison: API Haiku vs Sonnet CLI v1 vs Sonnet CLI v2 (2026-10-24)

| run | model | loop | runs |
|---|---|---|---|
| api_haiku | claude-haiku-5-5, effort medium | original (API key) | `agent/runs/*/20261013-*`, summary `summary.md` |
| sonnet_v1 | claude-sonnet-5-5, effort medium | original, through `claude -p` | `agent/runs/*/20261024-1[1-3]*`, summary `cli/summary.md` |
| sonnet_v2 | claude-sonnet-5-5, effort medium | fingerprint + interleaved selection, source-mapped scale errors, verdict notes, best-circuit goal | `agent/runs/*/20261024-1[5-9]*`, summary `cli_v2/summary.md` |

All runs: 26 tasks, 8 attempts each. One run per configuration, so per-task differences include model sampling variance.

## Speedups, remeasured under identical conditions

Every best kernel whose circuit differs from the baseline was rebuilt and timed against the baseline in 7 alternating batches of 10 runs, in one session (`agent/remeasure_runs.py`, raw data in `remeasured.csv`). "same" means the best kernel has the baseline's circuit, so x1.00. `*` means not confirmed (median gain below 3% or fewer than 6 of 7 batch wins).

| task | api_haiku | sonnet_v1 | sonnet_v2 |
|---|---:|---:|---:|
| chebyshev_elu_d7 | 1.07 | same | **1.42** |
| chebyshev_gelu_d7 | 1.27 | 1.42 | **1.44** |
| chebyshev_hardshrink_d7 | 1.12 | 1.11 | **1.20** |
| chebyshev_mish_d7 | 1.06 | 1.05* | **1.40** |
| chebyshev_relu_d2 | 1.27 | **1.28** | 1.21 |
| chebyshev_relu_d7 | 1.31 | **1.40** | 1.39 |
| chebyshev_selu_d7 | 1.04* | **1.42** | 1.19 |
| chebyshev_sigmoid_d5 | 1.13 | 1.13 | **1.21** |
| chebyshev_sigmoid_d7 | 1.13 | **1.45** | 1.26 |
| chebyshev_silu_d7 | 1.41 | **1.70*** (5/7 wins) | 1.46 |
| chebyshev_softplus_d7 | 1.37 | 1.40 | **1.47** |
| extract_sparse | 1.37 | 1.39 | 1.40 |
| embedding | 1.01* | 0.98* | same |
| quad | 1.01* | same | same |
| relu_composite | same | 1.01* | same |
| 11 other tasks | same | same | same |
| **geometric mean, all 26** | 1.091 | 1.127 | **1.141** |
| **geometric mean, 11 Chebyshev** | 1.193 | 1.290 | **1.326** |

## Search behaviour

| | api_haiku | sonnet_v1 | sonnet_v2 |
|---|---:|---:|---:|
| correct attempts | 202/208 | 188/208 | 185/208 |
| distinct new correct circuits | 23 | 35 | **73** |
| correct attempts repeating the baseline circuit | 94 | 98 | 90 |
| correct attempts repeating an earlier circuit | 85 | 55 | **22** |
| CHEHAB errors (scale mismatch) | 4 (3) | 20 (18) | 21 (16) |
| next attempt correct after a failure | 5/6 | 9/20 | **16/21** |
| longest run of consecutive CHEHAB errors | 2 | 6 | **2** |
| tasks reported ≥3% faster but not confirmed by remeasurement | 8 | 6 (one, `silu`, is a real circuit change that missed the 6/7-wins rule) | **0** |
| wall time | 88 min | 95 min | 129 min |
| equivalent API cost (not billed for CLI runs) | $0.21 | $7.18 | $11.29 |

## Reading

- The selection rule did what it should: v2 reports no noise "speedups". Every v2 best that is not x1.00 was confirmed 7/7 in the independent remeasurement.
- The feedback changes mainly changed exploration: v2 found twice as many distinct circuits and stopped repeating itself. It recovered from CHEHAB errors far better (16/21 vs 9/20, no long error streaks), though it made a similar number of scale-mismatch errors in total.
- v2 found the Clenshaw-style rewrite (ct-pt 8→2) for `elu` and `mish`, where v1 found nothing or noise. It missed it for `selu`, where v1 found it. With one run each, per-task wins and losses such as `selu` and `sigmoid_d7` are within sampling variance; the aggregate (geomean 1.127 → 1.141, Chebyshev 1.290 → 1.326) is the more reliable signal, and still a single sample.
- About 90 correct attempts per run still reproduce the baseline circuit, mostly on the 15 tasks that are already optimal (add, multiply, batchnorm, extract, table, embedding). There the model has nothing to find, which is expected.
- v2 costs about 35% more wall time (remeasurement batches) and more tokens (longer feedback turns).
