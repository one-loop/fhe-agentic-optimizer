Idea: remove one level and one plaintext multiply by making the quadratic term a product of two level-1 ciphertexts, with alpha applied to both factors via the plaintext multiplications.

y = c0 − c2 + c1·α·x + 2·c2·α²·x². Write it as a product of two level-1 ciphertexts:
- A = x*alpha (L1), which equals z.
- B = x*c2 (L1), which equals c2·x.
- A*B = c2·α·x², and we still need 2α·(A*B), so the factor α is missing and costs a level.

Use the linear term to absorb it instead: y = z·(2·c2·z + c1) + c0 − c2.
- Let u = x*c2 (L1) and w = u + u.
- Let p = z*z... that is again more multiplies.

Another way: a ciphertext multiplied by a plaintext only costs a level once. Take P = (x*alpha) and Q = x*(alpha*c2), but alpha*c2 is plaintext-plaintext arithmetic, which is not allowed.

Remaining cheap change: shape the ct-ct multiply so that both operands are at the same level, avoiding the mod switch, without extra plaintext multiplications. Use z2 = z + c1/(2c2), which isn't possible either. So take the factored form with the linear term kept separate and the squaring done as a `square` (one input, so no alignment needed):

y = (z² + z²)·c2 + z·c1 + c0 − c2. This was attempt 3 with the same circuit. A different one is to put c1 at the front, giving ct-pt multiplies on x directly in parallel at L1:
- z = x*alpha (L1)
- l = x*c1, and then l*alpha gives c1·α·x at L2, which is one more pt multiply.

I'll try a variant that sums before the final multiply: m = (z*c2 + z*c2 + c1)*z is the current best, so the only free change is to drop the doubling by using the identity 2·c2·z² = (c2·z)·(z+z):
- v = z*c2 (L2)
- zz = z + z (L1, a cheap add)
- m = v*zz (ct-ct)
- l = z*c1 (a pt multiply, L2)

That has 3 pt multiplies, so worse. Instead, fold c1 into the first factor: (v2 + c1) is already done. The alternative that removes the doubling add is to use `zz` in the product with `w = v + c1/2`, which isn't available.

So I'll submit the best kernel with the doubling moved onto the L1 operand, a different add placement and possibly a different circuit:

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext c0("c0");
  Plaintext c1("c1");
  Plaintext c2("c2");
  Ciphertext z = x * alpha;
  Ciphertext zz = z + z;
  Ciphertext v = z * c2;
  Ciphertext m = v * zz;
  Ciphertext l = z * c1;
  Ciphertext r = m + l;
  r = r + c0;
  r = r - c2;
  r.set_output("y");
}
```