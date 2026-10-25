Idea: reduce depth from 3 to 2. Writing y = c0 − c2 + c1·z + 2·c2·z², with z = α·x, I can avoid multiplying x by alpha first:

- a = x·c1 is not needed separately. Use p = x·alpha (ct-pt, L1) and q = x·(something) in parallel, so both sit at L1.
- Let u = x·alpha (L1) and v = u + u (free add). Then 2·c2·z² is still needed with c2 applied.
- Parallel: u = x·alpha and m = x·c2 (both L1, ct-pt). Then u·m = α·c2·x² (ct-ct, L2), which is c2·z²·4/… and is off by a factor of α. Compute u·u' instead, with u' = (x·alpha)·c2? That adds a level.

Option: m = x·c2 (L1) and n = u·u (L2) give c2 only after another multiply, so that fails. Take instead m = u + u (free, L1) and k = x·c2 (L1). Then m·k = 2·α·c2·x², which is wrong by a factor of α.

Fix: z·c2 needs α, so use the inputs differently. Let u = x·alpha (L1) and t = u·c2 (L2) as before. To keep depth 2, use the c1 term at L1 plus the square at L2: z² = u·u (ct-ct, L2), then multiplying by 2·c2 is a third level. This does not work. Depth 2 with all three multiplications (α, α, c2) plus x² needs at least 3 levels, unless a product of two L1 values carries all three factors. Each factor is a separate plaintext, so u1 = x·alpha and u2 = x·alpha·c2 cannot both be L1.

So I keep the depth at 3 and try to save the remaining cost: the c1 term is merged into the Horner form already. What remains is the mod_switch for alignment. Here the L2 term s = w + w + c1 meets z at L1, and CHEHAB aligns z. I'll keep the structure and resubmit with the additions reordered, which may remove the alignment.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext c0("c0");
  Plaintext c1("c1");
  Plaintext c2("c2");
  Ciphertext z = x * alpha;
  Ciphertext w = z * c2;
  Ciphertext s = w + w;
  s = s + c1;
  Ciphertext r = z * s;
  r = r + c0;
  r = r - c2;
  r.set_output("y");
}
```