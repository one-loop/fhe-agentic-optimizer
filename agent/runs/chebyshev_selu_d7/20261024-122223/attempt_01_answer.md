Idea: the polynomial is evaluated in the power basis with a minimal number of ciphertext multiplications, since the coefficients are plaintext inputs and can't be combined at compile time. Plaintext-plaintext arithmetic is not allowed, so the Chebyshev-to-monomial conversion can't be done on the coefficients. Instead I keep the Chebyshev structure but use the doubling identities, which need only the constant `one`:

- T2 = 2z² − 1
- T4 = 2T2² − 1
- T3 = 2·z·T2 − z
- T5 = 2·T2·T3 − z
- T6 = 2·T3² − 1
- T7 = 2·T3·T4 − z

These products are shallower and use fewer ciphertext-ciphertext multiplies than the linear chain.

Depth: z = x·alpha takes 1 level. T2 takes 2, T3 and T4 take 3, and T5, T6, T7 take 4. Multiplying by the coefficients then adds a level, so the total is 5 against 8 for the baseline.

Ciphertext multiplications: z² (1), z·T2 (2), T2² (3), T2·T3 (4), T3² (5), T3·T4 (6). That is still 6, but at lower depth.

To save more, I fold alpha into the first step: z = x·alpha costs a plaintext multiply, and I keep it. The additions of terms at different levels are aligned by CHEHAB.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext one("one");
  Ciphertext z = x * alpha;
  Ciphertext z2 = square(z);
  Ciphertext t2 = z2 + z2 - one;
  Ciphertext zt2 = z * t2;
  Ciphertext t3 = zt2 + zt2 - z;
  Ciphertext t2sq = square(t2);
  Ciphertext t4 = t2sq + t2sq - one;
  Ciphertext p25 = t2 * t3;
  Ciphertext t5 = p25 + p25 - z;
  Ciphertext t3sq = square(t3);
  Ciphertext t6 = t3sq + t3sq - one;
  Ciphertext p34 = t3 * t4;
  Ciphertext t7 = p34 + p34 - z;

  Plaintext c0("c0"), c1("c1"), c2("c2"), c3("c3"), c4("c4"), c5("c5"), c6("c6"), c7("c7");
  Ciphertext r = z * c1;
  r = r + t2 * c2;
  r = r + t3 * c3;
  r = r + t4 * c4;
  r = r + t5 * c5;
  r = r + t6 * c6;
  r = r + t7 * c7;
  r = r + c0;
  r.set_output("y");
}
```