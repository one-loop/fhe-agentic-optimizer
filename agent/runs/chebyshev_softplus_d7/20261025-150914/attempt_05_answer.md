Idea: combine attempt 4's reuse of s with attempt 3's trick of taking the c6 product from w. With r = 2·(w·c6) − c6, the whole coefficient chain u is ready at depth 2, before s is available, and the final multiply s·(u+u) is the only deep operation.

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
  Ciphertext w = square(z);
  Ciphertext t2 = w + w - one;
  Ciphertext s = square(t2);
  Ciphertext wc6 = w * c6;
  Ciphertext r = (wc6 + wc6) - c6;
  Ciphertext r2 = r + r;
  Ciphertext u = r2 + c4;
  Ciphertext m = s * (u + u);
  Ciphertext r3 = r2 + r;
  Ciphertext result = (z * c1 + t2 * c2 - r3) + (m - c4 + c0);
  result.set_output("y");
}
```