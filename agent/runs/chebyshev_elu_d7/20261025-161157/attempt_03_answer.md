Idea: use a baby-step/giant-step split on T4 instead of the full recurrence. With T_{4+j} = 2·T4·T_j − T_{4−j}:

p = c0 + (c1−c7)T1 + (c2−c6)T2 + (c3−c5)T3 + T4·(c4 + 2c5·T1 + 2c6·T2 + 2c7·T3)

This needs only 4 ciphertext×ciphertext multiplications (z², z·T2, T2², and T4·B) instead of 6. The attempt-1 and attempt-2 failures came from different-history terms meeting at the same level. This version keeps the differences as plaintext-coefficient products of the same ciphertext (identical history) and adds groups in increasing level order, so groups at different levels are aligned automatically.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext one("one");
  Plaintext c0("c0"), c1("c1"), c2("c2"), c3("c3"), c4("c4"), c5("c5"), c6("c6"), c7("c7");
  Ciphertext z = x * alpha;
  Ciphertext zz = square(z);
  Ciphertext t2 = zz + zz - one;
  Ciphertext zt2 = z * t2;
  Ciphertext t3 = zt2 + zt2 - z;
  Ciphertext t2s = square(t2);
  Ciphertext t4 = t2s + t2s - one;

  // B = c4 + 2 c5 T1 + 2 c6 T2 + 2 c7 T3
  Ciphertext zc5 = z * c5;
  Ciphertext t2c6 = t2 * c6;
  Ciphertext t3c7 = t3 * c7;
  Ciphertext b = (zc5 + zc5);
  b = b + (t2c6 + t2c6);
  b = b + (t3c7 + t3c7);
  b = b + c4;
  Ciphertext hi = t4 * b;

  // A = c0 + (c1-c7) T1 + (c2-c6) T2 + (c3-c5) T3
  Ciphertext a = z * c1 - z * c7;
  a = a + (t2 * c2 - t2 * c6);
  a = a + (t3 * c3 - t3 * c5);
  a = a + c0;

  Ciphertext r = a + hi;
  r.set_output("y");
}
```