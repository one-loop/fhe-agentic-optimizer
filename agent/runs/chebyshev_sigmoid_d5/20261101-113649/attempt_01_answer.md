The baseline builds T3 and T5 by recurrence, which costs 4 ciphertext-ciphertext multiplications and depth 6. This kernel expands the sum into the monomial basis instead. With z = alpha·x, T3 = 4z³ − 3z and T5 = 16z⁵ − 20z³ + 5z, so:

y = c0 + (c1 − 3c3 + 5c5)z + (4c3 − 20c5)z³ + 16c5·z⁵

Plaintext-plaintext arithmetic is not allowed, so the coefficients cannot be combined directly. I fold each coefficient into a ciphertext first:

- r = z·c1, q = z·c3 and p = z·c5, which are three scalar plaintext multiplications.
- The integer factors are built only from additions.
- The polynomial is then evaluated Horner-style in z²:

  y = c0 + (r − 3q + 5p) + z²·((4q − 20p) + z²·16p)

This needs 3 ciphertext-ciphertext multiplications instead of 4, and depth 4 instead of 6. The scales stay consistent:

- r, q and p all come from the same multiplication pattern, so they can be added at the same level.
- The other sums combine terms at different levels, which CHEHAB aligns for free.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext c0("c0");
  Plaintext c1("c1");
  Plaintext c3("c3");
  Plaintext c5("c5");

  Ciphertext z = x * alpha;
  Ciphertext z2 = z * z;

  Ciphertext r = z * c1;
  Ciphertext q = z * c3;
  Ciphertext p = z * c5;

  Ciphertext p2 = p + p;
  Ciphertext p4 = p2 + p2;
  Ciphertext p8 = p4 + p4;
  Ciphertext p16 = p8 + p8;
  Ciphertext p20 = p16 + p4;
  Ciphertext p5 = p4 + p;

  Ciphertext q2 = q + q;
  Ciphertext q3 = q2 + q;
  Ciphertext q4 = q2 + q2;

  // linear part: (c1 - 3c3 + 5c5) z
  Ciphertext lin = r - q3 + p5;
  // inner: (4c3 - 20c5) z + 16 c5 z^3
  Ciphertext inner = (q4 - p20) + z2 * p16;
  Ciphertext result = lin + z2 * inner;
  result = result + c0;
  result.set_output("y");
}
```