Idea: Paterson–Stockmeyer-style split around T4. Using T5 = 2·T4·T1 − T3, T6 = 2·T4·T2 − T2 and T7 = 2·T4·T3 − T1, the high part becomes T4·(c4 + 2·(c5·z + c6·t2 + c7·t3)) − (c5·t3 + c6·t2 + c7·z).

So the full polynomial is T4·W plus a lower part, where W = c4 + 2·(c5·z + c6·t2 + c7·t3). The lower part is c0 + (c1·z − c7·z) + (c2·t2 − c6·t2) + (c3·t3 − c5·t3).

This needs only 4 ct-ct multiplies (z², z·t2, t2², T4·W) instead of 6, at the cost of 2 extra plaintext multiplies. Depth is 5, so 6 levels are used. All additions combine terms at different levels, which avoids same-level scale mismatches.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext one("one");
  Plaintext c0("c0"), c1("c1"), c2("c2"), c3("c3"), c4("c4"), c5("c5"), c6("c6"), c7("c7");
  Ciphertext z = x * alpha;
  Ciphertext z2 = square(z);
  Ciphertext t2 = z2 + z2 - one;
  Ciphertext a3 = z * t2;
  Ciphertext t3 = a3 + a3 - z;
  Ciphertext s4 = square(t2);
  Ciphertext t4 = s4 + s4 - one;

  Ciphertext q = z * c5;
  q = q + t2 * c6;
  q = q + t3 * c7;
  Ciphertext w = q + q + c4;
  Ciphertext prod = t4 * w;

  Ciphertext low = z * c1 - z * c7;
  low = low + (t2 * c2 - t2 * c6);
  low = low + (t3 * c3 - t3 * c5);
  low = low + c0;
  Ciphertext result = prod + low;
  result.set_output("y");
}
```