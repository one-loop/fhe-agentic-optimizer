Idea: the result only needs even powers (c2, c4, c6, c0) plus the linear term c1·z. Write everything as a polynomial in z with T1, T2, T4, T6 expanded:

- T2 = 2z²−1
- T4 = 8z⁴−8z²+1
- T6 = 32z⁶−48z⁴+18z²−1

The even part is a polynomial in u = z²: E(u) = a0 + a1·u + a2·u² + a3·u³, so y = c1·z + E(u).

The coefficients a_i must be built from the plaintexts. Plaintext-plaintext arithmetic isn't allowed, so I fold them in via ciphertext operations, using the ciphertexts' own scaling instead.

Using x directly: z = alpha·x, so z² = (alpha·x)². Compute u = z·z (1 ct-ct mul after the z multiply), then u2 = u·u, and u3 = u2·u.

Coefficient terms: 
- a3 = 32c6, a2 = 8c4 − 48c6, a1 = 4c2 − 8c4 + 18c6 (T2 contributes 2c2·u, so a1 = 2c2 − 8c4 + 18c6), a0 = c0 − c2 + c4 − c6.

These need plaintext-plaintext arithmetic, which isn't allowed. Instead I keep the Chebyshev terms in separate ciphertexts and avoid building coefficients:

- A = c6·u (ct-pt)
- Use Horner on u with the T-polynomial coefficients held implicitly: P(u) = ((32c6·u + …)). I can't form the combined coefficients, so I use a structure of basis sums with doubling.

Trick: 2·t = t + t is allowed on ciphertexts. So build the basis polynomials as ciphertexts:
- T2 = 2u − one
- T4 = 2·T2² − one
- T6 = 2·T2·T4 − T2

That costs: z (pt mul), u (mul), T2² (mul), T2·T4 (mul), for a depth of about 4. This replaces the baseline's chain of z·t[k] products (5 ct-ct muls) with 3 ct-ct muls. The c-multiplies are 4 ct-pt, and c1·z can use z = x·alpha, so c1·alpha would be plaintext-plaintext; I use (x·alpha)·c1 instead.

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
  Ciphertext q = t2 * t2;
  Ciphertext t4 = q + q - one;
  Ciphertext p = t2 * t4;
  Ciphertext t6 = p + p - t2;
  Ciphertext result = z * c1 + t2 * c2 + t4 * c4 + t6 * c6 + c0;
  result.set_output("y");
}
```