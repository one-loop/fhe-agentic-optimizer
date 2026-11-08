I could not find a cheaper circuit than attempt 1; I believe it is optimal for this function.

- **Levels:** the leading term 2·α²·c2·x² needs five multiplicative leaves, so it cannot fit in fewer than 3 levels.
- **Multiplications:**
  - Attempt 1 uses exactly 2 ciphertext×plaintext multiplies, one for α and one for c2, which is the minimum.
  - It uses 1 ciphertext×ciphertext multiply, needed for the x² factor.
  - Moving α or c2 elsewhere, as in attempt 3, adds a multiply and a level.
- **Additions:** the 4 additions are the minimum, because plaintext-plaintext arithmetic (c0 − c2, 2·c2) is not allowed.
- **Remaining overhead:** one ~1 ms mod switch to align z (level 1) with v (level 2). Removing it would need another multiply of 5 ms or more.

Since no cheaper structure exists, I am resubmitting the best kernel:

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext c0("c0");
  Plaintext c1("c1");
  Plaintext c2("c2");
  Ciphertext z = x * alpha;
  Ciphertext w = z * c2;
  Ciphertext v = w + w + c1;
  Ciphertext p = z * v;
  Ciphertext y = p - c2 + c0;
  y.set_output("y");
}
```