The kernel is at a plateau, so I'm not spending the remaining attempts on it. Attempt 6 was correct and measured 115.86 ms, against 115.00 ms for attempt 2. The gap is run-to-run noise, since the circuit is identical. The best kernel is attempt 2, about 23% faster than the 150.21 ms baseline.

Why it stops improving:
- The degree-6 polynomial needs at least three ciphertext multiplies. This kernel uses exactly three, which is the floor for a power-based evaluation.
- The five plaintext multiplies (the `alpha` scaling and four coefficient multiplies) can't be merged, because combining coefficients would need plaintext-plaintext arithmetic, which the DSL forbids.

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