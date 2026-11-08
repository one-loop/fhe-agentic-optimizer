Idea: the earlier results show that operations performed after more levels have been consumed are cheaper, since the ciphertext modulus is smaller there. So I keep the Clenshaw chain unchanged and burn two levels up front with scalar `*one` multiplies. Every later multiplication, rotation-free add and alignment then runs on a smaller modulus, and the total depth is 10, which is still allowed. The extra multiplies are cheap scalar ones (about 5 ms each).

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext one("one");
  Ciphertext x1 = x * one;
  Ciphertext x2 = x1 * one;
  Ciphertext z = x2 * alpha;
  Ciphertext z2 = z + z;
  Plaintext c0("c0"), c1("c1"), c2("c2"), c3("c3"), c4("c4"), c5("c5"), c6("c6"), c7("c7");
  Ciphertext b6 = z2 * c7 + c6;
  Ciphertext b5 = z2 * b6 + c5 - c7;
  Ciphertext b4 = z2 * b5 + c4 - b6;
  Ciphertext b3 = z2 * b4 + c3 - b5;
  Ciphertext b2 = z2 * b3 + c2 - b4;
  Ciphertext b1 = z2 * b2 + c1 - b3;
  Ciphertext y = z * b1 + c0 - b2;
  y.set_output("y");
}
```