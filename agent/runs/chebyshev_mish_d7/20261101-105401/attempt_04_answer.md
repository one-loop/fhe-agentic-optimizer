Attempt 3 is the best so far at 168.71 ms. This attempt keeps its arithmetic and only changes how T3 is built, to remove one scale alignment.

Before, T3 = 2·z·T2 − z, which subtracts z (level 1) from a level-3 product. CHEHAB had to align z with an extra multiply_plain + rescale. Now T3 = z·(2·T2 − one): the constant is subtracted at level 2, before the single ciphertext multiplication, so no alignment is needed.

The rest is attempt 3's circuit:
- **High part:** T4·(2q + c4), with q = c5·z + c6·T2 + c7·T3.
- **Low part:** (c1 − c7)·z + (c2 − c6)·T2 + (c3 − c5)·T3 + c0. Each difference costs two scalar plaintext multiplications, with c6·T2 shared.

The expected gain is one match_scale, about 5 ms. This has not been run yet.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext one("one");
  Plaintext c0("c0"), c1("c1"), c2("c2"), c3("c3"), c4("c4"), c5("c5"), c6("c6"), c7("c7");

  Ciphertext z = x * alpha;
  Ciphertext zz = z * z;
  Ciphertext T2 = zz + zz - one;
  Ciphertext W = T2 + T2 - one;
  Ciphertext T3 = z * W;
  Ciphertext T2s = T2 * T2;
  Ciphertext T4 = T2s + T2s - one;

  Ciphertext c6T2 = T2 * c6;
  Ciphertext q = z * c5 + c6T2 + T3 * c7;
  Ciphertext q2 = (q + q) + c4;
  Ciphertext high = T4 * q2;

  Ciphertext low = (z * c1 - z * c7) + (T2 * c2 - c6T2) + (T3 * c3 - T3 * c5);
  Ciphertext result = high + low + c0;
  result.set_output("y");
}
```