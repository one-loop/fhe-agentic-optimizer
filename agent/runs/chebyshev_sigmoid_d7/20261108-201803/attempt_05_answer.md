Attempt 3 already has the minimum of 5 ciphertext-ciphertext and 5 plaintext multiplications at depth 4. Its three level-2 to higher alignments are inherent to this approach, because c1 and c3 can only enter through level-2 terms. So this attempt strips the remaining overhead.

1. **Drop s = 2·T2 − 1.** Use s·r = 2·T2·r − r and s·(c7·z) = 2·T2·(c7·z) − c7·z. Ciphertext-ciphertext products are taken only with T2.
2. **Drop T4 = 2·T2² − 1.** Use 2·T4·q = 4·T2²·q − 2·q, so "one" appears only once, in T2.
3. **Regroup by level.**
   - Level 3: q = 2p + c5·z − c7·z, where p = T2·(c7·z).
   - Level 4: 4·(T2²·q).
   - Level 3: 2·T2·r − 4p, with r = c3·z − c5·z, where all terms have the same history.
   - Level 2: c1·z + c7·z − c3·z − c5·z, plus c0.

This removes one plaintext add and its encoding, the plaintext add at level 3, and several ciphertext adds. The multiplication counts and depth stay the same.

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
  Plaintext c7("c7");

  Ciphertext z = x * alpha;
  Ciphertext z1 = z * c1;
  Ciphertext z3 = z * c3;
  Ciphertext z5 = z * c5;
  Ciphertext z7 = z * c7;

  Ciphertext zz = z * z;
  Ciphertext t2 = zz + zz - one;
  Ciphertext t2s = t2 * t2;

  Ciphertext p = t2 * z7;
  Ciphertext q = p + p + (z5 - z7);
  Ciphertext g = t2s * q;
  Ciphertext g2 = g + g;
  Ciphertext g4 = g2 + g2;

  Ciphertext r = z3 - z5;
  Ciphertext a = t2 * r;
  Ciphertext p2 = p + p;
  Ciphertext mid = a + a - p2 - p2;

  Ciphertext lin = z1 + z7 - z3 - z5;
  Ciphertext result = g4 + mid + lin + c0;
  result.set_output("y");
}
```