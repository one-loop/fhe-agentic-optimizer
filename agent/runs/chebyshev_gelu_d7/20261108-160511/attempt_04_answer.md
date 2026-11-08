This version cuts the multiplicative depth from 5 to 4. The c6 and c4 terms now share one product, s·(4·c6·w + 2·c4), where s = w². The leftover −3·c6·w − c4 enters at lower levels, so CHEHAB can align it without an extra level. It keeps 3 ciphertext-ciphertext multiplications, and I have not compiled or run it.

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
  Ciphertext u = z * z;
  Ciphertext w = u + u - one;          // T2
  Ciphertext s = w * w;
  Ciphertext r = w * c6;
  Ciphertext r2 = r + r;
  Ciphertext r4 = r2 + r2;
  Ciphertext v = r4 + c4 + c4;         // 4*c6*w + 2*c4
  Ciphertext t = s * v;
  Ciphertext r3 = r2 + r;
  Ciphertext t2 = w * c2;
  Ciphertext t1 = z * c1;
  Ciphertext result = t - r3;
  result = result - c4;
  result = result + t2;
  result = result + t1;
  result = result + c0;
  result.set_output("y");
}
```