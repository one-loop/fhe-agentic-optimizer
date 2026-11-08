Attempt 3 is the best so far, so I keep its structure and remove the separate −c7·z correction. I fold that term into the c7 branch by computing T7 = 2·T3·T4 − z directly, so one scalar multiply (z·c7) goes away. The extra multiply by `one` stays, to keep the two level-5 terms from meeting at the same level. I have not run it.

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
  Ciphertext t7 = p + p - z;
  Ciphertext hi2 = (t7 * c7) * one;

  Ciphertext low = z * c1 + t2 * c2 + t4 * c4;
  low = low - z * c5;
  low = low + c0;
  low = low - c6;

  Ciphertext y = (hi + low) + hi2;
  y.set_output("y");
}
```