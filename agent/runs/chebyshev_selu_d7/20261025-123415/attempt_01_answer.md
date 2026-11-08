Idea: evaluate the polynomial in the monomial basis, so the Chebyshev recurrence's multiplications go away. The coefficients are plaintext inputs, so I can't precompute the monomial coefficients (plaintext-plaintext arithmetic is forbidden). Instead I use a baby-step/giant-step style evaluation with the Chebyshev basis directly.

Compute z = x*alpha (1 level, ct-pt mul).
- z2 = z*z (level 2), T2 = 2z2 - one.
- T3 = 2*z*T2 - z, T4 = 2*T2^2 - one.

Then
p = c0 + c1 T1 + c2 T2 + c3 T3 + (T4)(c4 + c5 T1 + c6 T2 + c7 T3).

This works because T_{4+j} = 2 T4 T_j - T_{4-j}, so T4*Tj-type terms combine:
- T5 = 2 T4 T1 - T3
- T6 = 2 T4 T2 - T2... precisely T6 = 2 T4 T2 - T2 (since T_{4-2} = T2)
- T7 = 2 T4 T3 - T1

So the sum is c5 T5 + c6 T6 + c7 T7 = 2 T4 (c5 T1 + c6 T2 + c7 T3) - (c5 T3 + c6 T2 + c7 T1).
The full result is:
y = c0 + c1 T1 + c2 T2 + c3 T3 + c4 T4 + 2 T4 · Q - (c5 T3 + c6 T2 + c7 T1), with Q = c5 T1 + c6 T2 + c7 T3.

Ct-ct multiplications: z2, z*T2, T2^2, T4*Q, which is 4 instead of 6.

Depth: z at level 1, T2 at level 2, T3 at level 3, T4 at level 3. Q is a ct-pt product of T3, so level 4. T4*Q is then level 5. This is shallower than the baseline.

Scale caution: terms are added at different levels, which CHEHAB aligns. Since T3 = 2 z T2 - z, the subtraction mixes z (level 1) with z*T2 (level 3), which is fine.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext one("one");
  Plaintext c0("c0"), c1("c1"), c2("c2"), c3("c3"), c4("c4"), c5("c5"), c6("c6"), c7("c7");
  Ciphertext z = x * alpha;
  Ciphertext z2 = z * z;
  Ciphertext t2 = z2 + z2 - one;
  Ciphertext zt2 = z * t2;
  Ciphertext t3 = zt2 + zt2 - z;
  Ciphertext sq = t2 * t2;
  Ciphertext t4 = sq + sq - one;
  Ciphertext q = z * c5 + t2 * c6 + t3 * c7;
  Ciphertext r = z * c7 + t2 * c6 + t3 * c5;
  Ciphertext m = t4 * q;
  Ciphertext low = z * c1 + t2 * c2 + t3 * c3 + t4 * c4;
  Ciphertext y = low + m + m - r;
  y = y + c0;
  y.set_output("y");
}
```