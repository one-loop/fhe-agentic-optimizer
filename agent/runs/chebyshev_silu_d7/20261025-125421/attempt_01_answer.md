Idea: the polynomial is even/odd mixed, p(z) = c0 + c1 z + c2 T2 + c4 T4 + c6 T6. In the power basis, with u = z², this becomes an even polynomial in z (degree 6) plus the linear term c1 z.

Even part: E(u) = a0 + a2 u + a4 u² + a6 u³, with u = z².
- T2 = 2u − 1
- T4 = 8u² − 8u + 1
- T6 = 32u³ − 48u² + 18u − 1

So the coefficients are:
- a6 = 32 c6
- a4 = 8 c4 − 48 c6
- a2 = 2 c2 − 8 c4 + 18 c6
- a0 = c0 − c2 + c4 − c6

These are plaintext-only combinations, so I can't compute them as plaintext-plaintext arithmetic. Every operation needs a ciphertext operand. To avoid that, I fold the coefficient work into ciphertext operations: multiply the ciphertext by each coefficient plaintext and combine the results, which keeps every operation ciphertext-based.

The plan is to evaluate in terms of u, with the coefficient multiplications done on ciphertexts:
- z = x·alpha (one ct-pt multiply).
- u = z² (one square).
- u² = u·u (one square).
- Even part: c2·T2 + c4·T4 + c6·T6 + c0, written with the Chebyshev recurrences on u.
  - T2 = 2u − 1 and T4 = 2T2² − 1 are computed from ciphertexts.
  - T6 = 2·T2·T4 − T2 costs one ct-ct multiply.
- That gives 3 ct-ct multiplies for the squares and T6, plus 1 for z.

A cheaper route: compute T2 = 2u − 1, T4 = 2T2² − 1 (square), and T6 = 2·T2·T4 − T2 (multiply). Total multiplies:
- z: 1 ct-pt
- u: square
- T4: square
- T6: ct-ct multiply
- 4 coefficient ct-pt multiplies (c1, c2, c4, c6)

That is 3 ct-ct against 5 in the baseline, and depth is lower. T6 sits at depth 4 (z=1, u=2, T4=3, T6=4), so the c6 multiply takes it to depth 5. The c1·z term can be shifted to a shallower level.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext one("one");
  Plaintext c0("c0");
  Plaintext c1("c1");
  Plaintext c2("c2");
  Plaintext c4("c4");
  Plaintext c6("c6");
  Ciphertext z = x * alpha;
  Ciphertext u = square(z);
  Ciphertext t2 = u + u - one;
  Ciphertext s = square(t2);
  Ciphertext t4 = s + s - one;
  Ciphertext m = t2 * t4;
  Ciphertext t6 = m + m - t2;
  Ciphertext result = z * c1;
  result = result + t2 * c2;
  result = result + t4 * c4;
  result = result + t6 * c6;
  result = result + c0;
  result.set_output("y");
}
```