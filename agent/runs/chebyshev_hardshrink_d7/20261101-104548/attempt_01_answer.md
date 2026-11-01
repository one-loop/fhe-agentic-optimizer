Degree 5 → 4 ct-ct multiplications and depth 8 → 5 by factoring the odd Chebyshev terms through T3, using T5 = 2·T2·T3 − T1 and T7 = 2·T4·T3 − T1:

y = T3·(c3 + 2·c5·T2 + 2·c7·T4) + c1·z − c5·z − c7·z

**Ciphertext-ciphertext multiplications (4):**
- z² = z·z, which gives T2 = 2z² − 1
- T3 = z·(2·T2 − 1)
- T4 = 2·T2² − 1
- one final product T3·inner

**Plaintext multiplications:** all coefficients enter as scalar plaintext multiplies or adds. No plaintext-plaintext arithmetic is needed.

**Depth:** T3·inner sits at level 5. The three z-terms sit at level 2 and share one multiplication pattern, so they can be added together.

**Scales inside `inner`:** c5·T2 is at level 3 and c7·T4 is at level 4. Different levels are always fine.

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
  Ciphertext t3 = z * (t2 + t2 - one);
  Ciphertext t2sq = t2 * t2;
  Ciphertext t4 = t2sq + t2sq - one;

  Ciphertext a = t2 * c5;
  Ciphertext b = t4 * c7;
  Ciphertext inner = a + a + b + b + c3;

  Ciphertext main_part = t3 * inner;
  Ciphertext lin = z * c1 - z * c5 - z * c7;

  Ciphertext result = main_part + lin;
  result.set_output("y");
}
```