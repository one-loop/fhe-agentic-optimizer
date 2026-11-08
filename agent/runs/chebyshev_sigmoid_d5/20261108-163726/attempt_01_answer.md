The sigmoid approximation only uses odd powers of z, so I rewrote it as a polynomial in z and w = z². With T3 = 4z³ − 3z and T5 = 16z⁵ − 20z³ + 5z, the result is:

y = c0 + z·(c1 − 3c3 + 5c5) + 4·w·z·(c3 − 5c5 + 4c5·w)

The plaintext coefficients can't be combined directly, so I pre-multiply z by each of them: u = c5·z, v = c3·z. Their constant multiples (3v, 5u, 4×, and so on) are built from ciphertext additions, which are cheap. This gives:

- r = v − 5u + 4·(u·w)
- y = c0 + (c1·z − 3v + 5u) + 4·(w·r)

This uses 3 ciphertext-ciphertext multiplications (w, u·w, w·r) instead of 4, and 4 plaintext multiplications (alpha, c1, c3, c5), the same as the baseline. The depth is 4 levels instead of 6. It does not use `one`. I haven't run this against CHEHAB, so the timing and the 1e-06 accuracy are untested.

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
  Ciphertext w = z * z;
  Ciphertext u = z * c5;
  Ciphertext v = z * c3;
  Ciphertext s = z * c1;

  Ciphertext uw = u * w;
  Ciphertext uw2 = uw + uw;
  Ciphertext uw4 = uw2 + uw2;
  Ciphertext u2 = u + u;
  Ciphertext u4 = u2 + u2;
  Ciphertext u5 = u4 + u;
  Ciphertext r = (v - u5) + uw4;

  Ciphertext m = w * r;
  Ciphertext m2 = m + m;
  Ciphertext m4 = m2 + m2;

  Ciphertext v2 = v + v;
  Ciphertext v3 = v2 + v;
  Ciphertext low = (s - v3) + u5;

  Ciphertext result = low + m4;
  result = result + c0;
  result.set_output("y");
}
```