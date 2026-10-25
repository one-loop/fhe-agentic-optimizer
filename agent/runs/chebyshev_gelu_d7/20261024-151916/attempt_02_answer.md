Idea: rewrite the even part in terms of w = T2(z) = 2z² − 1, so that it factors as

c0 + c2·w + c4·(2w² − 1) + c6·w·(4w² − 3) = w·[c2 + 2c4·w + c6·(4w² − 3)] − c4 + c0.

This needs only three ciphertext multiplications (`square(z)`, `square(w)` and `w * bracket`). It also drops the T6 product, and it uses 4 ciphertext-plaintext multiplications instead of 5. Here 4w² − 3 = 2·T4 − 1, which is cheap to form.

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
  Ciphertext t4 = w2 + w2 - one;
  Ciphertext s = t4 + t4 - one;
  Ciphertext w_2 = w + w;
  Ciphertext bracket = w_2 * c4 + s * c6;
  bracket = bracket + c2;
  Ciphertext result = w * bracket;
  result = result + z * c1;
  result = result - c4;
  result = result + c0;
  result.set_output("y");
}
```