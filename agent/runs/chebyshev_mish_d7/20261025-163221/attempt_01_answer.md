Idea: avoid the Chebyshev recurrence and evaluate the polynomial in the monomial basis on z, using fewer ct-ct multiplications and plaintext multiplications. However, the coefficients are runtime plaintext inputs, so converting to the monomial basis would need plaintext-plaintext arithmetic, which is not allowed. I stay in the Chebyshev basis and use the doubling identities instead:

- T2 = 2z²-1
- T4 = 2T2²-1
- T3 = 2·z·T2 - z
- T5 = 2·T2·T3 - z
- T6 = 2·T3²-1
- T7 = 2·T3·T4 - z

Depth: z = x*alpha (1 level), z² (2), T2 (2), T3 (3), T4 (3), T5/T6/T7 (4), then the coefficient multiply makes 5. That is shallower than the baseline's 8.

Ciphertext multiplications: z², z·T2, T2², T2·T3, T3², T3·T4 = 6, the same count as the baseline but with less depth, so later operations are cheaper and there are fewer alignments. To save a level and the first plaintext multiply, I can fold alpha in: z = x*alpha is a ct-pt multiply that is needed anyway.

Only the sums and constants remain. The "one" subtraction is a ct-pt operation (a plaintext subtract), which is allowed. The terms c_k·T_k each use one plaintext multiply, and I fold the factor 2 into the additions before the multiply (t + t).

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext one("one");
  Plaintext c0("c0"), c1("c1"), c2("c2"), c3("c3"), c4("c4"), c5("c5"), c6("c6"), c7("c7");
  Ciphertext z = x * alpha;
  Ciphertext z2 = square(z);
  Ciphertext T2 = z2 + z2 - one;
  Ciphertext a = z * T2;
  Ciphertext T3 = a + a - z;
  Ciphertext b = square(T2);
  Ciphertext T4 = b + b - one;
  Ciphertext c = T2 * T3;
  Ciphertext T5 = c + c - z;
  Ciphertext d = square(T3);
  Ciphertext T6 = d + d - one;
  Ciphertext e = T3 * T4;
  Ciphertext T7 = e + e - z;
  Ciphertext result = z * c1;
  result = result + T2 * c2;
  result = result + T3 * c3;
  result = result + T4 * c4;
  result = result + T5 * c5;
  result = result + T6 * c6;
  result = result + T7 * c7;
  result = result + c0;
  result.set_output("y");
}
```