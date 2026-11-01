I'm not using the last attempt. Attempt 7 is the same circuit again and measured 116.01 ms, within the same ±1 ms noise as the rest. The kernel is at its practical floor, and I'd only be repeating it.

The best kernel is attempt 2 (115.00 ms, against 150.21 ms for the baseline, about 23% faster):

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