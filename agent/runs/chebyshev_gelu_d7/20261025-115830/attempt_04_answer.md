Idea: group the even part by s = w² instead of by w, so everything stays one level lower. With t = c6·w:

total = s·(4t + 2c4) − c4 − 3t + c2·w + c1·z + c0.

This still uses 3 ct-ct multiplications and 4 plaintext multiplications. The final product s·k lands at level 4 instead of 5. The remaining terms sit at level 3 or lower and are aligned automatically.

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
  Ciphertext s = square(w);
  Ciphertext t = w * c6;
  Ciphertext t2 = t + t;
  Ciphertext k = t2 + t2;
  k = k + c4;
  k = k + c4;
  Ciphertext f = s * k;
  Ciphertext m = w * c2;
  m = m - t;
  m = m - t;
  m = m - t;
  m = m - c4;
  m = m + c0;
  Ciphertext g = z * c1;
  m = m + g;
  Ciphertext result = f + m;
  result.set_output("y");
}
```