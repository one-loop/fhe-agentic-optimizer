The sum is now ordered as ((q + lin) + c1·z) + c0. The aim is to change which terms CHEHAB has to align: lin (level 3) and c1·z (level 2) are each lifted directly to q's level 4, rather than through an intermediate sum. The c0 encoding is applied once, on the final ciphertext.

The arithmetic is the same as attempt 1:
- 3 ct-ct multiplies, 4 ct-pt multiplies, depth 4.
- y = T4·(2·c6·u + c4) + (c2 − c6)·u + c1·z + c0.

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
  Ciphertext z2 = z * z;             // level 2
  Ciphertext u = z2 + z2 - one;      // T2, level 2
  Ciphertext u2 = u * u;             // level 3
  Ciphertext t4 = u2 + u2 - one;     // T4, level 3
  Ciphertext a = u * c6;             // level 3
  Ciphertext b = a + a + c4;         // level 3
  Ciphertext q = t4 * b;             // level 4
  Ciphertext lin = u * c2 - a;       // level 3
  Ciphertext s = q + lin;
  Ciphertext s2 = s + l1;
  Ciphertext result = s2 + c0;
  result.set_output("y");
}
```