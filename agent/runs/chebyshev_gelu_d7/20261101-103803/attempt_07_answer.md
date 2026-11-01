Attempt 6 saved about 2.3% by dropping one plaintext encoding but missed the 3% bar. This version keeps that algebra and changes only the summation order:

y = 2·(u²·b) + ((c2·u − a − b) + c1·z) + c0

The level-2 term c1·z is lifted only to level 3, the cheaper deeper level, where it joins the other level-3 terms. That sum then gets a single alignment to the level-4 product. Plaintext additions stay at three (−one, +c4, +c0), with c0 added last at the deepest and cheapest level. The `square()` calls are kept.

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

  Ciphertext z = x * alpha;          // level 1
  Ciphertext l1 = z * c1;            // level 2
  Ciphertext z2 = square(z);         // level 2
  Ciphertext u = z2 + z2 - one;      // T2, level 2
  Ciphertext u2 = square(u);         // level 3
  Ciphertext a = u * c6;             // level 3
  Ciphertext b = a + a + c4;         // level 3
  Ciphertext q = u2 * b;             // level 4
  Ciphertext lin = u * c2 - a - b;   // level 3
  Ciphertext low = lin + l1;         // level 3
  Ciphertext s = (q + q) + low;      // level 4
  Ciphertext result = s + c0;
  result.set_output("y");
}
```