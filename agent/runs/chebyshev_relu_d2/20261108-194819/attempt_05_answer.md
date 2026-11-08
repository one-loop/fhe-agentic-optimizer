I don't think a cheaper circuit than attempt 1 exists for this function:

- **Levels:** 2·c2·α²·x² is a product of five factors, so it needs at least 3 levels.
- **Multiplications:** α and c2 each need one ciphertext×plaintext multiply, which is 2. x² needs 1 ciphertext×ciphertext multiply. Attempt 1 has exactly these.
- **Additions:** the 4 additions (doubling, +c1, −c2, +c0) cannot be merged, because plaintext-plaintext arithmetic is not allowed.
- **Remaining overhead:** one ~1 ms mod switch, which aligns z (level 1) with v (level 2) before the ciphertext×ciphertext multiply. Removing it would need an extra multiply of at least 5 ms.
- **Alternatives checked:** pairing two level-1 branches (x·α)·(x·c2) or using `square` would need 3 or 4 plaintext multiplies, so they would be slower.

Here is the best kernel again as the final answer. Resubmitting it gives the same circuit, so it will not be timed again.

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