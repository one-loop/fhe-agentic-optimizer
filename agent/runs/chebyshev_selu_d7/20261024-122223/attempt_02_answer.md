The scale mismatch came from my accumulation chain. The running sum had already been aligned to level 4 when I added `t4*c4`, so the two operands were at the same level with different scales. This version sums terms of the same level and multiplication history first, then combines the groups from shallowest to deepest, so mixed-level additions only happen between groups at different levels. The polynomial evaluation itself is unchanged.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext one("one");
  Ciphertext z = x * alpha;
  Ciphertext z2 = square(z);
  Ciphertext t2 = z2 + z2 - one;
  Ciphertext zt2 = z * t2;
  Ciphertext t3 = zt2 + zt2 - z;
  Ciphertext t2sq = square(t2);
  Ciphertext t4 = t2sq + t2sq - one;
  Ciphertext p25 = t2 * t3;
  Ciphertext t5 = p25 + p25 - z;
  Ciphertext t3sq = square(t3);
  Ciphertext t6 = t3sq + t3sq - one;
  Ciphertext p34 = t3 * t4;
  Ciphertext t7 = p34 + p34 - z;

  Plaintext c0("c0"), c1("c1"), c2("c2"), c3("c3"), c4("c4"), c5("c5"), c6("c6"), c7("c7");
  Ciphertext a = z * c1;
  Ciphertext b = t2 * c2;
  Ciphertext A = a + b;
  Ciphertext B = t3 * c3 + t4 * c4;
  Ciphertext C = t5 * c5 + t6 * c6 + t7 * c7;
  Ciphertext r = A + B;
  r = r + C;
  r = r + c0;
  r.set_output("y");
}
```