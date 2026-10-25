I'm stopping here and keeping the attempt 1 kernel as the final answer. Attempts 1, 3, 4 and 5 used the same kernel and measured 284–296 ms, so the spread is noise. Attempt 2 was a reordering that measured slower (319.7 ms). Attempt 5 measured 285.6 ms, within that noise band.

The kernel uses four ciphertext multiplies, against six in the baseline, and reaches depth 5 instead of 8. The identities T5 = 2·T2·T3 − T1, T7 = 2·T3·T4 − T1 and T4 = 2·T2² − 1 let the high-order odd terms collapse into one product T3·V. Four ciphertext multiplies is close to the minimum for degree 7 with this approach, so the remaining time is mostly ciphertext multiplies and level alignment. I don't see a restructuring that removes either.

```cpp
// Chebyshev hardshrink degree 7 on [-4, 4].
// Uses T5 = 2*T2*T3 - T1 and T7 = 2*T3*T4 - T1 so that
// c5*T5 + c7*T7 + c3*T3 = T3 * (2*(c5*T2 + c7*T4) + c3) + (c5+c7)*(-z).
// Only 4 ciphertext-ciphertext multiplies (z^2, z*T2, T2^2, T3*V), depth ~5.

void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext one("one");
  Plaintext c0("c0");
  Plaintext c1("c1");
  Plaintext c3("c3");
  Plaintext c5("c5");
  Plaintext c7("c7");

  Ciphertext z = x * alpha;          // z in [-1, 1]

  Ciphertext z2 = z * z;
  Ciphertext T2 = z2 + z2 - one;     // T2 = 2z^2 - 1

  Ciphertext zT2 = z * T2;
  Ciphertext T3 = zT2 + zT2 - z;     // T3 = 2 z T2 - z

  Ciphertext T2sq = T2 * T2;
  Ciphertext T4 = T2sq + T2sq - one; // T4 = 2 T2^2 - 1

  Ciphertext W = T2 * c5 + T4 * c7;  // c5*T2 + c7*T4
  Ciphertext V = W + W + c3;         // 2W + c3

  Ciphertext prod = T3 * V;          // T3 * (2c5T2 + 2c7T4 + c3)

  Ciphertext lin = z * c1 - z * c5 - z * c7;  // (c1 - c5 - c7) z

  Ciphertext result = prod + lin + c0;
  result.set_output("y");
}
```