Idea: convert to the power basis in the plaintext domain is not possible (coefficients are only available as encrypted-side plaintext inputs, and plaintext-plaintext arithmetic is disallowed). Instead I reduce the ciphertext work by evaluating the polynomial with a baby-step/giant-step split on the Chebyshev basis.

Plan: z = x*alpha (1 pt mul). Compute:
- z2 = z*z, T2 = 2z2 - 1
- T3 = 2*z*T2 - z, T4 = 2*T2*T2 - 1 (squares)
- Use T_{4+j} = 2*T4*T_j - T_{4-j}, so T5 = 2 T4 T1 - T3, T6 = 2 T4 T2 - T2, T7 = 2 T4 T3 - T1.

So p(z) = (c0 + c1 T1 + c2 T2 + c3 T3) + T4*(2 c5 T1 + 2 c6 T2 + 2 c7 T3 + c4) - c5 T3 - c6 T2 - c7 T1.
Combine the low parts: A = c0 + (c1-c7) T1 + (c2-c6) T2 + (c3-c5) T3, which needs plaintext subtractions. Those are allowed since the Ciphertext operand is absent... they are not (plaintext-plaintext), so I keep the products separate instead.

Written explicitly, the high part is B = c4 + 2c5 T1 + 2c6 T2 + 2c7 T3, built with ciphertext-plaintext multiplies and additions, where doubling is done on the ciphertext side (T1+T1 etc.). The result is A + T4*B, with A = c0 + c1 T1 + c2 T2 + c3 T3 - c5 T3 - c6 T2 - c7 T1.

Ciphertext-ciphertext multiplies: z2, T3, T4, and T4*B, which makes 4 instead of 6, and fewer plaintext multiplies than the baseline's 8 are not achievable here (11 vs 8), but the depth is shorter. The terms in A and B mix levels, which CHEHAB aligns.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext one("one");
  Plaintext c0("c0"), c1("c1"), c2("c2"), c3("c3"), c4("c4"), c5("c5"), c6("c6"), c7("c7");
  Ciphertext z = x * alpha;
  Ciphertext z2 = square(z);
  Ciphertext T2 = z2 + z2 - one;
  Ciphertext zt2 = z * T2;
  Ciphertext T3 = zt2 + zt2 - z;
  Ciphertext s2 = square(T2);
  Ciphertext T4 = s2 + s2 - one;

  Ciphertext T1d = z + z;
  Ciphertext T2d = T2 + T2;
  Ciphertext T3d = T3 + T3;

  Ciphertext B = T1d * c5 + T2d * c6 + T3d * c7 + c4;
  Ciphertext A = z * c1 + T2 * c2 + T3 * c3 + c0;
  Ciphertext sub = T3 * c5 + T2 * c6 + z * c7;
  Ciphertext r = (A - sub) + T4 * B;
  r.set_output("y");
}
```