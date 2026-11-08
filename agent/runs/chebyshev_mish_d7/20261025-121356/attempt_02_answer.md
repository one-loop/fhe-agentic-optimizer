The previous attempt failed because `c4*T4` met the `T3`-level terms at the same level with a different scale. I fold `c4` into the big product instead, using `T4*(D + c4)`. This removes one plaintext multiplication, and the two groups now meet at different levels (5 and 4), where CHEHAB aligns them for free.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext one("one");
  Plaintext c0("c0"), c1("c1"), c2("c2"), c3("c3"), c4("c4"), c5("c5"), c6("c6"), c7("c7");
  Ciphertext z = x * alpha;
  Ciphertext z2 = square(z);
  Ciphertext t2 = z2 + z2 - one;
  Ciphertext zt2 = z * t2;
  Ciphertext t3 = zt2 + zt2 - z;
  Ciphertext t2s = square(t2);
  Ciphertext t4 = t2s + t2s - one;
  Ciphertext D0 = z * c5 + t2 * c6 + t3 * c7;
  Ciphertext D = D0 + D0 + c4;
  Ciphertext low = z * c1 + t2 * c2 + t3 * c3;
  Ciphertext sub = t3 * c5 + t2 * c6 + z * c7;
  Ciphertext r = (low - sub) + t4 * D;
  Ciphertext y = r + c0;
  y.set_output("y");
}
```