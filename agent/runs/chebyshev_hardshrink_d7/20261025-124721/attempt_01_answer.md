Idea: the output is an odd polynomial in z, so with w = z² we have y = z·(a1 + a3·w + a5·w² + a7·w³), where the a's come from the Chebyshev coefficients. The a's would need plaintext-plaintext arithmetic, which is not allowed. So I keep the coefficients as plaintext multiplies and restructure the evaluation instead.

T1 = z
T3 = 4z³ - 3z
T5 = 16z⁵ - 20z³ + 5z
T7 = 64z⁷ - 112z⁵ + 56z³ - 7z

Integer multiples are not available as constants, but they can be built from additions. Plaintext scalar products with c_k are cheap (~5 ms), so I would rather fold the powers into a few ct-pt multiplies.

Since no plaintext-plaintext arithmetic is allowed, I express y = c1·z + c3·T3 + c5·T5 + c7·T7 in terms of the monomials z, z³, z⁵, z⁷:
- y = z·(c1·1 + c3(4w-3) + c5(16w²-20w+5) + c7(64w³-112w²+56w-7)), with w = z².
- Group by powers of w, using additions and the c's as plaintexts: P(w) = A + B·w + C·w² + D·w³. The coefficients A, B, C, D would be combinations of c's with integer multipliers, which I build with ciphertext additions after multiplying each c by a ciphertext (for example c3·w).

Cheaper plan: compute z = x·alpha (1 pt mul), w = z·z (1 ct mul), and then Q = c7·w (pt mul).
- Let u = 4w - 3, so T3 = z·u.
- Instead use the recurrence in w: T5 = T3·(4w-2) - T1 and T7 = T5·(4w-2) - T3, which keeps the depth lower than the baseline.

Define s = 2w - 1, so T2 = s, and then v = 4w - 2 = s + s. Compute T3 = z·v - z = z·(v - 1), where the "-1" comes from one as a plaintext. Because one is a plaintext, I can subtract it from a ciphertext.
- T3 = z·(v - one) (this works as v - one = 4w - 3)
- T5 = T3·v - z
- T7 = T5·v - T3

This uses ct-ct multiplies: w, z·(v-one), T3·v, T5·v, which is 4 multiplies instead of 6. Then four plaintext multiplies (c1, c3, c5, c7) are needed. The depth is about 5 plus the final plaintext multiply.

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
  Ciphertext w2 = w + w;
  Ciphertext v = w2 + w2 - one - one;
  Ciphertext v1 = v - one;
  Ciphertext t3 = z * v1;
  Ciphertext t5 = t3 * v - z;
  Ciphertext t7 = t5 * v - t3;
  Ciphertext result = z * c1 + t3 * c3 + t5 * c5 + t7 * c7;
  result.set_output("y");
}
```