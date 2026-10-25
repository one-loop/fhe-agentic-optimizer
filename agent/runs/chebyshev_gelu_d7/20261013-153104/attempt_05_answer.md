I'm stopping here rather than spending more attempts. The last three kernels all land at about 231.5 ms, within noise of each other, and the structure has plateaued at the minimum I can find: three ciphertext-ciphertext multiplies (z², T2², T4·T2) and five plaintext multiplies. The remaining ideas I checked either keep that count or add multiplies. Removing the `alpha` multiply would need a plaintext `alpha²`, which the task does not provide.

This is the best kernel so far (attempt 4, 231.47 ms, about 24% faster than the 304.31 ms baseline, max error 1.19e-07):

```cpp
// Chebyshev gelu degree 7 (best): 3 ct-ct muls, 5 pt muls.
// Uses T_{m+n} = 2 T_m T_n - T_{|m-n|} to build T2, T4, T6 with three ciphertext
// products; c0 is folded into the shallowest term; terms are accumulated shallow to deep.

void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext one("one");

  Ciphertext z = x * alpha;                 // depth 1

  Ciphertext z2 = z * z;                    // depth 2
  Ciphertext T2 = z2 + z2 - one;            // T2 = 2z^2 - 1

  Ciphertext T2sq = T2 * T2;                // depth 3
  Ciphertext T4 = T2sq + T2sq - one;        // T4 = 2 T2^2 - 1

  Ciphertext T42 = T4 * T2;                 // depth 4
  Ciphertext T6 = T42 + T42 - T2;           // T6 = 2 T4 T2 - T2

  Plaintext c0("c0");
  Plaintext c1("c1");
  Plaintext c2("c2");
  Plaintext c4("c4");
  Plaintext c6("c6");

  Ciphertext term1 = z * c1;
  term1 = term1 + c0;
  Ciphertext term2 = T2 * c2;
  Ciphertext term4 = T4 * c4;
  Ciphertext term6 = T6 * c6;

  Ciphertext acc = term1 + term2;
  acc = acc + term4;
  acc = acc + term6;
  acc.set_output("y");
}
```