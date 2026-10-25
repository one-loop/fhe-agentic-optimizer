Attempt 1 is already at 4 ciphertext multiplies, which is close to the minimum for a degree-7 evaluation. A Paterson–Stockmeyer-style split (z², z³, z⁴ then one block product) also needs 4. The remaining cost is mostly the ciphertext multiplies plus level alignment. In this variant I fold c0 into the cheap low-level linear part, so one plaintext add happens at low level and the final sum is just `prod + lin`. This is a small change and may only give a marginal gain.

```cpp
// Chebyshev hardshrink degree 7 on [-4, 4].
// T5 = 2*T2*T3 - T1, T7 = 2*T3*T4 - T1, T4 = 2*T2^2 - 1.
// p = T3 * (2*(c5*T2 + c7*T4) + c3) + (c1 - c5 - c7) z + c0.
// 4 ciphertext-ciphertext multiplies (z^2, z*T2, T2^2, T3*V), depth ~5.

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

  // Low-level linear part first (cheap, shallow level).
  Ciphertext lin = z * c1 - z * c5 - z * c7 + c0;

  Ciphertext z2 = z * z;
  Ciphertext T2 = z2 + z2 - one;     // T2 = 2z^2 - 1

  Ciphertext zT2 = z * T2;
  Ciphertext T3 = zT2 + zT2 - z;     // T3 = 2 z T2 - z

  Ciphertext T2sq = T2 * T2;
  Ciphertext T4 = T2sq + T2sq - one; // T4 = 2 T2^2 - 1

  Ciphertext W = T2 * c5 + T4 * c7;  // c5*T2 + c7*T4
  Ciphertext V = W + W + c3;         // 2W + c3

  Ciphertext prod = T3 * V;

  Ciphertext result = prod + lin;
  result.set_output("y");
}
```