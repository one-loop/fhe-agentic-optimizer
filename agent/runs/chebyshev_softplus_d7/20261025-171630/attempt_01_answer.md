Idea: the target is an even/odd split in z. With z = alpha*x, the polynomial is p(z) = c0 + c1 z + c2 T2 + c4 T4 + c6 T6.

The even part is a polynomial in u = z², and the odd part is just c1 z. T2 = 2u−1, T4 = 8u²−8u+1, T6 = 32u³−48u²+18u−1.

So p = A + B u + C u² + D u³ + c1 z, where the coefficients are combinations of the c's. The cost of computing them is plaintext-only arithmetic, which is not allowed because every operation needs a ciphertext operand. Doing that math on the ciphertext side costs plaintext multiplications.

Alternative: stay in the T-basis but cut multiplications.
- z = x*alpha (pt mul).
- T2 = 2z²−1 needs one ct mul.
- T4 = 2 T2² − 1 needs one ct mul.
- T6 = 2 T2 T4 − T2 needs one ct mul.
- Total: 3 ct-ct muls instead of 5, with depth: z (1), z² (2), T2 (2), T4 (3), T6 (4), and the coefficient multiply makes depth 5.

Scale and level notes: T2 = (z²+z²) − one, where z² is at level 2 and `one` is a plaintext, so the subtraction is fine. T4 = 2 T2² − one. T6 = 2 T2·T4 − T2, where T2 is at a shallower level than the product, which is fine.

Can we skip the alpha multiply? z² = x²·alpha², which needs alpha² and is plaintext-plaintext, so it's not allowed. Instead take xa = x*alpha, which is one pt mul and is needed anyway.

Result = c1·z + c2·T2 + c4·T4 + c6·T6 + c0, so four pt muls and one add of c0.

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
  Ciphertext z2 = square(z);
  Ciphertext t2 = z2 + z2 - one;
  Ciphertext s2 = square(t2);
  Ciphertext t4 = s2 + s2 - one;
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