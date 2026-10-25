Idea: the result is an odd polynomial in z, so y = z·P(z²) with P a cubic in w = z². Writing everything in terms of x directly removes the chain of Chebyshev recurrences.

Odd Chebyshev terms:
- T1 = z
- T3 = 4z³ − 3z
- T5 = 16z⁵ − 20z³ + 5z
- T7 = 64z⁷ − 112z⁵ + 56z³ − 7z

The coefficients are plaintext inputs, so the combined polynomial coefficients (a1, a3, a5, a7) must be built from c1, c3, c5, c7 using only plaintext-ciphertext operations. That needs numeric constants such as 4 or 5, which are not allowed, and plaintext-plaintext arithmetic is also not allowed. The constants can be built by repeated addition on ciphertexts, but that costs extra levels.

Alternative that avoids the constants: keep the recurrence, but use the product identities with fewer multiplications and less depth.
- z2 = z·z (depth 2 including the alpha multiply)
- T3 = 2·z·T2 − z, T4 = 2·T2² − 1
- T5 = 2·T2·T3 − z (T2·T3 gives T5 + T1)
- T7 = 2·T3·T4 − T1 (T3·T4 gives T7 + T1)
- T6 is not needed.

Depth: z has depth 1 (the multiply by alpha), T2 depth 2, T3 depth 3, T4 depth 3, T5 depth 4, T7 depth 4. The coefficient multiplies then bring the total to depth 5 (versus 8 in the baseline).

Cutting depth further: fold alpha into the first step. T2 = 2z² − 1 with z² = (x·alpha)², which still needs a depth-1 multiply, so it is kept as is.

Multiplications: z (pt), z², T2·z, T2², T2·T3, T3·T4, plus 4 coefficient multiplies. That is 5 ct-ct multiplies against 6 in the baseline, with less depth.

Scale handling: the terms are added at different levels (c1·z at a shallow level, the others deeper), which CHEHAB aligns automatically. In the T5 and T7 product forms, t5 = p + p − z, where z has a lower depth, which is fine.

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
  Ciphertext z2 = z * z;
  Ciphertext t2 = z2 + z2 - one;
  Ciphertext zt2 = z * t2;
  Ciphertext t3 = zt2 + zt2 - z;
  Ciphertext t2sq = t2 * t2;
  Ciphertext t4 = t2sq + t2sq - one;
  Ciphertext p5 = t2 * t3;
  Ciphertext t5 = p5 + p5 - z;
  Ciphertext p7 = t3 * t4;
  Ciphertext t7 = p7 + p7 - z;
  Ciphertext result = z * c1 + t3 * c3 + t5 * c5 + t7 * c7;
  result.set_output("y");
}
```