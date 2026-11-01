Idea: rewrite the polynomial in Horner form in w, which removes one plaintext multiplication. With e = c6·w, the cubic becomes P = w·(c2 + w·(4e + 2c4)) − 3e − c4. The `−3c6·w` term is just `3e`, which needs only ciphertext additions. There are 3 plaintext multiplications (alpha, c6, c1) instead of 4, and still 3 ciphertext-ciphertext multiplications.

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
  Ciphertext w = u + u - one;
  Ciphertext e = w * c6;
  Ciphertext e2 = e + e;
  Ciphertext e4 = e2 + e2;
  Ciphertext f = e4 + c4 + c4;
  Ciphertext g = w * f + c2;
  Ciphertext e3 = e2 + e;
  Ciphertext p = w * g - e3;
  p = p - c4;
  Ciphertext r = p + z * c1;
  r = r + c0;
  r.set_output("y");
}
```