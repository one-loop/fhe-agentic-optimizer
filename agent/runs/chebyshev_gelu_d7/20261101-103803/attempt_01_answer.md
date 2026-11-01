The baseline builds T3 through T7 as a chain and uses 5 ciphertext-ciphertext multiplies. This version only builds the even terms it needs, using the substitution u = T2(z) = 2z² − 1:
- T4 = 2u² − 1
- T6 = 2·u·T4 − u

The c6 and c4 terms are then folded into one product:
c4·T4 + c6·T6 = T4·(2·c6·u + c4) − c6·u.

This leaves 3 ciphertext-ciphertext multiplies (z², u², T4·b) and 4 plaintext multiplies (alpha, c6, c2, c1). The multiplicative depth drops from 7 to 4.

The additions should not cause scale mismatches. The two terms −c6·u and c2·u have the same multiplication history and meet at level 3. The term c1·z is at level 2 and the main product is at level 4, so CHEHAB aligns those for free.

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
  Ciphertext z2 = z * z;             // level 2
  Ciphertext u = z2 + z2 - one;      // T2, level 2
  Ciphertext u2 = u * u;             // level 3
  Ciphertext t4 = u2 + u2 - one;     // T4, level 3
  Ciphertext a = u * c6;             // c6*T2, level 3
  Ciphertext b = a + a + c4;         // 2 c6 T2 + c4, level 3
  Ciphertext q = t4 * b;             // level 4
  Ciphertext lin = u * c2 - a;       // (c2 - c6) T2, level 3
  Ciphertext l1 = z * c1;            // level 2
  Ciphertext result = q + (lin + (l1 + c0));
  result.set_output("y");
}
```