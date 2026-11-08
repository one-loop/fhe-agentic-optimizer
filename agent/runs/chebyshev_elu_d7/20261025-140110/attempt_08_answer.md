Idea: a last variation on the even/odd split in y = T2. The odd part is S = z·(c1 − b5) + b3·T3 with T3 = z·(W − 1), which gives a different dependency structure and level profile than computing z·inner.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext one("one");
  Plaintext c0("c0"), c1("c1"), c2("c2"), c3("c3"), c4("c4"), c5("c5"), c6("c6"), c7("c7");
  Ciphertext z = x * alpha;
  Ciphertext z2 = square(z);
  Ciphertext y = z2 + z2 - one;
  Ciphertext W = y + y;

  Ciphertext T3 = z * (W - one);

  // odd part
  Ciphertext b5 = W * c7 + c5;
  Ciphertext b3 = (W * b5 + c3) - c7;
  Ciphertext S = (z * (c1 - b5)) + b3 * T3;

  // even part
  Ciphertext f2 = W * c6 + c4;
  Ciphertext f1 = (W * f2 + c2) - c6;
  Ciphertext E = (y * f1 + c0) - f2;

  Ciphertext r = S + E;
  r.set_output("y");
}
```