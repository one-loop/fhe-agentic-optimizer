The best previous kernel used four ciphertext-ciphertext multiplies. I replace the T3-based T6 with the identity T6 = 2·T2·T4 − T2, which needs only T2·T4. That drops to three ciphertext multiplies (z², T2², T2·T4) at the same multiplicative depth. The coefficient multiplies and the `alpha` scaling are unchanged.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext one("one");
  Ciphertext z = x * alpha;

  // T2 = 2 z^2 - 1
  Ciphertext z2 = z * z;
  Ciphertext t2 = z2 + z2 - one;

  // T4 = 2 T2^2 - 1
  Ciphertext t2sq = t2 * t2;
  Ciphertext t4 = t2sq + t2sq - one;

  // T6 = 2 T2 T4 - T2
  Ciphertext q = t2 * t4;
  Ciphertext t6 = q + q - t2;

  Plaintext c0("c0");
  Plaintext c1("c1");
  Plaintext c2("c2");
  Plaintext c4("c4");
  Plaintext c6("c6");

  Ciphertext y = z * c1;
  y = y + t2 * c2;
  y = y + t4 * c4;
  y = y + t6 * c6;
  y = y + c0;
  y.set_output("y");
}
```