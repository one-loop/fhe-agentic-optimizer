Idea: only even terms matter besides c1, so write the polynomial in z with a power basis and exploit the even structure.

With u = z², the polynomial is P = c0 + c1 z + c2 T2 + c4 T4 + c6 T6. The even part is a cubic in u. The Chebyshev polynomials expand as:
- T2 = 2u − 1
- T4 = 8u² − 8u + 1
- T6 = 32u³ − 48u² + 18u − 1

Even part: E(u) = (c0 − c2 + c4 − c6) + (2c2 − 8c4 + 18c6)u + (8c4 − 48c6)u² + 32c6 u³.

Computing the new coefficients needs plaintext-plaintext arithmetic, which is not allowed. So I fold them in on the ciphertext side: each plaintext coefficient multiplies a ciphertext, and I combine the terms by ciphertext linear combinations.

Ciphertext ops:
- z = x·alpha (pt mul, level 1)
- u = z·z (ct mul, level 2)
- u2 = u·u (ct mul, level 3)
- T4 = u2+u2+u2+u2 ... doubling by additions: 8u2 is three doublings, which are cheap adds.
- T6 = T2·T4-style products are avoided by using u³ = u·u2 (level 4 versus T6 at level 4, same depth).

Rather than expanding with constants, I use the T-recurrence terms directly and keep the coefficients as plaintext multiplies on cheap ciphertexts:
- T2 = 2u − 1 (the `one` subtraction is a plaintext subtract, which is fine).
- T4 = 2·T2² − 1, with T2² a ct mul.
- T6 = 2·T2·T4 − T2, with a ct mul.

That is 4 ct mul (z·z, T2², T2·T4, plus x·alpha as a pt mul) versus 5 ct mul in the baseline. Depth: z=1, u=2, T2=2, T2²=3, T4=3, T2·T4=4, T6=4, then the coefficient multiply makes 5. Levels drop from 7 to 5.

Terms: c1·z (level 2), c2·T2 (level 3), c4·T4 (level 4), c6·T6 (level 5). The terms are at different levels, so scale alignment is handled by CHEHAB.

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
  Ciphertext u = z * z;
  Ciphertext t2 = u + u - one;
  Ciphertext s = t2 * t2;
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