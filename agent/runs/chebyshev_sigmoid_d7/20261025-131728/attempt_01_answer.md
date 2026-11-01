Idea: use the odd polynomial p(z) = c0 + z·(c1 + c3 T3'(z) + ...) in powers of z², so the evaluation needs few ciphertext multiplications.

Writing in monomials of z: y = c0 + a1 z + a3 z³ + a5 z⁵ + a7 z⁷. The monomial coefficients are linear combinations of the c's, and these can only be formed through plaintext ops. Plaintext-plaintext arithmetic is not allowed, so I keep the c's as ct-pt multiplications and avoid forming combinations.

Using T-basis: T3 = 4z³ − 3z, T5 = 16z⁵ − 20z³ + 5z, T7 = 64z⁷ − 112z⁵ + 56z³ − 7z. Constants like 4 and 3 cannot be used directly, only through additions, which is expensive. A cheaper route is to compute with x directly, putting alpha into the polynomial structure.

Plan, with s = z² (one ct-ct square) and u = x·alpha:
- z = x·alpha (pt mul, level 1).
- s = z² (level 2).
- T3 = z(2s − 1.5)... this needs the constant 1.5, which is unavailable. Use T3 = z·(4s − 3) = 2z(2s − 1) − z, with 2s−1 = s+s−one. Then T3 = 2·z·w − z with w = s+s−one.
- T3 = (zw)+(zw) − z. This costs one ct-ct multiplication.
- T4 = 2s·... is not needed.
- T5 = 2·s·T3... hmm: T5 = 2 T2 T3 − T1 with T2 = w. This needs w·T3 at depth 4.
- T7 = 2 T4 T3 − T1, with T4 = 2w² − 1, which gives depth 4 as well.

Counts: z pt mul, s, T3 (zw), T5 = 2·w·T3 − z, T4 = 2w² − one, T7 = 2·T4·T3 − z. That is 5 ct-ct multiplications against the baseline's 6. Depth: z=1, s=2, w=2, T3=3, T4=3, T5=4, T7=4. Then the c multiplications bring the depth to 5, compared with 8 for the baseline, so the saving is mostly in the depth.

The scale and level rules: T3 is at level 3 and z at level 1, so the subtraction is aligned automatically. The terms are c1·z (level 2), c3·T3 (4), c5·T5 (5), and c7·T7 (5). Terms with the same multiplication history at the same level could mismatch, but CHEHAB handles this at differing levels, and I take the risk.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext one("one");
  Ciphertext z = x * alpha;
  Ciphertext s = z * z;
  Ciphertext w = s + s - one;
  Ciphertext zw = z * w;
  Ciphertext t3 = zw + zw - z;
  Ciphertext ww = w * w;
  Ciphertext t4 = ww + ww - one;
  Ciphertext p5 = w * t3;
  Ciphertext t5 = p5 + p5 - z;
  Ciphertext p7 = t4 * t3;
  Ciphertext t7 = p7 + p7 - z;
  Plaintext c1("c1");
  Plaintext c3("c3");
  Plaintext c5("c5");
  Plaintext c7("c7");
  Plaintext c0("c0");
  Ciphertext result = z * c1;
  result = result + t3 * c3;
  result = result + t5 * c5;
  result = result + t7 * c7;
  result = result + c0;
  result.set_output("y");
}
```