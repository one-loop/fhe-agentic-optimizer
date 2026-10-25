Attempt 1 cut the ciphertext multiplies from 5 to 3, but the timing still includes 5 plaintext multiplies and several level alignments (4 match_scale, 5 mod_switch). The three ciphertext multiplies are close to the minimum for a degree-7 polynomial, so the remaining gain is in the additions. This variant adds c0 to the shallowest term early and accumulates the terms from shallow to deep, so each addition meets operands in a consistent order. The arithmetic is the same, and I have not measured whether this reduces alignment work.

```cpp
// Chebyshev gelu degree 7: same arithmetic as attempt 1 (3 ct-ct muls, 5 pt muls).
// Change: c0 is folded into the shallowest term early, and terms are accumulated
// from shallow to deep to reduce level/scale alignment work.

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

  Ciphertext term1 = z * c1;                // shallowest term
  term1 = term1 + c0;                       // add constant term early (cheap plaintext add)
  Ciphertext term2 = T2 * c2;
  Ciphertext term4 = T4 * c4;
  Ciphertext term6 = T6 * c6;

  Ciphertext acc = term1 + term2;
  acc = acc + term4;
  acc = acc + term6;
  acc.set_output("y");
}
```