Idea: convert to a power basis in z, then evaluate with fewer multiplications and a lower depth. Only plaintext inputs are available as constants, so the coefficient combinations have to be computed with plaintext-ciphertext operations.

Plaintext-plaintext arithmetic is forbidden, so I can't precompute the power-basis coefficients from c_k. Instead I keep the Chebyshev structure but cut the cost.

Plan:
- z = x*alpha costs one level. To save it, use T_k(z) in terms of x directly. That needs the scaling constants, which are not available, so the first multiply stays.
- Compute z2 = z*z, then t2 = 2z2 - 1 (depth 2).
- t3 = 2*z*t2 - z, t4 = 2*t2*t2 - 1 (depth 3), t4 needs t2 squared.
- t5 = 2*t2*t3 - z, t6 = 2*t3*t3 - 1, t7 = 2*t3*t4 - z, all at depth 4 with products of lower-depth terms.
- The coefficient multiplies add one more level, giving depth 5 overall instead of 8.

Cost: the products are z*z, z*t2, t2*t2, t2*t3, t3*t3, t3*t4, which is 6 ct-ct multiplies, the same count as the baseline but at lower depth. Each coefficient multiply is applied to a term at its own level, so the sum aligns.

Doubling is done by additions. The "-one" subtraction is a plaintext subtraction with a ciphertext operand, which is allowed. For t3 = 2*z*t2 - z, I compute m = z*t2, then m + m - z. Scale mismatches are handled by CHEHAB alignment, since the terms are at different levels.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext one("one");
  Ciphertext z = x * alpha;
  Ciphertext z2 = z * z;
  Ciphertext t2 = z2 + z2 - one;
  Ciphertext m3 = z * t2;
  Ciphertext t3 = m3 + m3 - z;
  Ciphertext m4 = t2 * t2;
  Ciphertext t4 = m4 + m4 - one;
  Ciphertext m5 = t2 * t3;
  Ciphertext t5 = m5 + m5 - z;
  Ciphertext m6 = t3 * t3;
  Ciphertext t6 = m6 + m6 - one;
  Ciphertext m7 = t3 * t4;
  Ciphertext t7 = m7 + m7 - z;
  Plaintext c1("c1"), c2("c2"), c3("c3"), c4("c4"), c5("c5"), c6("c6"), c7("c7"), c0("c0");
  Ciphertext result = z * c1;
  result = result + t2 * c2;
  result = result + t3 * c3;
  result = result + t4 * c4;
  result = result + t5 * c5;
  result = result + t6 * c6;
  result = result + t7 * c7;
  result = result + c0;
  result.set_output("y");
}
```