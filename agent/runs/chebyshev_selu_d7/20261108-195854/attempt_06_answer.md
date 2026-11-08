Attempt 4 reached about 2% over the best but not the required 3%. I'm keeping its structure: `square()` and P = 2·T2²·w − w, which saves the `− one` encoding for T4.

This version trims ciphertext additions by sharing sub-differences:
- **Shared difference:** `d = zc5 − zc7` is computed once and used in both q and `lin` (`lin = (zc1 − zc3) + d`).
- **Fewer chained adds:** the L3 terms are combined as `(g − w) + lin` with fewer intermediate additions.

The 5 multiplies, 4 levels and 5 plaintext multiplies are unchanged.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext one("one");
  Plaintext c0("c0");
  Plaintext c1("c1");
  Plaintext c2("c2");
  Plaintext c3("c3");
  Plaintext c4("c4");
  Plaintext c5("c5");
  Plaintext c6("c6");
  Plaintext c7("c7");

  Ciphertext z = x * alpha;                 // L1
  Ciphertext zz = square(z);                // L2
  Ciphertext T2 = zz + zz - one;            // L2
  Ciphertext T4s = square(T2);              // L3

  Ciphertext zc1 = z * c1;                  // L2
  Ciphertext zc3 = z * c3;
  Ciphertext zc5 = z * c5;
  Ciphertext zc7 = z * c7;

  Ciphertext d = zc5 - zc7;                 // (c5-c7) z, shared
  Ciphertext r = zc7 + zc7 + c6;            // L2
  Ciphertext h = T2 * r;                    // L3
  Ciphertext q = d + h;                     // L3
  Ciphertext w = q + q + c4;                // L3
  Ciphertext P = T4s * (w + w);             // L4

  Ciphertext s = zc3 - zc5;                 // L2
  Ciphertext u = s + s + c2 - c6;           // L2
  Ciphertext g = T2 * u;                    // L3
  Ciphertext lin = (zc1 - zc3) + d;         // L2

  Ciphertext y = ((g - w) + lin) + P + c0;
  y.set_output("y");
}
```