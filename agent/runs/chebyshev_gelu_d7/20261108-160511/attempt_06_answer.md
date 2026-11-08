In this version I use s2 = 2·w² as the multiplier, so v = 2·c6·w + c4 reuses the doubled value r2 that is already computed. This saves one addition and one plaintext add compared with attempt 4. The multiplication structure is the same. I have not compiled or run it.

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
  Ciphertext s2 = s + s;
  Ciphertext r = w * c6;
  Ciphertext r2 = r + r;
  Ciphertext v = r2 + c4;              // 2*c6*w + c4
  Ciphertext t = s2 * v;
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