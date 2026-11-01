Attempt 2 is correct but slightly slower than attempt 1 (116.10 ms vs 115.84 ms). That difference is within noise, and the reordering did not remove the alignments I hoped it would. Within this formulation the remaining costs are the three ciphertext-ciphertext multiplies and the five ciphertext-plaintext multiplies. Folding the coefficients into the ciphertext products would trade a cheap multiply for an expensive one, so I'm keeping the attempt-1 kernel as the best version.

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