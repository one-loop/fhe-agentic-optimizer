Idea: the same polynomial with fewer plaintext multiplications. Since T6 = T2·(2·T4 − 1), I can combine c2·T2 + c6·T6 = T2·(c6·(2·T4 − 1) + c2). The c2 term then becomes a plaintext add instead of a plaintext multiply, and the separate T6 product and its c6 multiply drop out. That leaves 4 plaintext multiplies (alpha, c1, c4, c6) instead of 5, and the depth stays at 5.

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
  Ciphertext t4s = square(t2);
  Ciphertext t4 = t4s + t4s - one;
  Ciphertext w = t4 + t4 - one;
  Ciphertext inner = w * c6 + c2;
  Ciphertext result = z * c1;
  result = result + t4 * c4;
  result = result + t2 * inner;
  result = result + c0;
  result.set_output("y");
}
```