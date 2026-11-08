The baseline builds every T_k up to degree 7 and uses 7 levels. Only c0, c1, c2, c4 and c6 matter, so I rewrite the even terms in terms of u = T2 = 2z² − 1:

- **Basis identities:** T4 = 2u² − 1 and T6 = u(2·T4 − 1).
- **Folding:** c4·T4 + c6·T6 = T4·(c4 + 2·c6·u) − c6·u.

The whole output becomes:

y = c0 + c1·z + (c2·u − c6·u) + T4·(c6·u + c6·u + c4)

**Expected cost:**
- 3 ciphertext-ciphertext multiplications: z², u², and T4·w.
- 4 plaintext multiplications: alpha, c6, c2, c1. The product c6·u is computed once and reused.
- 4 levels consumed instead of 7.

**Scale check:**
- The terms that meet at level 3 (c2·u and c6·u) come from the same multiplication pattern.
- The other terms sit at different levels: c1·z at level 2, the T4·w product at level 4. CHEHAB aligns those for free.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext one("one");
  Plaintext c0("c0");
  Plaintext c1("c1");
  Plaintext c2("c2");
  Plaintext c4("c4");
  Plaintext c6("c6");

  Ciphertext z = x * alpha;            // level 1
  Ciphertext zz = z * z;               // level 2
  Ciphertext u = zz + zz - one;        // T2, level 2
  Ciphertext uu = u * u;               // level 3
  Ciphertext t4 = uu + uu - one;       // T4, level 3
  Ciphertext c6u = u * c6;             // level 3
  Ciphertext w = c6u + c6u + c4;       // c4 + 2 c6 u, level 3
  Ciphertext high = t4 * w;            // c4 T4 + c6 (T6 + u), level 4
  Ciphertext mid = u * c2 - c6u;       // (c2 - c6) u, level 3
  Ciphertext lin = z * c1;             // level 2
  Ciphertext result = high + mid + lin + c0;
  result.set_output("y");
}
```