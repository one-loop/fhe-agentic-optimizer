Idea: fold the polynomial into an even/odd form in x and cut the multiplications.

Only c0, c1, c2, c4, c6 are kept, so the result is c0 + c1·T1 + c2·T2 + c4·T4 + c6·T6. T1 is odd. T2, T4 and T6 are even polynomials in z, so they can be written as polynomials in u = z².

- T2 = 2u − 1
- T4 = 8u² − 8u + 1
- T6 = 32u³ − 48u² + 18u − 1

So y = c0 + c1·z + P(u), where P(u) = a1·u + a2·u² + a3·u³ + a0.
The coefficients are:
- a3 = 32·c6
- a2 = 8·c4 − 48·c6
- a1 = 2·c2 − 8·c4 + 18·c6
- a0 = −c2 + c4 − c6

These coefficients are plaintext arithmetic, which is not allowed on its own because every operation needs a ciphertext operand. That cost is avoided by never forming a coefficient as a plaintext: I use the ciphertext-side structure instead. The constants 2, 8, 18, 32 and 48 can only come from additions of ciphertexts, which is expensive.

A cheaper route keeps the Chebyshev recurrence but removes the unused odd terms:
- z = x·alpha (one plaintext multiply)
- u = z² (one multiplication)
- T2 = 2u − one
- T4 = 2·T2² − one (one multiplication)
- T6 = 2·T2·T4 − T2 (one multiplication)

That is 3 ciphertext multiplications instead of 5, and depth 3 instead of 7.
The sum is c1·z + c2·T2 + c4·T4 + c6·T6 + c0, with 4 plaintext multiplies.

To save a level on z, u = z² = x²·alpha². Computing x² first and then multiplying by alpha² would need a plaintext product, which is not allowed. So I use u = z·z directly.

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
  Ciphertext result = z * c1 + t2 * c2 + t4 * c4 + t6 * c6 + c0;
  result.set_output("y");
}
```