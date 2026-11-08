I merge the c7 branch back into the single T3 product: c7·T4 is multiplied by `one` to move it up a level (level 5), so it no longer collides with the level-4 sum c5·T2 + c6·T3. This removes one ciphertext-ciphertext multiplication (4 total). The z·c7 correction comes back as a scalar multiply, so the count of plaintext multiplications stays at 9. I have not run it.

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
  Ciphertext w = (t4 * c7) * one;
  Ciphertext inner = (u + u + c3) + (w + w);
  Ciphertext hi = t3 * inner;

  Ciphertext low = z * c1 + t2 * c2 + t4 * c4;
  low = low - z * c5;
  low = low - z * c7;
  low = low + c0;
  low = low - c6;

  Ciphertext y = hi + low;
  y.set_output("y");
}
```