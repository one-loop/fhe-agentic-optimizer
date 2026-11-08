The new circuit cuts the depth from 5 levels to 4. The key change is to multiply z by each coefficient first, all at level 2, and then build the Chebyshev terms from those scaled copies.

**Building blocks** (z = alpha·x)
- s = 2·T2 − 1, so T3 = z·s.
- T5 = z·(2·T4 − s), so c5·T5 = 2·T4·(c5·z) − c5·T3.
- T7 = 2·T3·T4 − z, so c7·T7 = 2·T4·(c7·T3) − c7·z.

**Combined form**

y = c0 + (c1·z − c7·z) + s·(c3·z − c5·z) + 2·T4·(s·(c7·z) + c5·z)

**Expected cost**
- 5 ciphertext-ciphertext multiplications: z², T2², s·r, s·(c7·z) and T4·q. Attempt 1 had 4, but these run at lower levels.
- 5 plaintext multiplications instead of 6.
- 4 levels instead of 5.

**Scales**
- r = c3·z − c5·z and the term c1·z − c7·z each combine two terms with the same history, so they need no alignment.
- In q = s·(c7·z) + c5·z, the two terms sit at different levels (3 and 2), so CHEHAB aligns them without a scale mismatch.

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
  Ciphertext s = t2 + t2 - one;
  Ciphertext t2s = t2 * t2;
  Ciphertext t4 = t2s + t2s - one;

  Ciphertext r = z3 - z5;
  Ciphertext a = s * r;
  Ciphertext q = s * z7 + z5;
  Ciphertext b = t4 * q;

  Ciphertext lin = z1 - z7;
  Ciphertext result = b + b + a + lin + c0;
  result.set_output("y");
}
```