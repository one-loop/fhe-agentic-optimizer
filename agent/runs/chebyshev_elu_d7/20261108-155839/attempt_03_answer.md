Same-level scale mismatches are what the compiler rejects, so I force the two level-5 terms apart. The c7 term gets one extra multiply by the scalar `one`, which moves it to level 6. CHEHAB then aligns the shallower sum for free. This costs one more level than attempt 2 but is still far shallower than the baseline's 8.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext one("one");
  Plaintext c0("c0");
  Plaintext c1("c1");
  Plaintext c2("c2");
  Plaintext c3("c3");
  Plaintext c4("c4");
  Plaintext c5("c5");
  Plaintext c6("c6");
  Plaintext c7("c7");

  Ciphertext z = x * alpha;
  Ciphertext z2 = z * z;
  Ciphertext t2 = z2 + z2 - one;
  Ciphertext zt = z * t2;
  Ciphertext t3 = zt + zt - z;
  Ciphertext tt = t2 * t2;
  Ciphertext t4 = tt + tt - one;

  Ciphertext u = t2 * c5 + t3 * c6;
  Ciphertext inner = u + u + c3;
  Ciphertext hi = t3 * inner;

  Ciphertext p = t3 * t4;
  Ciphertext p2 = p + p;
  Ciphertext hi2 = (p2 * c7) * one;

  Ciphertext low = z * c1 + t2 * c2 + t4 * c4;
  low = low - z * c5;
  low = low - z * c7;
  low = low + c0;
  low = low - c6;

  Ciphertext y = (hi + low) + hi2;
  y.set_output("y");
}
```