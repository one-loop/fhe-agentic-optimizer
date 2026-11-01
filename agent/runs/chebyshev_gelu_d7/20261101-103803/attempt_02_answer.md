This attempt removes the explicit T4 = 2u² − 1 step. Since T4 = 2u² − 1, the product T4·b becomes u²·b + u²·b − b, and b = 2a + c4 with a = c6·u. That gives:

y = 2·u²·b + (c2·u − 3a) − c4 + c1·z + c0

The ct-ct multiply count (3), ct-pt multiply count (4) and depth (4) stay the same. Two additions and their alignment move to a lower level, and the depth-4 product now takes u² directly from level 3.

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
  Ciphertext a = u * c6;             // level 3
  Ciphertext b = a + a + c4;         // level 3
  Ciphertext q = u2 * b;             // level 4
  Ciphertext lin = u * c2 - a - a - a - c4;   // level 3
  Ciphertext l1 = z * c1 + c0;       // level 2
  Ciphertext result = (q + q) + (lin + l1);
  result.set_output("y");
}
```