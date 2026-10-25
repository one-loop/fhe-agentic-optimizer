I'm keeping the attempt 1 kernel as the final answer. Attempt 6 used the same kernel and measured 296.9 ms; across attempts 1, 3, 4, 5 and 6 the same code spans 284–297 ms, so the spread is run-to-run noise rather than a real difference.

The kernel uses four ciphertext multiplies (baseline: six) and reaches depth 5 (baseline: 8). It relies on the identities T5 = 2·T2·T3 − T1, T7 = 2·T3·T4 − T1 and T4 = 2·T2² − 1, which collapse the odd high-order terms into one product T3·V. Four ciphertext multiplies is close to the minimum for degree 7, so I don't expect a further restructuring to help meaningfully.

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