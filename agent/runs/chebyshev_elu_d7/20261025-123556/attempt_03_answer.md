The attempt-2 mismatch is between the t4 term (D^9) and the t5 term (D^11) at level 5. The t5 term carries extra scale history because z, from a much lower level, is subtracted before the c5 multiply. I now apply the coefficient to the double product first and subtract z·c5 separately. This is still a guess about how CHEHAB tracks scales.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext one("one");
  Ciphertext z = x * alpha;
  Ciphertext s = square(z);
  Ciphertext t2 = s + s - one;
  Ciphertext w3 = z * t2;
  Ciphertext t3 = w3 + w3 - z;
  Ciphertext s4 = square(t2);
  Ciphertext t4a = s4 + s4 - one;
  Ciphertext w4 = z * t3;
  Ciphertext t4b = w4 + w4 - t2;
  Ciphertext w5 = t2 * t3;
  Ciphertext s6 = square(t3);
  Ciphertext t6 = s6 + s6 - one;
  Ciphertext w7 = t3 * t4a;
  Plaintext c0("c0"), c1("c1"), c2("c2"), c3("c3"), c4("c4"), c5("c5"), c6("c6"), c7("c7");
  Ciphertext zc1 = z * c1;
  Ciphertext zc57 = z * c5;
  Ciphertext zc7 = z * c7;
  Ciphertext u5 = w5 * c5;
  Ciphertext u7 = w7 * c7;
  Ciphertext B = u5 + u5 + u7 + u7 - zc57 - zc7;
  Ciphertext result = zc1;
  result = result + t2 * c2;
  result = result + t3 * c3;
  result = result + t4b * c4;
  result = result + t6 * c6;
  result = result + B;
  result = result + c0;
  result.set_output("y");
}
```