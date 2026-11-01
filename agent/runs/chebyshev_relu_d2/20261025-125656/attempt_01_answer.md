The output is y = c0 + c1·z + c2·(2z² − 1), with z = αx.

Rewrite it as y = c0 − c2 + c1·α·x + 2·c2·α²·x². Plaintext-plaintext arithmetic is not allowed, so I can't precompute the combined coefficients directly. I can still group the terms differently:

- Let a = x * alpha (one ct-pt multiply).
- Then y = a·(c1 + (2c2)·a) + (c0 − c2). Forming 2·c2 and c0 − c2 needs plaintext arithmetic, which is disallowed.
- Instead, compute u = a * c2, which is a ct-pt multiply. Then u + u = 2·c2·a, and w = (u + u) * a is a ct-ct multiply.
- That gives 2·c2·a² = w.
- The linear term is a * c1, a ct-pt multiply at the same level as u. Then w − c2 can't be formed directly, since c2 is a plaintext and the subtraction is of a plaintext constant. That is fine, because ct − pt is allowed.

Depth is a·c2 (level 1), times a (level 2), which is still 3 levels in total. The better route is to reduce the multiplications:

- Compute t = x * alpha, a single ct-pt multiply.
- Compute s = t*t, a ct-ct multiply, then s*c2 and t*c1, which are 2 ct-pt multiplies. That is the same as the baseline's count.
- Alternative: compute x*x first (ct-ct, level 1), parallel with nothing else. Then the result is (x²)·(2c2α²) + x·(c1α), which still needs the coefficients combined.
- Combine through the ciphertext instead: p = x * c1 (a ct-pt multiply) and q = x * c2 (a ct-pt multiply). Then y = α·(p + (q+q)·(αx)) − c2 + c0. That is again more multiplications.

A cheaper grouping is to factor out z: y = z·(c1 + 2c2·z) + (c0 − c2).
- Compute z = x*alpha (ct-pt, level 1).
- Compute v = z*c2 (ct-pt, level 2) and v2 = v + v.
- Compute m = (v2 + c1) * z. The plaintext add is cheap, and the ct-ct multiply lands at level 3.
- Add c0 − c2 as (m + c0) − c2.

This uses 2 ct-pt multiplies and 1 ct-ct multiply, against 3 ct-pt and 1 ct-ct in the baseline. Depth is still 3. It also avoids the `match_scale` and the extra plaintext subtraction with `one`, so the circuit is smaller.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext c0("c0");
  Plaintext c1("c1");
  Plaintext c2("c2");
  Ciphertext z = x * alpha;
  Ciphertext v = z * c2;
  Ciphertext v2 = v + v;
  Ciphertext w = v2 + c1;
  Ciphertext m = w * z;
  Ciphertext r = m + c0;
  r = r - c2;
  r.set_output("y");
}
```