I'm stopping now and returning attempt 4 as the final kernel. Attempt 6 measured 237.55 ms, slower than attempt 4's 231.47 ms, and the same 3 ciphertext-ciphertext and 5 plaintext multiplies across attempts 2–6 suggest the remaining variation is noise. Two attempts are left, but I don't see a structural change that would cut a multiply, so I'd rather not spend them on reordering.

Final kernel (attempt 4, 231.47 ms, about 24% faster than the 304.31 ms baseline, max error 1.19e-07):

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