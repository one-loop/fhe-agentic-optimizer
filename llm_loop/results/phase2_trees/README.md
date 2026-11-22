# Random expression trees (BFV): Haiku, Sonnet, Opus

These are 4 Coyote random expression trees from `benchmarks/polynomials_coyote/polynomial_trees/`. Each tree is a `+`/`*` expression whose leaves are distinct ciphertext inputs.

- **Setup:** 1 run per model, 8 calls, execution feedback, `archive` strategy, `arithmetic` hint.
- **Re-timing:** one joint neutral re-timing (`python3 -m llm_loop.rescore --by-folder`). Each speedup is the baseline eval median divided by the kernel eval median, both from this one session.
- **Labels:** the "(3 runs each)" heading in `phase2_pilot.md` is fixed text in `rescore.py`. Each cell here is 1 run.

## Result: no speedup on any tree

| tree | inputs | ct×ct mul | mul-depth (baseline) | baseline eval (ms) | Haiku | Sonnet | Opus |
|---|---:|---:|---:|---:|---:|---:|---:|
| tree_50-50_5_1 | 8 | 2 | 2 | 53.3 | x1.000 (baseline kept) | x1.000 (baseline kept) | x1.000 (baseline kept) |
| tree_100-50_5_1 | 32 | 12 | 3 | 317.8 | x1.000 (baseline kept) | x1.000 (baseline kept) | x1.000 (baseline kept) |
| tree_50-50_10_1 | 34 | 17 | 5 | 449.4 | x1.000 (baseline kept) | x1.000 (baseline kept) | x1.000 (baseline kept) |
| tree_100-50_10_1 | 1024 | 512 | 9 | 13,880 | x1.000 (baseline kept) | x1.0004 (mul-depth 8) | x1.000 (baseline kept) |

Geometric mean across the 4 trees: x1.000 for every model.

## Attempts

- **Correctness:** every attempt that produced code was exact. There were 66 evaluated attempts and 0 wrong outputs. The other 30 calls were duplicates or returned no code.
- **Multiplications:** no attempt changed the number of ct×ct multiplications (2, 12, 17 and 512).
- **Rebalancing:** the models rebalanced the trees and lowered the multiplicative depth:
  - from 5 to 4 on tree_50-50_10_1 (Sonnet and Opus);
  - from 9 to 8 on tree_100-50_10_1 (Sonnet and Opus).
  
  Eval time stayed the same, within run-to-run noise. The loop timings for the 1024-input tree were 13.6 to 14.2 s against a baseline of 13.9 s. The selector kept the baseline whenever a lower depth didn't run faster. The one exception is Sonnet's 1024-input kernel, which was re-timed at x1.0004, which is noise.

## Why there is nothing to gain here

1. **The multiplication count is already minimal.** Each tree is a read-once formula: every input appears exactly once, so there are no shared subexpressions and no products to factor out. Sonnet said so itself on the 1024-input tree.
2. **Depth doesn't change runtime in this setup.** The BFV runtime uses fixed parameters (N = 16384) and does no modulus switching, so every multiplication costs the same at any depth. Cost is roughly the number of multiplications plus relinearizations, and those are fixed.
3. **Not packed.** Each input is a separate one-slot ciphertext. These trees were built to test vectorization, where scalar operations are packed into SIMD slots. This pipeline compiles them through CHEHAB's scalar path without the vectorizer, and the I/O format gives the model no way to pack inputs.

So this experiment shows the loop stays correct on unstructured code: 0 wrong outputs, and it never selected a slower kernel. It does not show any speedup. To find a speedup on trees, the next step would be one of these:
- a setup where depth matters, such as CKKS or BFV with a modulus chain that shrinks as depth drops;
- running the trees through the vectorizer, so the model can change the packing.

## Files

- `runs.csv`, `rescore.csv`, `rescore.meta.json`, `phase2_pilot.md`: output of `rescore.py`.
- Raw runs (git-ignored): `llm_loop/runs/phase2-trees-{haiku,sonnet,opus}/archive/<tree>/run1/`.
