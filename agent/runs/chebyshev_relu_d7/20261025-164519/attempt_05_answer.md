Attempt 4 was slower, so I go back to attempt 3 and trim its plaintext adds. Each plaintext add costs about 2.5 ms because of encoding. Writing inner = 2·(c6·s·2 + c4) = 4c6·s + 2c4 as `(w2 + c4)` followed by a ciphertext doubling saves one of them.

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
  Ciphertext s = u + u - one;
  Ciphertext w = s * c6;
  Ciphertext w2 = w + w;
  Ciphertext h = w2 + c4;
  Ciphertext inner = h + h;
  Ciphertext m = s * inner;
  m = m + c2;
  m = m - c6;
  m = m - c6;
  m = m - c6;
  Ciphertext r = s * m;
  r = r + z * c1;
  r = r + c0;
  r = r - c4;
  r.set_output("y");
}
```