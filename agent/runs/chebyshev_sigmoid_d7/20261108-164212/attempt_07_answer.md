Using T2 and T2² directly, I rewrite the cubic in T2 as Q = 4·g·T2² + 2·(c3·T2 − c5·T2 − 2·c7·T2 + c7) − c3, where g = 2·c7·T2 + c5 − c7. This puts the final z multiplication at level 5 instead of 6. It uses 4 plaintext multiplications instead of 2, and it keeps all multiplications by 2 or 4 as ciphertext additions. I have not compiled or timed it.

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
  Plaintext c0("c0");
  Ciphertext z = x * alpha;
  Ciphertext w = z * z;
  Ciphertext t2 = w + w - one;
  Ciphertext s2 = t2 * t2;
  Ciphertext a7 = t2 * c7;
  Ciphertext a5 = t2 * c5;
  Ciphertext a3 = t2 * c3;
  Ciphertext g = a7 + a7;
  g = g + c5;
  g = g - c7;
  Ciphertext h = g * s2;
  h = h + h;
  h = h + h;
  Ciphertext lin = a3 - a5 - a7 - a7;
  lin = lin + c7;
  lin = lin + lin;
  lin = lin - c3;
  Ciphertext q = h + lin;
  q = q + c1;
  q = q - c5;
  q = q - c7;
  Ciphertext result = z * q;
  result = result + c0;
  result.set_output("y");
}
```