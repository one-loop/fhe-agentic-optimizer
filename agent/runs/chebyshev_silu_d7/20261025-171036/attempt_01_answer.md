Idea: only even terms T2, T4, T6 plus the odd term T1 are needed, so I write the result as a polynomial in z² and z.

With u = z², the even part is c0 + c2·T2 + c4·T4 + c6·T6, which is a cubic in u. The odd part is c1·z, since the other odd coefficients are dropped.

T2 = 2u − 1
T4 = 8u² − 8u + 1
T6 = 32u³ − 48u² + 18u − 1

Even part: c0 − c2 + c4 − c6 + u(2c2 − 8c4 + 18c6) + u²(8c4 − 48c6) + u³·32c6.

The coefficients must be built from the plaintext inputs, and plaintext-plaintext arithmetic is not allowed. So I fold the constants into ciphertext operations instead.

Let a = c1·alpha (needs a ct-pt multiply anyway). To avoid per-term plaintext products, I use x² directly: u = x²·alpha², and fold the scaling in as follows.
- w = x·alpha (1 plaintext multiplication, level 1).
- u = w² (1 ct multiplication, level 2).
- Then u² (level 3), and the cubic as Horner: ((B3·u + B2)·u + B1)·u + B0 would need plaintext coefficients that I can't compute.

Instead I use the Chebyshev form with only ciphertext-times-plaintext products of the given c's:
- t2 = u+u − one (level 2)
- t4 = (t2·t2)+(t2·t2) − one (level 3)
- t6 = 2·t2·t4 − t2 (level 4)
- result = w·c1 + t2·c2 + t4·c4 + t6·c6 + c0

This needs 3 ct-ct multiplications (u, t2², t2·t4) instead of 5, and the plaintext multiplications drop to 4 plus the one for w. Depth is 4 or 5 instead of 7.

Scale handling: the terms c1·w, c2·t2 and the others sit at different levels, so CHEHAB aligns them for free.

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
  Ciphertext w = x * alpha;
  Ciphertext u = w * w;
  Ciphertext t2 = u + u - one;
  Ciphertext s = t2 * t2;
  Ciphertext t4 = s + s - one;
  Ciphertext p = t2 * t4;
  Ciphertext t6 = p + p - t2;
  Ciphertext result = w * c1;
  result = result + t2 * c2;
  result = result + t4 * c4;
  result = result + t6 * c6;
  result = result + c0;
  result.set_output("y");
}
```