Idea: evaluate the polynomial in the power basis with the coefficients folded in, which cuts the number of ciphertext multiplications and plaintext multiplications.

Problem: the coefficients are only available as plaintext inputs, so I cannot convert the Chebyshev basis to the power basis at compile time. Plaintext-plaintext arithmetic is also disallowed. So I keep the Chebyshev basis and reduce the multiplication count instead.

Plan, using z = x*alpha (one pt mult, depth 1):
- z2 = z*z (depth 2), T2 = 2 z2 - 1.
- T3 = 2 z T2 - z, T4 = 2 T2² - 1.
- Use the product identities T_{m+n} = 2 T_m T_n - T_{|m-n|}:
  - T4 = 2 T2*T2 - 1 (depth 3)
  - T3 = 2 z*T2 - z (depth 3)
  - T5 = 2 T2*T3 - z (depth 4)
  - T6 = 2 T3*T3 - 1 (depth 4)
  - T7 = 2 T3*T4 - z (depth 4)
- Ciphertext multiplications: z², T2·T2, z·T2, T2·T3, T3·T3, T3·T4, which is 6, the same as the baseline, but depth drops to 4 for the T's plus 1 for the coefficient multiply.

To save more, I fold the coefficient multiplications in earlier. For example c7*T7 = 2*(c7*T3)*T4 - c7*z, where c7*T3 is computed at a shallower level. That saves depth but adds plaintext multiplications, so I keep it simple: the lower depth makes the later operations cheaper, and the scale handling stays safe.

The "one" constant is needed in T2, T4 and T6. Each use is a plaintext subtraction, which is fine.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext one("one");
  Ciphertext z = x * alpha;
  Ciphertext z2 = square(z);
  Ciphertext t2 = z2 + z2 - one;
  Ciphertext z_t2 = z * t2;
  Ciphertext t3 = z_t2 + z_t2 - z;
  Ciphertext t2sq = square(t2);
  Ciphertext t4 = t2sq + t2sq - one;
  Ciphertext a5 = t2 * t3;
  Ciphertext t5 = a5 + a5 - z;
  Ciphertext s3 = square(t3);
  Ciphertext t6 = s3 + s3 - one;
  Ciphertext a7 = t3 * t4;
  Ciphertext t7 = a7 + a7 - z;

  Plaintext c0("c0"), c1("c1"), c2("c2"), c3("c3"), c4("c4"), c5("c5"), c6("c6"), c7("c7");
  Ciphertext result = z * c1;
  result = result + t2 * c2;
  result = result + t3 * c3;
  result = result + t4 * c4;
  result = result + t5 * c5;
  result = result + t6 * c6;
  result = result + t7 * c7;
  result = result + c0;
  result.set_output("y");
}
```