Idea: the polynomial is even/odd mixed: c0 + c1 z + c2 T2 + c4 T4 + c6 T6. Expand in the monomial basis to reduce multiplications: p(z) = a0 + a1 z + a2 z² + a4 z⁴ + a6 z⁶ (T2, T4, T6 are even, and only c1 is odd).

Writing u = z², the even part is e(u) = a0 + a2 u + a4 u² + a6 u³, and y = e(u) + a1 z.

The coefficients a_i are linear combinations of the c's, but computing them needs plaintext-plaintext arithmetic, which is not allowed. Instead I fold the coefficients in by using ciphertext operations.

Computing the monomial coefficients is not possible directly. So I use the Chebyshev form in terms of u:
- T2 = 2u - 1
- T4 = 8u² - 8u + 1
- T6 = 32u³ - 48u² + 18u - 1

Without plaintext arithmetic, the scalar coefficients cannot be combined. A plaintext times a ciphertext is cheap (~5 ms), though, so the plan is:
- z = x*alpha (one pt mul). To save a level and a multiplication, use u = z² = (x*alpha)². Compute w = x*alpha, then u = w*w.
- u2 = u*u, u3 = u2*u (ct-ct muls).
- Gather terms by degree with plaintext products, using only the scalars given. The combination coefficients for u^k need c's multiplied together, which isn't possible. Alternative: use T-polynomials built from u, such as T4 = 2 T2² - 1 and T6 = 2 T2 T4 - T2.
  - T2 = (u+u) - one
  - T2sq = T2*T2
  - T4 = (T2sq + T2sq) - one
  - Then c4*T4 and c6*T6 where T6 = 2*T2*T4 - T2: c6*T6 = 2*(T2*(c6*T4)) - c6*T2, with c6*T4 computed first.

Multiplication count: w (pt), u (ct), T2² (ct), T2*(c6*T4) (ct) — 3 ct-ct muls instead of 5. The pt muls are c1*w, c2*T2, c4*T4, c6*T4, c6*T2, so 5 plus the alpha one. The result is:

y = c1*w + (c2-c6)·T2 ... where c2*T2 - c6*T2 is kept as two terms, plus c4*T4 + 2*M + c0, where M = T2*(c6*T4).

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
  Ciphertext w = x * alpha;
  Ciphertext u = w * w;
  Ciphertext t2 = u + u - one;
  Ciphertext s = t2 * t2;
  Ciphertext t4 = s + s - one;
  Ciphertext a = t4 * c6;
  Ciphertext m = t2 * a;
  Ciphertext r = m + m;
  r = r + t4 * c4;
  r = r + t2 * c2;
  r = r - t2 * c6;
  r = r + w * c1;
  r = r + c0;
  r.set_output("y");
}
```