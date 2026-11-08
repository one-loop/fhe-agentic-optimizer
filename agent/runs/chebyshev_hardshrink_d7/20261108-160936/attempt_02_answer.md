The scale mismatch is between c5·T5 and c7·T7, which both land at level 5 with different multiplication histories. I fold c5 and c7 into the products before the last ciphertext-ciphertext multiply, so c5·T5 ends at level 4 and c7·T7 at level 5. They then meet at different levels. I haven't compiled this version either, so it may still hit a scale mismatch (c3·T3 and c5·T5 now both end at level 4).

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext one("one");
  Plaintext c1("c1");
  Plaintext c3("c3");
  Plaintext c5("c5");
  Plaintext c7("c7");
  Ciphertext z = x * alpha;
  Ciphertext u = z * z;
  Ciphertext t2 = u + u - one;
  Ciphertext zt2 = z * t2;
  Ciphertext t3 = zt2 + zt2 - z;
  Ciphertext t2sq = t2 * t2;
  Ciphertext t4 = t2sq + t2sq - one;
  Ciphertext a5 = t2 * c5;
  Ciphertext p5 = a5 * t3;
  Ciphertext s5 = p5 + p5 - z * c5;
  Ciphertext a7 = t3 * c7;
  Ciphertext p7 = a7 * t4;
  Ciphertext s7 = p7 + p7 - z * c7;
  Ciphertext result = z * c1;
  result = result + t3 * c3;
  result = result + s5;
  result = result + s7;
  result.set_output("y");
}
```