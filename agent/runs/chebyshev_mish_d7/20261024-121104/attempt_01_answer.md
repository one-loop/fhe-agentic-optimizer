Idea: evaluate the polynomial in the monomial basis with a reduced multiplication count. The coefficients are plaintext inputs, so I can't fold them into new constants at compile time. Converting Chebyshev to monomial form would need plaintext-plaintext arithmetic, which is not allowed. So I use a Paterson-Stockmeyer-like split on the Chebyshev basis directly.

Let z = x*alpha (one plaintext multiply, level 1).
- z2 = z*z (level 2).
- T2 = 2z2 - 1.
- T3 = 2*z*T2 - z (level 3).
- T4 = 2*T2² - 1 (level 3).

Write p = c0 + c1 T1 + c2 T2 + c3 T3 + (T4 * (c4 + c5 T1 + c6 T2 + c7 T3)), using T_{4+j} = 2 T4 Tj - T_{4-j}. So:
- T5 = 2 T4 T1 - T3
- T6 = 2 T4 T2 - T2
- T7 = 2 T4 T3 - T1

Then c4 T4 + c5 T5 + c6 T6 + c7 T7 = T4 * (2(c5 T1 + c6 T2 + c7 T3) + c4) - c5 T3 - c6 T2 - c7 T1.

The doubling can be absorbed by adding the product to itself, which is cheap.

Let Q = c4 + 2(c5 z + c6 T2 + c7 T3). The terms c5*z, c6*T2 and c7*T3 are plaintext multiplies, and T3 is at depth 3, so Q is at depth 4. Then T4*Q is a ct-ct multiply at depth 5. The total number of ct-ct multiplies is z2, z*T2, T2², and T4*Q, which is 4 instead of 6. Depth is about 5.

Final result:
y = c0 + (c1 - c7) z + (c2 - c6) T2 + (c3 - c5) T3 + T4*Q.

The subtractions are ciphertext operations applied to the products c_k*T_k, so I compute each c_k*T_k separately and subtract. That adds a few plaintext multiplies, which are cheap. T4 = 2 T2² - 1 uses `one`.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext one("one");
  Plaintext c0("c0"), c1("c1"), c2("c2"), c3("c3"), c4("c4"), c5("c5"), c6("c6"), c7("c7");
  Ciphertext z = x * alpha;
  Ciphertext z2 = z * z;
  Ciphertext T2 = z2 + z2 - one;
  Ciphertext zT2 = z * T2;
  Ciphertext T3 = zT2 + zT2 - z;
  Ciphertext s2 = T2 * T2;
  Ciphertext T4 = s2 + s2 - one;

  Ciphertext a5 = z * c5;
  Ciphertext a6 = T2 * c6;
  Ciphertext a7 = T3 * c7;
  Ciphertext inner = a5 + a6 + a7;
  Ciphertext Q = inner + inner + c4;
  Ciphertext P = T4 * Q;

  Ciphertext r = P + c0;
  r = r + z * c1 - z * c7;
  r = r + T2 * c2 - T2 * c6;
  r = r + T3 * c3 - T3 * c5;
  r.set_output("y");
}
```