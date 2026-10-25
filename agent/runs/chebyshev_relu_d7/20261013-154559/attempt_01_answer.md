The baseline spends six ciphertext-ciphertext multiplies building T3, T5 and T7, but the coefficients c3, c5 and c7 are negligible, so those polynomials are never needed. This kernel uses the identities T4 = 2T2² − 1 and T6 = 2T4·T2 − T2 to get T2, T4 and T6 with only three ciphertext multiplies (z², T2², T4·T2). The final sum uses four scalar plaintext multiplies. Multiplicative depth drops to about 5, within the 10 available levels.

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