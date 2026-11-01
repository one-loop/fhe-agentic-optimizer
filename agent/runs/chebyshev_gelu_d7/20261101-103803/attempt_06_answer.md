This version removes one plaintext addition. Each plaintext addition costs about 2.5 ms of encoding.

The change rests on two identities, using a = c6·u and b = 2a + c4:
- T4 = 2u² − 1, so T4·b = 2·(u²·b) − b.
- Since b is already a ciphertext that contains c4, subtracting b is a cheap ciphertext subtraction.

The result is y = 2·(u²·b) − b + (c2·u − a) + c1·z + c0. It needs only three plaintext additions (−one, +c4, +c0) instead of four. It keeps the square() calls and the add order that measured slightly faster in attempts 4 and 5. Also, −b and c2·u − a have the same multiplication history at level 3, so they combine without alignment.

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
  Ciphertext s = (q + q) + lin;
  Ciphertext s2 = s + l1;
  Ciphertext result = s2 + c0;
  result.set_output("y");
}
```