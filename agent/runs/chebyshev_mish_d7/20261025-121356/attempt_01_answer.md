Idea: evaluate the polynomial in the monomial basis, with the constant coefficients folded in, to cut ciphertext multiplications and plaintext multiplications.

Rewriting everything in the monomial basis would need constants that are not available as inputs, because plaintext-plaintext arithmetic is not allowed. So I keep the Chebyshev coefficients and use a cheaper evaluation structure instead.

Let z = x*alpha. Compute these powers of z:
- z2 = z*z
- z3 = z2*z
- z4 = z2*z2

The Chebyshev basis is T2 = 2z²−1, T3 = 4z³−3z, T4 = 8z⁴−8z²+1, and so on. Expressing the polynomial through the Chebyshev recurrence with a baby-step giant-step split gives:
- p(z) = A(z) + T4(z)*B(z), where A = c0 + c1 T1 + c2 T2 + c3 T3 and B = c4 + c5 T1 + c6 T2 + c7 T3 (using T4+k = 2 T4 Tk − T|4−k|).

That identity is T_{4+k} = 2 T4 T_k − T_{4−k}, so:
- T5 = 2 T4 T1 − T3
- T6 = 2 T4 T2 − T2
- T7 = 2 T4 T3 − T1

Then c5 T5 + c6 T6 + c7 T7 = T4 * (2(c5 T1 + c6 T2 + c7 T3)) − (c5 T3 + c6 T2 + c7 T1).

So p = c0 + c4 T4 + [c1 T1 + c2 T2 + c3 T3] + T4 * D − [c5 T3 + c6 T2 + c7 T1], with D = 2(c5 T1 + c6 T2 + c7 T3).

Ciphertext multiplications:
- z2 = z*z
- T3 = 2 z T2 − z, one multiplication
- T4 = 2 T2² − 1, one multiplication
- T4*D, one multiplication

That is 4 ct-ct multiplications plus the alpha multiplication, against 6 in the baseline.

Plaintext multiplications: c1..c7 on T1, T2, T3 (each used twice), plus c4 on T4. I merge the coefficients first: the combined coefficient of T1 is c1−c7, of T2 is c2−c6, and of T3 is c3−c5. But plaintext-plaintext arithmetic is not allowed, so I cannot combine them directly. I instead add ciphertext terms before multiplying, which costs the same number of multiplications anyway. I therefore write E = T1*c1 + T2*c2 + T3*c3 − (T1*c7 + T2*c6 + T3*c5), plus the D part, which needs another 3 multiplications. Total: 9 plaintext multiplications (6 for E, 3 for D) plus c4*T4, which is 10. This is more than the baseline's 8, but each costs about 5–7 ms against 30 ms for a ct-ct multiplication.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext one("one");
  Plaintext c0("c0"), c1("c1"), c2("c2"), c3("c3"), c4("c4"), c5("c5"), c6("c6"), c7("c7");
  Ciphertext z = x * alpha;
  Ciphertext z2 = square(z);
  Ciphertext t2 = z2 + z2 - one;
  Ciphertext zt2 = z * t2;
  Ciphertext t3 = zt2 + zt2 - z;
  Ciphertext t2s = square(t2);
  Ciphertext t4 = t2s + t2s - one;
  Ciphertext D0 = z * c5 + t2 * c6 + t3 * c7;
  Ciphertext D = D0 + D0;
  Ciphertext low = z * c1 + t2 * c2 + t3 * c3;
  Ciphertext sub = t3 * c5 + t2 * c6 + z * c7;
  Ciphertext r = low - sub + t4 * c4 + t4 * D;
  Ciphertext y = r + c0;
  y.set_output("y");
}
```