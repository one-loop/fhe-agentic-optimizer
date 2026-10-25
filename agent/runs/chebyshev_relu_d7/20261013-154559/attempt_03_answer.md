Attempt 2 is the best so far at 229.62 ms, slightly ahead of attempt 1. I looked for further reductions but found none that cut the operation count. Folding the coefficients into fewer plaintext multiplies requires plaintext-plaintext products (not allowed), and rewriting T6 to reuse T4·T2 still leaves four plaintext multiplies. The alpha-scaling alternatives (x·x times alpha twice) also cost two plaintext multiplies, no better than the current one. So I'm keeping this formulation as the final kernel.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Ciphertext z = x * alpha;
  Plaintext one("one");

  // T2 = 2 z^2 - 1
  Ciphertext z2 = z * z;
  Ciphertext T2 = z2 + z2 - one;

  // T4 = 2 T2^2 - 1
  Ciphertext T2sq = T2 * T2;
  Ciphertext T4 = T2sq + T2sq - one;

  // T6 = 2 T4 T2 - T2
  Ciphertext T4T2 = T4 * T2;
  Ciphertext T6 = T4T2 + T4T2 - T2;

  Plaintext c1("c1");
  Plaintext c2("c2");
  Plaintext c4("c4");
  Plaintext c6("c6");
  Plaintext c0("c0");

  Ciphertext r1 = z * c1;
  Ciphertext r2 = T2 * c2;
  Ciphertext r4 = T4 * c4;
  Ciphertext r6 = T6 * c6;

  Ciphertext result = r1 + r2;
  result = result + r4;
  result = result + r6;
  result = result + c0;
  result.set_output("y");
}
```