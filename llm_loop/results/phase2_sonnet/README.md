# Sonnet 5.5 in my loop: 9 Chebyshev tasks

Archive strategy, hints none, 3 runs per task, 8 calls per run. All 216 calls were served by
`claude-sonnet-5-5`.

These timings are from a clean re-timing on 2026-11-02, about 00:30 to 01:06. The first re-scoring
was noisy (median kernel spread 6.6%, from `mediaanalysisd`) and was replaced. No model calls were
made in the re-timing, and the search itself is unchanged. Median spread here is 0.5%; the Hardshrink
kernels are the worst, at about 6%.
