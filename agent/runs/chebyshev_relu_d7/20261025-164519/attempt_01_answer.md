Idea: the output is an even/odd mix of T1, T2, T4, T6 and c0 (the odd part is only c1·z). In the monomial basis in z, p(z) = c0 + c1 z + c2(2z²−1) + c4 T4 + c6 T6, which is a polynomial of degree 6 in z. T4 and T6 are even polynomials in u = z², so the whole function is a cubic in u plus c1·z:

p = a0 + a1 u + a2 u² + a3 u³ + c1 z

with u = z², T2 = 2u−1, T4 = 8u²−8u+1, T6 = 32u³−48u²+18u−1.

Coefficients: a3 = 32c6, a2 = 8c4 − 48c6, a1 = 2c2 − 8c4 + 18c6, a0 = c0 − c2 + c4 − c6.

Plaintext-plaintext arithmetic is not allowed, so I can't form the a_i directly. I fold the scalars into ciphertext operations instead:

- z = x·alpha (1 plaintext mul), u = z·z (1 ct mul). Better: u = z², and the linear term c1·z = x·(alpha·c1) would need a plaintext product, so I use z·c1 instead.
- The a_i can't be computed from the plaintexts, so I keep the Chebyshev combination at the ciphertext level: w = c2·T2 + c4·T4 + c6·T6 is evaluated in the u basis with the Chebyshev-polynomial coefficients applied as plaintext multiplies.

Cheaper form using the even polynomial q(u) = c0 + c2 T2 + c4 T4 + c6 T6, where T2 = 2u−1, T4 = 2T2²−1, T6 = 2·T2·T4 − T2. Let s = T2 (depth 2, after z, u). Then T4 = 2s²−1 (one square), and T6 = 2·s·T4 − s, so
q = c0 + c2 s + c4 T4 + c6 (2 s T4 − s) = c0 + (c2 − c6) s + c4 T4 + 2c6 · s·T4.
The (c2 − c6) term is again plaintext arithmetic, so I split it: c2·s − c6·s.

Operations: z (pt mul), u (square), s = u+u−one, s² (square), T4 = s²+s²−one, s·T4 (ct mul). That is 3 ct muls instead of 5, and the pt muls are c1·z, c2·s, c4·T4, c6·(sT4 + sT4 − s), about 5 in total. Depth: z=1, u=2, s=2, s²=3, T4=3, sT4=4, then c6 → 5.

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
  Ciphertext s = u + u - one;
  Ciphertext s2 = square(s);
  Ciphertext t4 = s2 + s2 - one;
  Ciphertext st4 = s * t4;
  Ciphertext t6 = st4 + st4 - s;
  Ciphertext r = z * c1;
  r = r + s * c2;
  r = r + t4 * c4;
  r = r + t6 * c6;
  r = r + c0;
  r.set_output("y");
}
```