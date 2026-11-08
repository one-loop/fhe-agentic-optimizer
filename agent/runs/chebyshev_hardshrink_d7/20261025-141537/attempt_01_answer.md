Idea: the result is an odd polynomial in z, so y = z·P(z²) with P of degree 3 in w = z².

Since z = x·alpha, we have z² = x²·alpha², and alpha² is a plaintext-plaintext product, which is not allowed. So a different route is needed.

Odd Chebyshev expansion in z:
- T1 = z
- T3 = 4z³ − 3z
- T5 = 16z⁵ − 20z³ + 5z
- T7 = 64z⁷ − 112z⁵ + 56z³ − 7z

These give y = a1·z + a3·z³ + a5·z⁵ + a7·z⁷, where the a's are combinations of c's and integers. Building them needs numeric constants, which can only be made by additions, such as 7z = z+z+z+..., and that is expensive on plaintext coefficients.

Alternative: write y = z·(b0 + b1 w + b2 w² + b3 w³) with w = z², where each b is a combination of c's built from plaintext additions. Plaintext-plaintext arithmetic is not allowed, but I can multiply the ciphertext by the plaintexts first and then combine.

A cheaper approach is to keep the T_k in the ciphertext domain, using w = z², T3 = z(4w−3), and so on:
- z = x·alpha (1 plaintext mul)
- w = z² (1 mul)
- T3 = z·(w+w+w+w−3·one...) needs plaintext `one` as an operand. Subtracting `one` from a ciphertext is allowed because a ciphertext is involved. So 4w−3 = (w+w+w+w) − (one+one+one) fails, since that sum is plaintext-plaintext. Instead use T3 = 2z·T2 − z with T2 = 2w − one.

Plan (depth is what matters):
- z = x·alpha
- w = z·z
- T2 = 2w − one
- T3 = 2·z·T2 − z
- T4 = 2·T2² − one (T4 = 2T2² − 1)
- T5 = 2·T2·T3 − z
- T7 = 2·T4·T3 − z (since T7 = 2T4T3 − T1)

Depths: z=1, w=2, T2=2, T3=3, T4=3, T5=4, T7=4. Multiplying by the coefficients adds one more level, giving depth 5 in total instead of 8.

The multiplications are z·T2, T2², T2·T3 and T4·T3, plus w, for 5 ct-ct multiplications. The constants multiply cheaply by 2 via addition. The final sum is c1·z + c3·T3 + c5·T5 + c7·T7, which needs 4 plaintext multiplications. Total: 5 ct-ct, 5 plaintext multiplications.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext one("one");
  Plaintext c1("c1");
  Plaintext c3("c3");
  Plaintext c5("c5");
  Plaintext c7("c7");
  Ciphertext z = x * alpha;
  Ciphertext w = z * z;
  Ciphertext t2 = w + w - one;
  Ciphertext zt2 = z * t2;
  Ciphertext t3 = zt2 + zt2 - z;
  Ciphertext t2sq = t2 * t2;
  Ciphertext t4 = t2sq + t2sq - one;
  Ciphertext p5 = t2 * t3;
  Ciphertext t5 = p5 + p5 - z;
  Ciphertext p7 = t4 * t3;
  Ciphertext t7 = p7 + p7 - z;
  Ciphertext result = z * c1;
  result = result + t3 * c3;
  result = result + t5 * c5;
  result = result + t7 * c7;
  result.set_output("y");
}
```