Attempt 4 is correct and marginally faster (115.69 ms vs 115.83 ms). It is effectively the same kernel as before, so the gain is within noise. The structure is unchanged: three ciphertext-ciphertext multiplies (z², T2², T4·T2) and four ciphertext-plaintext coefficient multiplies, at depth 5. I'm keeping it as the final kernel because the remaining options either trade a cheap ciphertext-plaintext multiply for an expensive ciphertext-ciphertext one or cannot reduce the count of either.

```cpp
// Chebyshev softplus degree 7 on [-4,4]; only T1,T2,T4,T6 are needed (c3,c5,c7 ~ 0).
// Uses T_{2n} = 2 T_n^2 - 1 and T_{m+n} = 2 T_m T_n - T_{|m-n|}.
// Ct-ct muls: z*z, T2*T2, T4*T2 (3 total). Depth 5.

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