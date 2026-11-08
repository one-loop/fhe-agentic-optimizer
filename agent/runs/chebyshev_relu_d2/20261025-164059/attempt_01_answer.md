Idea: rewrite the polynomial so it needs fewer multiplications.

y = c0 + c1·z + c2·(2z² − 1), with z = α·x.
= (c0 − c2) + c1·α·x + 2·c2·α²·x².

Let a = c1·α and b = c2·α². Then y = (c0 − c2) + x·(a + 2b·x). The constants have to be built from plaintexts, and plaintext-plaintext arithmetic isn't allowed, so I can't precompute them. Instead I restructure around the ciphertext:

- u = x·alpha (ct-pt mul, level 1).
- v = u·c2 (ct-pt mul, level 2), then v + v gives 2·c2·z.
- w = v + v + c1 (add plaintext, which needs the same scale; c1 is a plaintext, so the add is fine).
- w·z = 2·c2·z² + c1·z, which is a ct-ct mul at level 3.
- y = w·z − c2 + c0, but that needs plaintext-plaintext arithmetic (c0 − c2). Instead I compute (w·z + c0) − c2, which is two plaintext adds.

Cost: 2 ct-pt muls and 1 ct-ct mul, versus the baseline's 3 ct-pt muls and 1 ct-ct mul. The depth is 3, the same as before, but one multiplication fewer.

A scale concern: w is at level 2 (scale s³-ish after the two mults), and z is at level 1, so CHEHAB aligns z for free by mod switching. The product is then at level 3. The additions of c0 and c2 are plaintext adds on that result, which is fine.

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
  Ciphertext r = w * z;
  r = r + c0;
  r = r - c2;
  r.set_output("y");
}
```