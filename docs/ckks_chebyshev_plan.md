# CKKS Chebyshev (degree-5 Sigmoid) through CHEHAB: implementation plan

Status: plan for the next milestone. Builds on the Quad and BatchNorm CKKS
slices (`docs/ckks_quad_design.md`).

Target, same semantics as the cleaned direct-SEAL reference
(`benchmarks/dl_operators_ckks`):

```
z  = alpha*x + beta          (beta skipped when |beta| <= 1e-14, as in the reference)
T1 = z
T2 = 2*z*z - 1
Tk = 2*z*T{k-1} - T{k-2}
P  = sum_k c_k * T_k         (c_k skipped when |c_k| <= 1e-14)
```

## What the current CKKS path lacks

1. **Level mismatches.** `check_ckks_levels` rejects binary operations whose
   ciphertext operands are at different levels. The recurrence and the
   accumulation need them: `z * T{k-1}`, `Tk - T{k-2}`, `acc + c_k*T_k`.
2. **Scale mismatches.** After rescaling, ciphertexts carry different exact
   scales (`s_a*s_b/q_l`, `s*2^40/q_l`). SEAL's add/sub require (near-)equal
   scales, and forcing them equal reintroduces the bias fixed in the
   reference module.
3. **Constants 2, -1, c0.** CKKS functions reject IR constants.

## Constants without new IR

- `2*m` is written `m + m`: no constant, no level. A plaintext multiply by 2
  would cost a rescale per recurrence step, breaking parity with the reference.
- `-1` is `- one`, where `one` is a named plaintext input (value 1.0) lowered to
  `sub_plain` at the ciphertext's level and scale.
- `c0..c5`, `alpha`, `beta` are named plaintext inputs. The DSL program reads the
  committed coefficient set (`chebyshev_coeffs.hpp`) only to decide which terms
  exist, like the reference.

No floating-point IR constants are added. The active simplification rules
(`x+0`, `x-0`, `x-x`, `x*0`) do not rewrite `m + m`.

## CKKS-only alignment pass

New pass `align_ckks_operands`, run by `gen_he_code` after `insert_rescale` and
before `check_ckks_levels`, CKKS functions only. In topological order it tracks
for every ciphertext term:

- its level (rescales and mod switches consumed);
- its scale, **symbolically** as `Delta^a * prod_l q_l^(-b_l)` (`Delta` = nominal
  scale, `q_l` = prime dropped at level `l`). Rules:
  - fresh input: `Delta`
  - ct-pt mul: `*Delta`
  - ct-ct mul: product of the two scales
  - rescale at level `l`: `/q_l`
  - mod_switch, relin, add/sub, add_plain/sub_plain: unchanged

Symbolic tracking decides scale equality exactly at compile time, without SEAL.

For each binary ciphertext-ciphertext operation it rebuilds the term with
aligned operands:

| op | levels differ | scales differ |
|---|---|---|
| mul | `mod_switch` the higher-level operand down | irrelevant |
| add/sub, equal scales | `mod_switch` the higher-level operand down | - |
| add/sub, different scales, higher operand has a spare level | `match_scale(hi, lo)` then `mod_switch` to `lo`'s level | |
| add/sub, same level, different scales | fail clearly (would need an extra level) | |

It never raises a ciphertext's level.

`match_scale(a, ref)` is a new CKKS-only binary IR op: multiply `a` by 1 encoded
at `a`'s level with scale `ref.scale() * q_l / a.scale()`, then rescale. The
result lands exactly on `ref`'s scale (same technique as the reference's
`align_to`) and consumes one level. `ref` is an operand only to make the data
dependency explicit.

## Code generation (CKKS only)

- `fhe()` gains `const SEALContext &context` (`q_l` for `match_scale`); the
  generated main passes it.
- Generated source gets two static helpers: `ckks_landing_scale(context, ct,
  target)` and `ckks_snap_scale(ct, scale)`. The latter assigns a scale only
  when it agrees to 1e-9 relative (floating-point rounding) and throws otherwise.
- `match_scale` is emitted as encode at `parms_id()` + `multiply_plain` +
  `rescale_to_next` + snap to the reference scale.
- Before each ct-ct add/sub of two distinct objects: snap the second operand's
  scale to the first's.
- `mod_switch` terms keep their existing `mod_switch_to_next` mapping, one per level.

## Expected levels (consumed)

`z` 1, `T2` 2, `T3` 3, full degree 5: 6 (chain 10 -> 4), the same as the reference.
Relative to the reference, accumulation adds two `match_scale` operations on
the running sum. The reference lands every `c_k*T_k` exactly on 2^40, so it
only mod-switches the sum.

## Validation

`benchmarks/chebyshev_ckks` with targets `z`, `t2`, `t3`, `p5`, on the reference's
9 points and on 256 seeded random points in [-4, 4].

- FHE numerical error: decrypted output vs the same plaintext computation,
  required < 1e-6.
- Approximation error, reported separately: plaintext polynomial vs true
  Sigmoid on the test points, plus `max_fit_error` from `chebyshev_coeffs.json`.

## Commits

1. `match_scale` opcode in every opcode dispatch site.
2. CKKS codegen: context parameter, helpers, `match_scale` emission, scale snap
   before ct-ct add/sub.
3. `align_ckks_operands` pass; `check_ckks_levels` accepts `match_scale`.
4. `chebyshev_ckks` benchmark, generator and end-to-end script.

The BFV golden check runs after every commit; Quad and BatchNorm CKKS after
every commit that touches CKKS code.
