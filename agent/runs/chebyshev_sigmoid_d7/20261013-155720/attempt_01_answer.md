## Idea

The baseline spends 6 ciphertext-ciphertext multiplies building T1..T7 by recurrence. I use the identities T5 = 2·T2·T3 − z and T7 = 2·T3·T4 − z to regroup the odd part:

p(z) = c0 + (c1 − c5 − c7)·z + c3·T3 + 2·T3·(c5·T2 + c7·T4)

This needs only four ciphertext multiplies: z², T3 = (2z)·T2 − z, T4 = 2·T2² − 1, and the final T3 × (2A). The plaintext-scalar multiplies are cheaper (~5–7 ms) than ciphertext multiplies (~30 ms), so I spend a few extra of them on the linear terms (z·c1, z·c5, z·c7) instead of forming c1−c5−c7, which would be plaintext-plaintext arithmetic and is not allowed. Estimated cost is about 4 ct-ct muls plus 7 pt muls, versus 6 ct-ct and 5 pt in the baseline. Depth stays at about 5.

I checked the scale rules. The same-level sums combine terms with identical multiplication patterns (the three z·c_k products). Mixed-level sums are aligned for free.

```cpp
// Chebyshev sigmoid degree 7 on [-4,4], using
//   T5 = 2*T2*T3 - T1,  T7 = 2*T3*T4 - T1
// so that
//   p = c0 + (c1 - c5 - c7)*z + c3*T3 + 2*T3*(c5*T2 + c7*T4)
// Only 4 ciphertext-ciphertext multiplies are needed.

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

  Ciphertext z = x * alpha;            // z = x/4 in [-1,1]

  Ciphertext zz = z * z;
  Ciphertext T2 = zz + zz - one;       // T2 = 2z^2 - 1

  Ciphertext z2 = z + z;
  Ciphertext T3 = z2 * T2 - z;         // T3 = 2z*T2 - z

  Ciphertext T2sq = T2 * T2;
  Ciphertext T4 = T2sq + T2sq - one;   // T4 = 2*T2^2 - 1

  Ciphertext A = T2 * c5 + T4 * c7;    // c5*T2 + c7*T4
  Ciphertext A2 = A + A;
  Ciphertext B = T3 * A2;              // 2*T3*(c5*T2 + c7*T4)

  Ciphertext lin = z * c1 - z * c5 - z * c7;   // (c1 - c5 - c7)*z
  Ciphertext cub = T3 * c3;                    // c3*T3

  Ciphertext result = B + lin + cub + c0;
  result.set_output("y");
}
```