Idea: avoid the Chebyshev recurrence and evaluate the polynomial in the power basis, with a baby-step/giant-step split to cut ciphertext multiplications and plaintext multiplications.

Plaintext coefficients are inputs, so I can't precompute the monomial coefficients (plaintext-plaintext arithmetic is disallowed). Instead I keep the Chebyshev basis and use the product identities:
- T2 = 2z²−1
- T4 = 2T2²−1
- T3 = 2·z·T2 − z
- T5 = 2·T2·T3 − z
- T6 = 2·T3² − 1
- T7 = 2·T3·T4 − z

Depth: z = x·alpha (1 level), T2 (2), T3 (3), T4 (3), T5 (4), T6 (4), T7 (4). Then the coefficient multiplications add one more level, for a total of about 5.

Cost reduction: to skip the first alpha multiplication, I would need to fold alpha into the coefficients, which is not possible with plaintext-only arithmetic. So z stays, costing 1 plaintext multiplication.

Multiplication count: z², z·T2, T2², T2·T3, T3², T3·T4 gives 6 ct-ct multiplications, the same as the baseline. The saving is in depth: the lower levels make everything cheaper and it removes alignment work.

Constant handling: "2·a − 1" needs the plaintext `one`. Subtracting a plaintext is an add with encoding, which is fine.

Grouping: result = c0 + c1·z + c2·T2 + … Each term needs one plaintext multiplication (7 in total). Terms are at different levels, and CHEHAB aligns them.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext one("one");
  Ciphertext z = x * alpha;
  Ciphertext zz = square(z);
  Ciphertext t2 = zz + zz - one;
  Ciphertext zt2 = z * t2;
  Ciphertext t3 = zt2 + zt2 - z;
  Ciphertext t2s = square(t2);
  Ciphertext t4 = t2s + t2s - one;
  Ciphertext p23 = t2 * t3;
  Ciphertext t5 = p23 + p23 - z;
  Ciphertext t3s = square(t3);
  Ciphertext t6 = t3s + t3s - one;
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