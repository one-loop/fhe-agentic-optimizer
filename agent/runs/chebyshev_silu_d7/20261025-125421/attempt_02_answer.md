Idea: fold the c4 and c6 terms into one ciphertext product, which removes two plaintext multiplies and one level.

Since T6 = 2·T2·T4 − T2, we get c4·T4 + c6·T6 = T4·(c4 + 2·c6·T2) − c6·T2. Here w = c6·T2 is one plaintext multiply, and c4 enters through a plaintext add rather than a multiply. The final product with T4 then sits at depth 4 instead of 5. The c2 term is merged with the −c6·T2 term as T2·c2 − w.

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
  Ciphertext t2 = u + u - one;
  Ciphertext s = square(t2);
  Ciphertext t4 = s + s - one;
  Ciphertext w = t2 * c6;
  Ciphertext v = w + w + c4;
  Ciphertext m = t4 * v;
  Ciphertext result = z * c1;
  result = result + t2 * c2;
  result = result - w;
  result = result + m;
  result = result + c0;
  result.set_output("y");
}
```