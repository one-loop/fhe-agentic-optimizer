I'm stopping here and returning the best kernel, attempt 2 (133.96 ms). Attempts 3 and 4 were the same structure and came in at 133.97 and 134.15 ms, so the remaining differences are noise.

The kernel uses 3 ciphertext-ciphertext multiplications, which is the minimum for the degree-6 even part, and 5 ciphertext-plaintext multiplications, one per coefficient plus `alpha`. The one overhead I could not remove is the 8 `match_scale` insertions, which come from the plaintext constants (`one`, `c0`). Folding those constants into a single plaintext would require plaintext-plaintext arithmetic, which is not allowed.

Idea: build the even part from w = z², w² and w³ (three ciphertext multiplications), with each T_k written as an integer-coefficient combination of those powers. The integer coefficients are built with doubling additions, which are cheap, and each T_k gets one ciphertext-plaintext multiplication by its c_k.

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