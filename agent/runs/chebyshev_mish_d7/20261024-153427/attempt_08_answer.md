This last attempt goes back to the best Clenshaw chain and fuses its first two steps through z² = square(z). Expanding gives b5 = c5 − c7 + 2c6·z + 4c7·z², where 4c7·z² is c7·z2 added four times. It avoids the first dependent multiplication in the chain, so the later steps sit one level shallower. It costs one extra plaintext multiplication.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext c0("c0"), c1("c1"), c2("c2"), c3("c3"), c4("c4"), c5("c5"), c6("c6"), c7("c7");
  Ciphertext z = x * alpha;
  Ciphertext w = z + z;
  Ciphertext z2 = square(z);

  Ciphertext u = z2 * c7;
  Ciphertext u2 = u + u;
  Ciphertext u4 = u2 + u2;
  Ciphertext v = z * c6;
  Ciphertext b6 = w * c7 + c6;
  Ciphertext b5 = u4 + v + v + c5 - c7;

  Ciphertext m4 = w * b5;
  Ciphertext b4 = m4 + c4 - b6;
  Ciphertext m3 = w * b4;
  Ciphertext b3 = m3 + c3 - b5;
  Ciphertext m2 = w * b3;
  Ciphertext b2 = m2 + c2 - b4;
  Ciphertext m1 = w * b2;
  Ciphertext b1 = m1 + c1 - b3;
  Ciphertext y = z * b1 + c0 - b2;
  y.set_output("y");
}
```