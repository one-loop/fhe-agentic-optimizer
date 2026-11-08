Idea: keep the BSGS structure, but stop building T3 as a separate ciphertext. Since T3 = 2·z·T2 − z, I apply the coefficients to `e = z·T2` and to `z` directly. This drops the T3 subtraction and its level alignment, and T3 is no longer on the critical path to B. The z-multiples (c1, c3, c5, c7) all sit at level 2 with identical history, so they combine freely, and groups at other levels are added in increasing level order.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext one("one");
  Plaintext c0("c0"), c1("c1"), c2("c2"), c3("c3"), c4("c4"), c5("c5"), c6("c6"), c7("c7");
  Ciphertext z = x * alpha;
  Ciphertext zz = square(z);
  Ciphertext t2 = zz + zz - one;
  Ciphertext e = z * t2;
  Ciphertext t2s = square(t2);
  Ciphertext t4 = t2s + t2s - one;

  Ciphertext zc1 = z * c1;
  Ciphertext zc3 = z * c3;
  Ciphertext zc5 = z * c5;
  Ciphertext zc7 = z * c7;
  Ciphertext t2c2 = t2 * c2;
  Ciphertext t2c6 = t2 * c6;
  Ciphertext ec3 = e * c3;
  Ciphertext ec5 = e * c5;
  Ciphertext ec7 = e * c7;

  // B = c4 + 2(c5-c7) z + 2 c6 T2 + 4 c7 e
  Ciphertext bz = zc5 - zc7;
  Ciphertext b = bz + bz;
  b = b + (t2c6 + t2c6);
  Ciphertext e2 = ec7 + ec7;
  b = b + (e2 + e2);
  b = b + c4;
  Ciphertext hi = t4 * b;

  // A = c0 + (c1-c7-c3+c5) z + (c2-c6) T2 + 2(c3-c5) e
  Ciphertext a = (zc1 - zc7) - (zc3 - zc5);
  a = a + (t2c2 - t2c6);
  Ciphertext d = ec3 - ec5;
  a = a + (d + d);
  a = a + c0;

  Ciphertext r = a + hi;
  r.set_output("y");
}
```