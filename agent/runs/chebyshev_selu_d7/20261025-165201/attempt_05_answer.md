This version splits at T4 using T_{4+j} = 2·T4·Tj − T_{4−j}, which gives

p = [c0 + c1·T1 + c2·T2 + c3·T3 − c5·T3 − c6·T2 − c7·T1] + T4·[c4 + 2(c5·T1 + c6·T2 + c7·T3)].

This needs only 4 ciphertext-ciphertext multiplies (z², z·T2, T2², and T4·R) instead of 6, and the depth drops to 5. It uses more plaintext multiplies, which are cheaper. Terms that share a level also share the same multiplication pattern, so I expect no scale conflicts.

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
  Ciphertext p3 = z * t2;
  Ciphertext t3 = p3 + p3 - z;
  Ciphertext p4 = square(t2);
  Ciphertext t4 = p4 + p4 - one;

  Ciphertext a1 = z * c1 - z * c7;
  Ciphertext a2 = t2 * c2 - t2 * c6;
  Ciphertext a3 = t3 * c3 - t3 * c5;

  Ciphertext s = z * c5;
  Ciphertext s2 = t2 * c6;
  Ciphertext s3 = t3 * c7;
  Ciphertext lowR = s + s2;
  Ciphertext r = lowR + s3;
  Ciphertext R = r + r + c4;

  Ciphertext top = t4 * R;
  Ciphertext low = a1 + a2;
  low = low + a3;
  Ciphertext y = low + top;
  y = y + c0;
  y.set_output("y");
}
```