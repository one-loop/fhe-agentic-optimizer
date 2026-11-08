Substituting T7 = 2·T2·T5 − T3 into the sum gives c5·T5 + c7·T7 = T5·(c5 + 2·c7·T2) − c7·T3. This removes one ct-ct multiply, uses only one multiply by c7 on T2 and one on T3, and drops the c5·T5 multiply. The depth falls from 6 to 5.

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
  Ciphertext z2 = z * z;
  Ciphertext t2 = z2 + z2 - one;
  Ciphertext zt2 = z * t2;
  Ciphertext t3 = zt2 + zt2 - z;
  Ciphertext p5 = t2 * t3;
  Ciphertext t5 = p5 + p5 - z;
  Ciphertext m = t2 * c7;
  Ciphertext w = m + m + c5;
  Ciphertext result = z * c1;
  result = result + t3 * c3;
  result = result - t3 * c7;
  result = result + t5 * w;
  result.set_output("y");
}
```