The previous kernel uses 4 ciphertext-ciphertext multiplications, and those dominate the runtime. Every Chebyshev term is an even polynomial in w = z², so I compute only w = z², w² and w³ (3 ct-ct multiplications). Each T_k is then an integer-coefficient combination of these powers. Integer coefficients are built with doubling additions, which cost about 0.5 ms each, instead of multiplications. Each T_k still gets exactly one ciphertext-plaintext multiplication by its coefficient c_k.

- T2 = 2w − 1
- T4 = 8w² − 8w + 1
- T6 = 32w³ − 48w² + 18w − 1

```cpp
// Chebyshev relu degree 7 on [-4,4].
// w = z^2 is the only variable needed for the even terms; T2, T4, T6 are
// integer-coefficient combinations of w, w^2, w^3 built with doubling adds.
// Only 3 ciphertext-ciphertext multiplications (z*z, w*w, w2*w).

Ciphertext dbl(Ciphertext c, int n)
{
  for (int i = 0; i < n; ++i)
    c = c + c;
  return c;
}

void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext one("one");

  Ciphertext z = x * alpha;      // z = x/4 in [-1,1]
  Ciphertext w = z * z;          // w = z^2
  Ciphertext w2 = w * w;         // w^2 = z^4
  Ciphertext w3 = w2 * w;        // w^3 = z^6

  // T2 = 2w - 1
  Ciphertext T2 = w + w - one;

  // T4 = 8w^2 - 8w + 1
  Ciphertext T4 = dbl(w2, 3) - dbl(w, 3) + one;

  // T6 = 32w^3 - 48w^2 + 18w - 1,  48 = 16 + 32,  18 = 16 + 2
  Ciphertext T6 = dbl(w3, 5) - dbl(w2, 4) - dbl(w2, 5)
                + dbl(w, 4) + dbl(w, 1) - one;

  Plaintext c0("c0");
  Plaintext c1("c1");
  Plaintext c2("c2");
  Plaintext c4("c4");
  Plaintext c6("c6");

  Ciphertext term1 = z * c1;
  Ciphertext term2 = T2 * c2;
  Ciphertext term4 = T4 * c4;
  Ciphertext term6 = T6 * c6;

  Ciphertext result = term1 + term2;
  result = result + term4;
  result = result + term6;
  result = result + c0;
  result.set_output("y");
}
```