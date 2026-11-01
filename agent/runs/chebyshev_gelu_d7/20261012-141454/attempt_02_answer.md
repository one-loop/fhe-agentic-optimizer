The previous kernel already uses the minimum of 3 ciphertext-ciphertext multiplications (z², T2², T4·T2 give T2, T4, T6 at depth 5). The remaining cost is 5 ciphertext-plaintext multiplications (alpha, c1, c2, c4, c6), which can't be fused: plaintext-plaintext products are disallowed, so the coefficients can't be merged into one scalar. I'm resubmitting the same computation with the terms summed in a different order, which may reduce level-alignment work. The gain is likely small.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext one("one");

  // z = x/4 in [-1, 1]
  Ciphertext z = x * alpha;

  // T2 = 2z^2 - 1
  Ciphertext z2 = z * z;
  Ciphertext T2 = z2 + z2 - one;

  // T4 = 2*T2^2 - 1
  Ciphertext T2sq = T2 * T2;
  Ciphertext T4 = T2sq + T2sq - one;

  // T6 = 2*T4*T2 - T2
  Ciphertext P = T4 * T2;
  Ciphertext T6 = P + P - T2;

  Plaintext c6("c6");
  Ciphertext term6 = T6 * c6;
  Plaintext c4("c4");
  Ciphertext term4 = T4 * c4;
  Plaintext c2("c2");
  Ciphertext term2 = T2 * c2;
  Plaintext c1("c1");
  Ciphertext term1 = z * c1;

  // Sum the deepest terms first so the shallower ones align to them.
  Ciphertext result = term6 + term4;
  result = result + term2;
  result = result + term1;

  Plaintext c0("c0");
  result = result + c0;
  result.set_output("y");
}
```