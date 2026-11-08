Idea: cut one more multiplication level by avoiding the separate z = x·alpha step.

y = (c0 − c2) + c1·α·x + 2·c2·α²·x². Write it as c0 − c2 + z·(c1 + 2·c2·z), where z = α·x.

Alternative: the z² term can use x directly: 2·c2·z² = 2·(c2·z)·z, which is what attempt 1 did. To reduce plaintext multiplications, I can fold alpha in differently:
- p = x·c2 (ct-pt mul, level 1), and q = x·alpha (ct-pt mul, level 1), both in parallel, giving a shallower depth.
- p2 = p + p (2·c2·x).
- Then p2·(α²x)... this needs a ct-pt multiplication by α again.

Concretely: w = (p2·alpha) + c1 would require another ct-pt mul at level 2. Instead, w2 = p2 + c1/α is not possible without a plaintext division.

A different form: y = c0 − c2 + (c1·z) + 2·c2·z·z. Take m = z·z (ct-ct mul at level 2, with z at level 1), then m·c2 and z·c1 are ct-pt muls. That is the baseline structure, so it isn't an improvement.

Try parallelizing instead: q = x·alpha (level 1) and p = x·c2 (level 1) can run in parallel. Then r = (p + p)·q is a ct-ct mul at level 2 and gives 2·c2·α·x². This is off by a factor α, though. Using p' = q·c2, then p'·q has the right 2·c2·z², but the depth is 3 again. The depth is the same, but the work at deeper levels is cheaper. Here, instead I merge the linear term:
- t = (q·c2 + q·c2) + c1 is attempt 1's w.

So I keep attempt 1's structure but change where the additions happen to avoid the mod_switch: add c0 − c2 as a single combination. Since plaintext-plaintext arithmetic is not allowed, I fold c0 and c2 into the ciphertext side in a different order: (r − c2) + c0 is already what I have. The only change I make is to compute the c1 term separately, avoiding the extra mod_switch for z, by multiplying (w·z) where w comes from x directly:
- w = (x·c2)·alpha·2 + c1 does the same ct-pt multiplication count.

I don't see a strictly cheaper circuit, so I try a variant with the same number of operations but different rescale handling: square(z) instead of z·z can't replace the product because the linear term is needed, but I can use y = c2·2·(z + c1/(4c2))² + const, which needs division, so that's also not available.

Try the variant where the first multiplication is on x with c2, in parallel with z:

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext c0("c0");
  Plaintext c1("c1");
  Plaintext c2("c2");
  Ciphertext z = x * alpha;
  Ciphertext p = x * c2;
  Ciphertext p2 = p + p;
  Ciphertext s = z * z;
  Ciphertext a = z * c1;
  Ciphertext u = p2 * alpha;
  Ciphertext v = u * z;
  Ciphertext r = v + a;
  r = r + c0;
  r = r - c2;
  r.set_output("y");
}
```