# Head-to-head measurement of final kernels

Written by `experiments/rescore_teammates.py`, run 2026-11-01 from about 02:17 to 03:31 with
the lid open. On battery until about 03:19, then on AC (Low Power Mode off). `pmset -g log`
shows no sleep in that window. No model calls were made.

- **Compares kernels, not search methods.** The team kernels are one run each, from their own
  models, prompts and budgets, read from `shared/llm-feedback-loop` at 7bdb7c2. Mine are every
  no-hints final (3 or 5 runs per task, 1 for Quad and BatchNorm), Haiku 5.5, 8 calls per run.
- **Noise.** Speedups are medians of 10 processes. Most kernels have a max-min spread under 2%.
  A few have one slow process (worst 13%, ELU `api_haiku`), which the median absorbs. Of my kernels
  measured in earlier sessions too, the speedup moved by 0.08% at the median and 1.4% at worst.
  Differences of about 1% are not meaningful: Quad at x1.011 and BatchNorm2d at x0.996 are noise.
