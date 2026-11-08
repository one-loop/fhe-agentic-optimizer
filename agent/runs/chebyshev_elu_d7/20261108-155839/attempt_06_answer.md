I try dropping the extra multiply by `one`: c7·T4 is rewritten as 2·(c7·T2)·T2 − c7, which saves a plaintext multiply. It sits at level 4, the same as the c5·T2 + c6·T3 sum, so it may hit the scale mismatch again. If it does, I will revert to the attempt-5 kernel.

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

  Ciphertext a = t2 * c7;
  Ciphertext w = a * t2;
  Ciphertext w4 = (w + w) - c7;
  Ciphertext u = t2 * c5 + t3 * c6 + w4;
  Ciphertext inner = u + u + c3;
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