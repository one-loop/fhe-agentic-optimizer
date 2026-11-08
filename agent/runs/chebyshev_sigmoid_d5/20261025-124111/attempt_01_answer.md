Idea: the odd polynomial p(z) = c1 z + c3 T3 + c5 T5 + c0 can be written in terms of u = z², giving z·(a1 + a3 u + a5 u²) + c0. Since z = alpha·x, I fold alpha into the coefficients, and the work needed is:

- T3 = 4z³ − 3z and T5 = 16z⁵ − 20z³ + 5z. The coefficients a1 = c1 − 3c3 + 5c5, a3 = 4c3 − 20c5 and a5 = 16c5 are plaintext-only combinations. Plaintext-plaintext arithmetic is not allowed, so I cannot compute them directly.
- Instead I expand with ciphertext operations. Using x directly would avoid the first multiply, but the coefficients would need alpha powers, which is again plaintext arithmetic.

Evaluation plan, keeping multiplications few:
- z = x·alpha (pt mul, level 1).
- u = z·z (ct mul, level 2).
- T3 = z·(4u − 3) is awkward without constants, so I use the T-recurrence instead: T3 = 2·z·T2 − z, with T2 = 2u − one.
- Using powers: p = c1 z + c3 T3 + c5 T5, with T3 = 4z³ − 3z and T5 = 16z⁵ − 20z³ + 5z.
- Coefficients need plaintext multiplies, so I group the terms: z·[c1 + c3(4u − 3) + c5(16u² − 20u + 5)]. I can build this as (T3/z) = 4u − 3 = 2·T2 − 1 = (T2 + T2 + T2... ), using only additions of ciphertexts and `one`:
  - T2 = (u+u) − one
  - A = T3/z = 2·T2 + T2... check: 4u − 3 = 2(2u − 1) − 1 = T2+T2−one. ✓.
  - B = T5/z = 16u² − 20u + 5 = 2·T2·A − T2... check via the recurrence T5 = 2zT4 − T3, T4 = 2z·T3 − T2 (messy). Instead T5/z = 2·T4 − A·... Direct: with T2 = 2u−1, T2² = 4u² − 4u + 1, so 4·T2² = 16u² − 16u + 4, and B = 4·T2² − 4u + 1 = 4·T2² − 2(T2+one) + 1 = 4T2² − 2T2 − one. Using a square of T2 costs one ct mul.
- Then result = z·(c1 + c3·A + c5·B) + c0. Levels: z (1), u (2), T2 (2), T2² (3), the inner sum with plaintext multiplies (4), times z (5). That is 3 ct mults plus 4 pt mults, versus the baseline's 4 and 4.

To cut further, I multiply c3 into the sum before the final product, which gives c3·A at level 3 and c5·B at level 4, so mismatched levels are aligned for free.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext one("one");
  Plaintext c0("c0");
  Plaintext c1("c1");
  Plaintext c3("c3");
  Plaintext c5("c5");
  Ciphertext z = x * alpha;
  Ciphertext u = square(z);
  Ciphertext t2 = u + u - one;
  Ciphertext a = t2 + t2 - one;
  Ciphertext q = square(t2);
  Ciphertext q2 = q + q;
  Ciphertext q4 = q2 + q2;
  Ciphertext b = q4 - t2 - t2 - one;
  Ciphertext inner = a * c3 + b * c5;
  Ciphertext s = inner + c1;
  Ciphertext result = s * z + c0;
  result.set_output("y");
}
```