The polynomial is odd apart from c0, so y = c0 + z·S, where S = c1 + c3·e3 + c5·e5 + c7·e7 and eₖ = Tₖ/z. Each eₖ comes from Chebyshev identities and uses only ciphertext products:

- t = T2 = 2u − 1, where u = z².
- e3 = 2t − 1.
- T4 = 2t² − 1.
- e5 = 2·T4 − e3.
- e7 = 2·e3·T4 − 1.

This needs 4 ciphertext-ciphertext products (the squares for u and T4, the product e3·T4, and the final z·S). The three plaintext products c3·e3, c5·e5 and c7·e7 each sit at a different level, so CHEHAB aligns them for free. That avoids the same-level scale mismatch from the last attempt. Depth is 6 instead of 8.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext one("one");
  Plaintext c0("c0");
  Plaintext c1("c1");
  Plaintext c3("c3");
  Plaintext c5("c5");
  Plaintext c7("c7");
  Ciphertext z = x * alpha;
  Ciphertext u = square(z);
  Ciphertext t = u + u - one;
  Ciphertext e3 = t + t - one;
  Ciphertext t4 = square(t);
  t4 = t4 + t4 - one;
  Ciphertext e5 = t4 + t4 - e3;
  Ciphertext p = e3 * t4;
  Ciphertext e7 = p + p - one;
  Ciphertext s = e3 * c3 + e5 * c5 + e7 * c7;
  s = s + c1;
  Ciphertext result = z * s + c0;
  result.set_output("y");
}
```