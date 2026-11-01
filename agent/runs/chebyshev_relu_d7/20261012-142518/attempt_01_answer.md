The baseline's Chebyshev recurrence has depth 7. I restructure the evaluation with squaring identities: T4 = 2·T2² − 1 and T6 = 2·T3² − 1, where T2 = 2z² − 1 and T3 = 2z·T2 − z. This gives four ciphertext-ciphertext multiplications (z², T2², z·T2, T3²) instead of the recurrence chain, and the multiplicative depth drops to about 5. The coefficient multiplications are the same four ciphertext-plaintext products as before. Subtractions of `one` and `z` mirror the baseline's pattern, so the scale alignment should be unchanged.

```cpp
// Chebyshev relu degree 7 on [-4,4], restructured with squaring identities:
// T2 = 2z^2 - 1, T4 = 2*T2^2 - 1, T3 = 2z*T2 - z, T6 = 2*T3^2 - 1.
// Only 4 ciphertext-ciphertext multiplications, depth ~5.

void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext one("one");

  Ciphertext z = x * alpha;            // z = x/4 in [-1,1]

  Ciphertext z2 = z * z;               // z^2
  Ciphertext T2 = z2 + z2 - one;       // T2

  Ciphertext T2sq = T2 * T2;           // T2^2
  Ciphertext T4 = T2sq + T2sq - one;   // T4

  Ciphertext zT2 = z * T2;             // z*T2
  Ciphertext T3 = zT2 + zT2 - z;       // T3

  Ciphertext T3sq = T3 * T3;           // T3^2
  Ciphertext T6 = T3sq + T3sq - one;   // T6

  Plaintext c0("c0");
  Plaintext c1("c1");
  Plaintext c2("c2");
  Plaintext c4("c4");
  Plaintext c6("c6");

  Ciphertext term1 = z * c1;
  Ciphertext term2 = T2 * c2;
  Ciphertext term4 = T4 * c4;
  Ciphertext term6 = T6 * c6;

  Ciphertext result = term1 + term2;
  result = result + term4;
  result = result + term6;
  result = result + c0;
  result.set_output("y");
}
```