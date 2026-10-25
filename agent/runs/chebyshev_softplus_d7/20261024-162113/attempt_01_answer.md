Idea: evaluate only the needed even/odd terms. The polynomial is c0 + c1 z + c2 T2 + c4 T4 + c6 T6, and T2, T4 and T6 are all polynomials in z². Writing u = z² gives an even polynomial in u:

- T2 = 2u − 1
- T4 = 8u² − 8u + 1
- T6 = 32u³ − 48u² + 18u − 1

So y = c0 + c1 z + c2(2u−1) + c4(8u²−8u+1) + c6(32u³−48u²+18u−1) = a0 + a1 u + a2 u² + a3 u³ + c1 z.

The a_i would need plaintext arithmetic, which is not allowed. Plaintext-only operations are not allowed, but I can fold them into ciphertext operations. Each coefficient has to be applied as a ciphertext×plaintext multiply, and I can't build the combined constants a_i directly.

Cheaper structure: let the plaintext multiplies act on ciphertext terms.

- z = x·alpha (one pt mul).
- u = z·z (one ct mul). To save a level, use u = square(z).
- Compute the plaintext-scaled terms first: c6·u, and so on.

Using Horner in u with scalar multiplies:

- y = c0 + c1 z + u·(b1 + u·(b2 + u·b3)).
- I can't compute b's from the plaintexts directly, so I use T-polynomials built with ct additions: T2 = 2u − 1, which needs only adds and subtracting `one` (a plaintext add).
- T4 = 2·T2² − 1, so one square is needed.
- T6 = 2·T2·T4 − T2, so one more ct mul.

Total: z (pt mul), u (square), T2 (adds), T4 (square), T6 (ct mul), then 4 pt muls. That is 3 ct mults instead of 5. Depth: z is level 1, u is 2, T2 is 2, T4 is 3, T6 is 4, and the final c6 multiply makes 5. The c1·z term can be written as x·(alpha·c1), but that would be plaintext-plaintext arithmetic, so I keep c1·z as is.

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
  Ciphertext t4s = square(t2);
  Ciphertext t4 = t4s + t4s - one;
  Ciphertext t6p = t2 * t4;
  Ciphertext t6 = t6p + t6p - t2;
  Ciphertext result = z * c1;
  result = result + t2 * c2;
  result = result + t4 * c4;
  result = result + t6 * c6;
  result = result + c0;
  result.set_output("y");
}
```