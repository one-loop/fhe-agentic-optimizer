**Idea:** Expand the Chebyshev sum into a polynomial in z = x/4, using T2 = 2z²−1, T4 = 8z⁴−8z²+1 and T6 = 32z⁶−48z⁴+18z²−1. Only three ciphertext multiplications are needed (z², z⁴ = (z²)², z⁶ = z⁴·z²), instead of five. Each coefficient is applied with a plaintext multiply, and integer factors like 8, 18, 32 and 48 come from ciphertext doublings and additions, which are cheap. Constant terms (c0 − c2 + c4 − c6) are added as plaintexts. Multiplicative depth is 5 (z⁶·c6), down from 7.

```cpp
// Chebyshev softplus degree 7 on [-4,4], evaluated as a power-basis polynomial in z = x/4.
// y = c0 + c1 z + z^2 (2c2 - 8c4 + 18c6) + z^4 (8c4 - 48c6) + 32 c6 z^6 - c2 + c4 - c6

Ciphertext dbl_n(Ciphertext c, int k)
{
  for (int i = 0; i < k; ++i)
    c = c + c;
  return c;
}

void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Ciphertext z = x * alpha;

  // Powers of z: 3 ciphertext multiplications total.
  Ciphertext z2 = z * z;
  Ciphertext z4 = z2 * z2;
  Ciphertext z6 = z4 * z2;

  // Linear term.
  Plaintext c1("c1");
  Ciphertext r = z * c1;

  // z^2 terms: 2c2 z^2 - 8c4 z^2 + 18c6 z^2
  Plaintext c2("c2");
  Ciphertext a = z2 * c2;
  Ciphertext a2 = dbl_n(a, 1);

  Plaintext c4("c4");
  Ciphertext b = z2 * c4;
  Ciphertext b8 = dbl_n(b, 3);

  Plaintext c6("c6");
  Ciphertext cc = z2 * c6;
  Ciphertext c18 = dbl_n(cc, 4) + dbl_n(cc, 1);

  // z^4 terms: 8c4 z^4 - 48c6 z^4
  Ciphertext e = z4 * c4;
  Ciphertext e8 = dbl_n(e, 3);

  Ciphertext f = z4 * c6;
  Ciphertext f48 = dbl_n(f, 5) + dbl_n(f, 4);

  // z^6 term: 32 c6 z^6
  Ciphertext g = z6 * c6;
  Ciphertext g32 = dbl_n(g, 5);

  Ciphertext result = r + a2;
  result = result - b8;
  result = result + c18;
  result = result + e8;
  result = result - f48;
  result = result + g32;

  // Constant terms: c0 - c2 + c4 - c6
  Plaintext c0("c0");
  result = result + c0;
  result = result - c2;
  result = result + c4;
  result = result - c6;

  result.set_output("y");
}
```