Attempt 3 needs 5 ciphertext-ciphertext multiplications at depth 4, and I believe that is the minimum. The coefficients can only be introduced through plaintext multiplies, so with only those operations neither the count nor the depth can go lower.

This attempt removes small extras instead:
- **The "one" add at level 3.** T4 is no longer formed. I use b = T4·q = 2·T2²·q − q, so the plaintext add of "one" at level 3 (the encoding plus its alignment) disappears.
- **Same-level grouping.** The leftover −2q splits into −2·p and −2·c5·z, where p = s·(c7·z). The −2·p part joins s·r at level 3, and both have the same multiplication history. The −2·c5·z part joins the level-2 linear terms.

This keeps the same-level sums aligned and leaves only the unavoidable cross-level alignments.

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

  Ciphertext p = s * z7;
  Ciphertext q = p + z5;
  Ciphertext g = t2s * q;
  Ciphertext g2 = g + g;
  Ciphertext g4 = g2 + g2;

  Ciphertext r = z3 - z5;
  Ciphertext a = s * r;
  Ciphertext mid = a - p - p;

  Ciphertext lin = z1 - z7 - z5 - z5;
  Ciphertext result = g4 + mid + lin + c0;
  result.set_output("y");
}
```