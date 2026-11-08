The target is an even/odd split: y = c0 + c1 z + c2 T2 + c4 T4 + c6 T6.

Write the polynomial in u = z²:
- T2 = 2u − 1
- T4 = 8u² − 8u + 1
- T6 = 32u³ − 48u² + 18u − 1

So y = a0 + a1 z + a2 u + a4 u² + a6 u³, where the a's are combinations of c's. Computing the a's would need plaintext-plaintext arithmetic, which is not allowed. Instead I keep the coefficient multiplications on ciphertexts, in a different grouping.

Define the polynomial in z² as P(u) = (c0 − c2 + c4 − c6) + (2c2 − 8c4 + 18c6)u + (8c4 − 48c6)u² + 32c6 u³. That would still need plaintext arithmetic to form the coefficients, so I avoid it and use the Chebyshev structure directly:

- z = x·alpha (1 pt mul, depth 1).
- u = z² (depth 2).
- T2 = 2u − 1 (no mult).
- T4 = 2·T2² − 1 (one square, depth 3).
- T6 = 2·T2·T4 − T2 (one mult, depth 4).
- Terms: c1·z (z is at depth 1, so the product is depth 2), c2·T2 (depth 3), c4·T4 (depth 4), c6·T6 (depth 5).

This needs 3 ct-ct multiplications (u, T4, T6) plus 1 square for T2², so ct-ct = 4 including squares. The pt multiplications are alpha plus 4 coefficient multiplications, 5 in total, the same as the baseline. The ct-ct count drops from 5 to 4, and depth drops from 7 to 5.

To save one more, I fold alpha into u: u = (x·alpha)², which still takes the alpha multiplication first. Alternatively, z² = x²·alpha², but alpha² would be plaintext-plaintext arithmetic, so I keep it as is.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext one("one");
  Plaintext c0("c0");
  Plaintext c1("c1");
  Plaintext c2("c2");
  Plaintext c4("c4");
  Plaintext c6("c6");
  Ciphertext z = x * alpha;
  Ciphertext u = square(z);
  Ciphertext t2 = u + u - one;
  Ciphertext s = square(t2);
  Ciphertext t4 = s + s - one;
  Ciphertext m = t2 * t4;
  Ciphertext t6 = m + m - t2;
  Ciphertext r = z * c1;
  r = r + t2 * c2;
  r = r + t4 * c4;
  r = r + t6 * c6;
  r = r + c0;
  r.set_output("y");
}
```