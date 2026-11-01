The three ciphertext-ciphertext multiplies (z², T2², T4·T2) look close to minimal for a degree-6 polynomial, so the remaining cost is mostly in the ciphertext-plaintext multiplies and level alignments. This variant sums the terms from deepest to shallowest. Each addition then mixes operands whose levels are closer together, which should reduce the number of alignments CHEHAB inserts. The gain is probably small.

```cpp
// Chebyshev softplus degree 7 on [-4,4]; only T1,T2,T4,T6 are needed (c3,c5,c7 ~ 0).
// Ct-ct muls: z*z, T2*T2, T4*T2 (3 total). Terms summed deepest-first to reduce level alignment.

void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext one("one");

  Ciphertext z = x * alpha;            // z = x/4 in [-1,1]
  Ciphertext z2 = z * z;               // z^2
  Ciphertext T2 = z2 + z2 - one;       // T2 = 2z^2 - 1
  Ciphertext p22 = T2 * T2;            // T2^2
  Ciphertext T4 = p22 + p22 - one;     // T4 = 2T2^2 - 1
  Ciphertext p42 = T4 * T2;            // T4*T2
  Ciphertext T6 = p42 + p42 - T2;      // T6 = 2 T4 T2 - T2

  Plaintext c1("c1");
  Plaintext c2("c2");
  Plaintext c4("c4");
  Plaintext c6("c6");
  Plaintext c0("c0");

  Ciphertext term6 = T6 * c6;
  Ciphertext term4 = T4 * c4;
  Ciphertext term2 = T2 * c2;
  Ciphertext term1 = z * c1;

  Ciphertext result = term6 + term4;
  result = result + term2;
  result = result + term1;
  result = result + c0;
  result.set_output("y");
}
```