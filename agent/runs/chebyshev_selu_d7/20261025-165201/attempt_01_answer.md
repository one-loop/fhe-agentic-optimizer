The baseline computes every T_k through ciphertext multiplications and then multiplies each one by its coefficient. I switch to the monomial basis with a Paterson-Stockmeyer-style evaluation.

Since the coefficients are plaintext inputs, I cannot fold them into constants at compile time. Converting from the Chebyshev basis to the monomial basis would need plaintext-plaintext arithmetic, which is not allowed. So I keep the Chebyshev basis but cut the multiplication count with the product identities:
- T2 = 2z² − 1
- T3 = 2·T1·T2 − T1
- T4 = 2·T2² − 1
- T5 = 2·T2·T3 − T1
- T6 = 2·T3² − 1
- T7 = 2·T3·T4 − T1

This needs these ciphertext multiplications: z² (depth 1), T2·T1, T2², T3·T2, T3², T3·T4. The count is 6, same as the baseline, but the depth drops.

The depths, counting z = x·alpha as depth 1 (a plaintext multiply):
- z: 1
- z²: 2, so T2: 2
- T3 = 2·z·T2 − z: 3
- T4 = 2·T2² − 1: 3
- T5 = 2·T2·T3 − z: 4
- T6 = 2·T3² − 1: 4
- T7 = 2·T3·T4 − z: 4

Multiplying by the coefficients adds one level, so the total is 5 instead of 8. The final levels are cheaper because the baseline's deep chain disappears.

To save more, I fold alpha into x differently. Squaring x directly needs a scale of alpha², which is a plaintext multiply anyway, so I keep z = x·alpha.

The `2*a` terms are written as `a + a`. The term `- one` uses a plaintext, which is allowed because the other operand is a ciphertext.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext one("one");
  Ciphertext z = x * alpha;
  Ciphertext z2 = square(z);
  Ciphertext t2 = z2 + z2 - one;
  Ciphertext p3 = z * t2;
  Ciphertext t3 = p3 + p3 - z;
  Ciphertext p4 = square(t2);
  Ciphertext t4 = p4 + p4 - one;
  Ciphertext p5 = t2 * t3;
  Ciphertext t5 = p5 + p5 - z;
  Ciphertext p6 = square(t3);
  Ciphertext t6 = p6 + p6 - one;
  Ciphertext p7 = t3 * t4;
  Ciphertext t7 = p7 + p7 - z;

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