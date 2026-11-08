Last time the mismatch came from the c3 and c4 terms sitting at the same level with different scale histories. This attempt keeps the doubling-identity evaluation and gives the T4 term its own level by multiplying it by the plaintext `one`, one extra level on that term. I also group the deep terms T5, T6 and T7 into their own sum before combining it with the lower-degree terms. The remaining risk is that those three terms still meet at the same level with different scales.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext one("one");
  Plaintext c0("c0"), c1("c1"), c2("c2"), c3("c3"), c4("c4"), c5("c5"), c6("c6"), c7("c7");
  Ciphertext z = x * alpha;
  Ciphertext z2 = square(z);
  Ciphertext T2 = z2 + z2 - one;
  Ciphertext a = z * T2;
  Ciphertext T3 = a + a - z;
  Ciphertext b = square(T2);
  Ciphertext T4 = b + b - one;
  Ciphertext c = T2 * T3;
  Ciphertext T5 = c + c - z;
  Ciphertext d = square(T3);
  Ciphertext T6 = d + d - one;
  Ciphertext e = T3 * T4;
  Ciphertext T7 = e + e - z;
  Ciphertext low = z * c1;
  low = low + T2 * c2;
  low = low + T3 * c3;
  Ciphertext t4 = (T4 * c4) * one;
  Ciphertext high = T5 * c5;
  high = high + T6 * c6;
  high = high + T7 * c7;
  Ciphertext result = low + t4;
  result = result + high;
  result = result + c0;
  result.set_output("y");
}
```