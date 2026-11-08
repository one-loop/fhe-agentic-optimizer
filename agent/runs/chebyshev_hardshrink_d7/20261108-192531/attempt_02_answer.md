This removes one plaintext multiplication by folding c5 into the T4 factor. Since T4 = 2·T2² − 1, the term group can be rewritten:

v = c3 + 2c5·T2 + 2c7·T4 = 2·T2·(c5 + 2c7·T2) + c3 − 2c7

So I compute p = c7·T2 once and v = 2·T2·(2p + c5) + c3 − c7 − c7. The product T2·(…) replaces the old T2² multiplication, so there are still 4 ciphertext multiplications. The separate c5·T2 product is gone, leaving 5 plaintext multiplications instead of 6, and the depth stays at 5 levels.

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

  Ciphertext z = x * alpha;              // L1
  Ciphertext zz = z * z;                 // L2
  Ciphertext t2 = zz + zz - one;         // L2
  Ciphertext zt2 = z * t2;               // L3
  Ciphertext t3 = zt2 + zt2 - z;         // L3

  Ciphertext p = t2 * c7;                // L3
  Ciphertext q = p + p + c5;             // L3
  Ciphertext r = t2 * q;                 // L4
  Ciphertext v = r + r + c3 - c7 - c7;   // L4
  Ciphertext main_term = t3 * v;         // L5

  Ciphertext lin = z * c1 - z * c5 - z * c7; // L2
  Ciphertext y = main_term + lin;
  y.set_output("y");
}
```