I'm stopping here. Attempts 2 through 5 all compile to the same circuit, and the measured times (115.00, 116.24, 116.11, 117.64 ms) differ only by run-to-run noise, so further resubmissions won't improve the result. The best kernel is attempt 2, about 23% faster than the 150.21 ms baseline.

What it does:
- It evaluates the required Chebyshev terms T2, T4 and T6 with doubling identities, T2 = 2z²−1, T4 = 2T2²−1 and T6 = 2·T2·T4 − T2, so only three ciphertext multiplies are needed. Degree 6 needs at least three, so this is the floor for this approach.
- The remaining cost is five plaintext multiplies (the `alpha` scaling and four coefficient multiplies). Merging them would require plaintext-plaintext arithmetic, which the DSL forbids.

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