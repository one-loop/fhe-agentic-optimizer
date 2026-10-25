I regrouped the even part as a polynomial in w = T2. T4 = 2w² − 1 and T6 = w·(4w² − 3), so the sum c2·w + c4·T4 + c6·T6 + c0 equals w·(c2 + 2·c4·w + c6·(4w² − 3)) + c0 − c4. This drops one plaintext multiply from the previous best (4 instead of 5) and keeps depth 5. The terms that meet in an add sit at different levels, which avoids the scale mismatch seen in attempts 2 and 3.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext one("one");
  Plaintext c0("c0");
  Plaintext c1("c1");
  Plaintext c2("c2");
  Plaintext c4("c4");
  Plaintext c6("c6");
  Ciphertext z = x * alpha;
  Ciphertext u = square(z);
  Ciphertext w = u + u - one;
  Ciphertext w2 = square(w);
  Ciphertext w4 = w2 + w2;
  w4 = w4 + w4;
  Ciphertext sp = w4 - one - one - one;
  Ciphertext a = w * c4;
  Ciphertext inner = sp * c6;
  inner = inner + a + a;
  inner = inner + c2;
  Ciphertext result = w * inner;
  result = result + z * c1;
  result = result + c0;
  result = result - c4;
  result.set_output("y");
}
```