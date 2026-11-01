Idea: cut the plaintext multiply on the linear path by folding it into the factored form, and drop the extra level of the quadratic path.

y = z·(2·c2·z + c1) + c0 − c2, with z = αx. Expand the inner factor using x instead of z: 2·c2·z + c1 = 2·c2·α·x + c1. Build it from a level-1 product of x with the plaintext c2, then multiply with the level-1 ciphertext z. The product (c2·x)·z = c2·α·x². We need 2·c2·α²·x², so one more α is missing. Absorb it by building the first factor from z itself (as in attempt 1), which costs a level.

An alternative that removes a plaintext multiply: use (2·c2·z + c1) = c2·(z + z) + c1 and compute the ct-ct product on x-derived operands:
- z = x*alpha (L1)
- w = (z + z)*c2 + c1, which is the same as attempt 1.

A different circuit with fewer ops: take the ct-ct square of x first, since x² needs no plaintext multiply.
- s = square(x) (L1, ct-ct)
- y = s·(2c2α²) + x·(c1α) + (c0 − c2)

The coefficients 2c2α² and c1α need plaintext-plaintext arithmetic. We can still apply them sequentially to the ciphertext: s*c2 then *alpha then *alpha is 3 pt multiplies, which is too many. The cheaper way is to apply alpha before squaring (z = x*alpha, then square(z), L2), then multiply by c2 (L3), which equals attempt 3.

So I'll try a variant where the linear term is merged before the multiply, lowering the number of add/pt operations: r = z·(w) with w = (z+z)*c2 + c1, with the doubling taken on z first (cheaper at L1, with no change in multiplies). This is attempt 4's doubling placement but with the linear term merged, so it keeps 2 pt multiplies:

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
  Ciphertext v = zz * c2;
  Ciphertext w = v + c1;
  Ciphertext m = w * z;
  Ciphertext r = m + c0;
  r = r - c2;
  r.set_output("y");
}
```