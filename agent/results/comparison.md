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

# Repeatability: four independent v2 runs (2026-10-25)

Three more v2 runs with identical settings (Sonnet 5.5, effort medium, 8 attempts, 10 timed runs), one after another on the same laptop, each task in a fresh CLI session with no information from other tasks or runs. Summaries: `cli_v2/` (first run) and `cli_v2_repeat{1,2,3}/`. All four runs' best kernels were then re-timed against the baseline in one session (7 alternating batches): `remeasured_v2_repeats.csv`.

| task | first | repeat 1 | repeat 2 | repeat 3 | mean | sd |
|---|---:|---:|---:|---:|---:|---:|
| chebyshev_elu_d7 | 1.40 | 1.07 | 1.42 | 1.08 | 1.25 | 0.19 |
| chebyshev_gelu_d7 | 1.45 | 1.41 | 1.57 | 1.43 | 1.46 | 0.07 |
| chebyshev_hardshrink_d7 | 1.22 | 1.28 | 1.22 | 1.20 | 1.23 | 0.03 |
| chebyshev_mish_d7 | 1.46 | 1.06 | 1.91† | 1.12 | 1.39 | 0.39 |
| chebyshev_relu_d2 | 1.31 | 1.28 | 1.25 | 1.29 | 1.28 | 0.02 |
| chebyshev_relu_d7 | 1.40 | 1.39 | 1.41 | 1.52 | 1.43 | 0.06 |
| chebyshev_selu_d7 | 1.20 | 1.03 | 1.08 | 1.43 | 1.18 | 0.18 |
| chebyshev_sigmoid_d5 | 1.21 | 1.52 | 1.50 | 1.50 | 1.44 | 0.15 |
| chebyshev_sigmoid_d7 | 1.27 | 1.40 | 1.32 | 1.10 | 1.27 | 0.13 |
| chebyshev_silu_d7 | 1.46 | 1.45 | 1.45 | 1.45 | 1.45 | 0.01 |
| chebyshev_softplus_d7 | 1.52 | 1.43 | 1.43 | 1.43 | 1.45 | 0.05 |
| extract_sparse | 1.40 | 1.37 | 1.40 | 1.40 | 1.39 | 0.01 |
| 14 other tasks | 1.00 | 1.00 | 1.00 | 1.00 | 1.00 | 0 |
| **geomean, 26 tasks** | 1.150 | 1.128 | 1.168 | 1.137 | | |
| **geomean, 11 Chebyshev** | 1.351 | 1.291 | 1.400 | 1.313 | | |

| | first | repeat 1 | repeat 2 | repeat 3 |
|---|---:|---:|---:|---:|
| tasks completed / failed | 26 / 0 | 26 / 0 | 26 / 0 | 26 / 0 |
| correct attempts | 185 | 186 | 188 | 188 |
| distinct new circuits | 73 | 81 | 74 | 82 |
| CHEHAB errors | 21 | 20 | 18 | 16 |
| remeasurements / not confirmed | 42 / 24 | 41 / 22 | 36 / 17 | 38 / 18 |
| wall time (min) | 129 | 132 | 131 | 136 |
| equivalent API cost | $11.29 | $10.51 | $11.85 | $11.15 |

Findings:

- **Consistent:** every run improved exactly the same 12 tasks and left the other 14 at x1.00; no run reported an unconfirmed speedup; every speedup was confirmed again in the joint re-timing, and each run's own speedups agree with the joint re-timing within 0.05 (a run-to-run timing difference of a few percent). Baseline times vary 2.3% (median) across runs. About half of all remeasurements were rejected as not confirmed, so the rule is active.
- **Stable tasks:** `silu`, `softplus`, `relu_d2`, `relu_d7`, `hardshrink`, `gelu` and `extract_sparse` give nearly the same result every run (sd ≤ 0.07).
- **Bimodal tasks:** `elu`, `mish` and `selu` land at about x1.42 exactly when the model finds the Clenshaw-style rewrite (same 8 levels, ct-pt 8→2), and at x1.04–x1.19 when it instead finds a shallower power-basis evaluation (5–6 levels, fewer ct-ct but 7–9 ct-pt). No run combined the two. `sigmoid_d5`/`d7` also vary between different level/multiplication trade-offs. This is model sampling variance, not measurement noise: the joint re-timing reproduces each run's number.
- **† Level-burning (benchmark artifact):** repeat 2's `mish` best starts with `x * one * one`, deliberately consuming 2 extra levels (10 instead of 8, the whole budget) so that later operations run lower in the modulus chain, where they are cheaper. It is correct and really faster under CHEHAB's fixed encryption parameters, but it would not survive parameters sized to each kernel's depth, so it should not count as an optimization. It occurred once in 104 task runs; the system prompt's line that "work at deeper levels" makes kernels faster invites it. Without it, repeat 2's `mish` best would be its attempt 3 (x1.42), and repeat 2's geomeans about 1.155 / 1.363.

**Follow-up (2026-10-25):** the level-burning exploit came from CHEHAB's fixed 10-level modulus chain. The chain is now sized to each kernel, which removes the exploit and credits depth reductions; see `experiments/combine_and_level_floor/`. All runs above used the fixed chain, so their speedups are not comparable with later runs.
