`T7` can be computed as `2·T2·T5 − T3`. This drops the `T2²` product, leaving 4 ct-ct multiplications. It also puts `T7` one level deeper than `T5`, so `c5·T5` and `c7·T7` land at different levels. I accumulate the terms in ascending level order so that no two terms with different multiplication histories meet at the same level.

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
  Ciphertext w = z * z;
  Ciphertext t2 = w + w - one;
  Ciphertext zt2 = z * t2;
  Ciphertext t3 = zt2 + zt2 - z;
  Ciphertext p5 = t2 * t3;
  Ciphertext t5 = p5 + p5 - z;
  Ciphertext p7 = t2 * t5;
  Ciphertext t7 = p7 + p7 - t3;
  Ciphertext r = z * c1;
  r = r + t3 * c3;
  r = r + t5 * c5;
  r = r + t7 * c7;
  r.set_output("y");
}
```