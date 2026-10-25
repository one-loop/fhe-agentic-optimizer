The mismatch is probably in the level-5 group: `p25` multiplies `t2` (level 2) by `t3` (level 3), so one operand is aligned and its scale differs from `t3sq` and `p34`. This version multiplies the coefficient into one factor first, giving `(t2*c5)*t3`, `(t3*c6)*t3` and `(t3*c7)*t4`. `A5` lands at level 4 and `A6`, `A7` at level 5, so the terms that share a level also share the same multiplication pattern. The `z*c` pieces and the constant subtractions stay as before.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Ciphertext z = x * alpha;
  Plaintext one("one");
  Plaintext c0("c0"), c1("c1"), c2("c2"), c3("c3"), c4("c4"), c5("c5"), c6("c6"), c7("c7");

  Ciphertext z2 = square(z);
  Ciphertext t2 = z2 + z2 - one;
  Ciphertext zt2 = z * t2;
  Ciphertext t3 = zt2 + zt2 - z;
  Ciphertext t2sq = square(t2);
  Ciphertext t4 = t2sq + t2sq - one;

  Ciphertext A5 = (t2 * c5) * t3;
  Ciphertext A6 = (t3 * c6) * t3;
  Ciphertext A7 = (t3 * c7) * t4;

  Ciphertext G3 = z2 * c2;
  Ciphertext G4 = zt2 * c3 + t2sq * c4;
  Ciphertext G5 = A6 + A7;

  Ciphertext s = G3 + G4;
  s = s + A5;
  s = s + G5;
  Ciphertext d = s + s;

  Ciphertext Zg = z * c1;
  Zg = Zg - z * c3;
  Zg = Zg - z * c5;
  Zg = Zg - z * c7;

  Ciphertext r = d + Zg;
  r = r + c0;
  r = r - c2;
  r = r - c4;
  r = r - c6;
  r.set_output("y");
}
```