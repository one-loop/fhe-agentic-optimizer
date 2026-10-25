The baseline uses six ciphertext multiplies and depth 8. This version uses four. It applies the identity T5 = 2·T2·T3 − T1 and T7 = 2·T3·T4 − T1, so the odd high-order terms collapse into one ciphertext product T3·V, where V = 2(c5·T2 + c7·T4) + c3. The linear part c1·z − c5·z − c7·z and the constant c0 are added at the end using only plaintext multiplies and adds. The expected depth is about 5, and the terms that meet in each sum have different multiplication histories or sit at different levels, so the scale-matching rule is respected.

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