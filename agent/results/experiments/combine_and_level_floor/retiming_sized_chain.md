# Old best kernels re-timed under the sized modulus chain (2026-11-08)

`agent/remeasure_runs.py` on the `sized-chain-and-history` branch: each run's best kernel rebuilt with the chain sized to its own levels (baseline likewise), timed against the baseline in 7 alternating batches. No model calls. Data: `../../remeasured_sized_chain.csv` (11 Chebyshev tasks) and `../../remeasured_sized_chain_part2.csv` (embedding, extract_sparse, table_mult). Old = each run's own reported speedup under the fixed 10-level chain.

| task | Opus (1 Nov) old → sized | Sonnet v2 first old → sized | Sonnet v2 repeat 2 old → sized |
|---|---|---|---|
| chebyshev_elu_d7 | 1.08 → 2.00 | 1.40 → 1.49 | 1.42 → 1.50 |
| chebyshev_gelu_d7 | 1.41 → 2.92 | 1.45 → 2.27 | 1.57 → 2.60 |
| chebyshev_hardshrink_d7 | 1.62 → 2.60 | 1.22 → 2.29 | 1.22 → 1.80 |
| chebyshev_mish_d7 | 1.12 → 2.25 | 1.46 → 1.57 | 1.91 (level burn) → 1.29 |
| chebyshev_relu_d2 | 1.29 → 1.38 | 1.31 → 1.41 | 1.25 → 1.40 |
| chebyshev_relu_d7 | 1.41 → 2.99 | 1.40 → 2.96 | 1.41 → 2.97 |
| chebyshev_selu_d7 | 1.17 → 2.19 | 1.20 → 1.80 | 1.08 → 2.00 |
| chebyshev_sigmoid_d5 | 1.54 → 2.14 | 1.21 → 2.02 | 1.50 → 1.94 |
| chebyshev_sigmoid_d7 | 1.60 → 2.59 | 1.27 → 1.92 | 1.32 → 2.02 |
| chebyshev_silu_d7 | 1.58 → 2.66 | 1.46 → 2.35 | 1.45 → 2.87 |
| chebyshev_softplus_d7 | 1.52 → 3.17 | 1.52 → 2.35 | 1.43 → 2.96 |
| embedding | 5.69 → **0.61** (slower; 0/7 wins) | 1.00 → 1.00 | 1.00 → 1.00 |
| extract_sparse | 1.38 → 1.25 | 1.40 → 1.26 | 1.40 → 1.23 |
| table_mult | 1.07 → **0.71** (slower; 0/7 wins) | 1.00 → 1.00 | 1.00 → 1.00 |
| geomean, these 14 (slower counted as 1.00) | **2.014** | 1.747 | 1.775 |

Every faster result won 7/7 batches.

- Both level-burning results reverse: Opus `embedding` (8 levels added through a zero-valued term) and Opus `table_mult` consume levels the baseline does not need and become slower than the baseline; repeat 2's `mish` drops from 1.91 to 1.29.
- Depth reductions now count: most Chebyshev kernels that saved levels roughly double their speedup.
- Opus `embedding` also failed its 1e-4 tolerance once during the first re-timing attempt (error 1.10e-4 against 1.8e-5 in its original run, with an identical 12-prime chain): it is near the tolerance whatever the chain, so a single correctness pass is not enough for it.
- Ranking is unchanged: Opus > Sonnet, but Opus's two largest old results were the two level-burning kernels.
