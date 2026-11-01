Idea: cut a level and a plaintext multiply by squaring x directly and folding alpha into the plaintext multiplies.

y = c0 − c2 + c1·α·x + 2·c2·α²·x². With the available plaintexts, the factoring y = a·(c1 + 2c2·a) + (c0 − c2) with a = α·x needs only one ct-pt multiply for a.

Alternative with the ct-ct multiply early: let a = x*alpha (L1). Compute s = a*a (ct-ct, L2). Then y = c2·(s + s) + c1·a + c0 − c2. That needs two more pt multiplies, so it is worse.

Better: u = x*c2 (L1) and a = x*alpha (L1) in parallel, and the ct-ct product u*a gives c2·α·x² at L2. The missing α can be recovered by computing u from x*c2 where the factor is applied at the other side, but that costs a level again.

I'll instead keep the best structure and cut the mod switch. In attempt 1, z (L1) is multiplied with w (L2), so z needs a mod switch. I'll build the second operand from x independently, so that both operands sit at the same level:
- z1 = x*alpha, z = z1 (L1)
- w = (z*c2 doubled) + c1, at L2
- the other factor is z*one at L2. That is a pt multiply replacing the mod switch (~5 ms vs ~1 ms), so it is worse.

So the mod switch is cheap and the circuit is near minimal. One remaining option is to fold the doubling: replace v+v with a different placement. v2 = v+v is one ciphertext add (~0.5 ms), and moving it after the product costs the same. A real saving is to drop the doubling by using the product z*z: p = z*z (ct-ct, L2), then y = (p+p)*c2 + z*c1 + (c0 − c2). That has 2 pt multiplies too, but both are at L2 and L1, with z*c1 at L1 needing a mod switch. This is a different circuit, so I'll try it.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext c0("c0");
  Plaintext c1("c1");
  Plaintext c2("c2");
  Ciphertext z = x * alpha;
  Ciphertext p = z * z;
  Ciphertext p2 = p + p;
  Ciphertext q = p2 * c2;
  Ciphertext l = z * c1;
  Ciphertext r = q + l;
  r = r + c0;
  r = r - c2;
  r.set_output("y");
}
```