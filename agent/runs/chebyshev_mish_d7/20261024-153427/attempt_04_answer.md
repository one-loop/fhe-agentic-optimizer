This attempt splits the polynomial around T4, using T_{4+j} = 2·T4·T_j − T_{4−j}. The high part becomes T4·(c4 + 2(c5·z + c6·T2 + c7·T3)) − (c5·T3 + c6·T2 + c7·z). This needs only 4 ciphertext multiplications instead of 6 and a depth of about 5 instead of 8. The cost is more plaintext multiplications, but each one is cheaper than a ciphertext multiplication.

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

  Ciphertext s = z * c5 + t2 * c6 + t3 * c7;
  Ciphertext q = s + s + c4;
  Ciphertext hi = t4 * q;

  Ciphertext lo = z * c1 + t2 * c2 + t3 * c3;
  Ciphertext sub = t3 * c5 + t2 * c6 + z * c7;
  Ciphertext y = hi + lo - sub + c0;
  y.set_output("y");
}
```