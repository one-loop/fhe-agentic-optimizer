Idea: reduce the number of ct-ct multiplications and plaintext multiplications by evaluating the polynomial in the power basis, with the coefficients folded in by a Paterson–Stockmeyer-like split, using only the given plaintext inputs.

Since the coefficients are plaintext inputs, I cannot convert the basis at compile time. Instead I use the Chebyshev identities with the coefficients folded in, so that each plaintext multiply is applied to a low-depth term:

- Let z = alpha*x (one plaintext multiply, depth 1).
- z2 = z*z, T2 = 2 z2 - 1.
- T4 = 2 T2^2 - 1.
- T3 = 2 z T2 - z.
- Split: p = (c0 + c1 z + c2 T2 + c3 T3) + T4 * (c4 + c5 z + c6 T2 + c7 T3), using T4+j = 2 T4 Tj - T(4-j).
 - Sum for k = 4..7: c_k T_k = c_k (2 T4 T_{k-4}... ) which brings extra terms. This is tricky, so I use the simpler identity T_{4+j} = 2 T4 Tj - T_{4-j} (for j=0..3):
 - T4 = T4, T5 = 2 T4 z - z, T6 = 2 T4 T2 - T2, T7 = 2 T4 T3 - z... wait, T7 = 2 T4 T3 - T1 (since 4-3 = 1), and T5 = 2 T4 T1 - T3, T6 = 2 T4 T2 - T2.
- Then the result is:
 - A = c0 + c1 z + c2 T2 + c3 T3 - c5 T3 - c7 z
 - B = c4 T4 + 2 T4 (c5 z + c6 T2 + c7 T3) - c6 T2
 - which gives y = A' + T4 * (c4 + 2(c5 z + c6 T2 + c7 T3)), with A' = c0 + (c1 - c7) z + (c2 - c6) T2 + (c3 - c5) T3.

The coefficient differences need plaintext-plaintext subtraction, which is not allowed, so I apply them on the ciphertext side instead: A' = c0 + c1 z - c7 z + ..., with each product a separate ct-pt multiply. This costs the same number of plaintext multiplies, but the ct-ct multiplies drop to z2, z*T2, T4 and T4*Q (4 total, versus 6).

Depth: z (1), T2 (2), T3 (3), T4 (3), Q = c-terms on T3 (4), T4*Q (5). The result is at depth 5 instead of 8.

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
  Ciphertext zT2 = z * T2;
  Ciphertext T3 = zT2 + zT2 - z;
  Ciphertext T2s = square(T2);
  Ciphertext T4 = T2s + T2s - one;

  Ciphertext Q = (z * c5) + (T2 * c6) + (T3 * c7);
  Ciphertext inner = (Q + Q) + c4;
  Ciphertext hi = T4 * inner;

  Ciphertext lo = (z * c1) + (T2 * c2) + (T3 * c3);
  Ciphertext m = (z * c7) + (T2 * c6) + (T3 * c5);
  Ciphertext r = hi + lo - m;
  r = r + c0;
  r.set_output("y");
}
```