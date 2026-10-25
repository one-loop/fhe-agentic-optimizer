This attempt doesn't beat attempt 1, so I'm keeping the same structure. The three ciphertext-ciphertext multiplies (z², T2², T4·T2) are the minimum I can find for a degree-6 evaluation, and the five plaintext multiplies are each needed. The remaining candidate savings are alignment and plaintext-add overhead, worth a few milliseconds at most. Alternatives I considered (folding coefficients into earlier multiplies, or dropping the alpha scaling) either need plaintext-plaintext products, which CKKS disallows, or add ciphertext multiplies. Attempt 1 remains the best kernel.

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