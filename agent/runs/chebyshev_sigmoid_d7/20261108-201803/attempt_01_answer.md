Using product identities of Chebyshev polynomials cuts both the multiplication count and the depth.

**Identities used**
- T2 = 2z² − 1
- T3 = 2·z·T2 − z
- T4 = 2·T2² − 1
- T5 = 2·T2·T3 − z
- T7 = 2·T3·T4 − z

**Rewritten output**

y = c0 + c1·z − c5·z − c7·z + T3·(c3 + 2·(c5·T2 + c7·T4))

**Expected cost**
- 4 ciphertext-ciphertext multiplications: z², z·T2, T2² and the final T3·v. The baseline uses 6.
- Multiplicative depth 5, down from 8:

| Value | Level |
|---|---|
| z | 1 |
| T2 | 2 |
| T3 | 3 |
| T4 | 3 |
| v | 4 |
| y | 5 |

**Scales**
- Terms that meet at the same level have the same multiplication history: the three z·c terms.
- All other sums combine operands at different levels, so CHEHAB aligns them without a scale mismatch.

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
  Ciphertext zz = z * z;
  Ciphertext t2 = zz + zz - one;
  Ciphertext zt2 = z * t2;
  Ciphertext t3 = zt2 + zt2 - z;
  Ciphertext t2s = t2 * t2;
  Ciphertext t4 = t2s + t2s - one;

  Ciphertext u = t2 * c5 + t4 * c7;
  Ciphertext v = u + u + c3;
  Ciphertext high = t3 * v;

  Ciphertext lin = z * c1 - z * c5 - z * c7;
  Ciphertext result = high + lin + c0;
  result.set_output("y");
}
```