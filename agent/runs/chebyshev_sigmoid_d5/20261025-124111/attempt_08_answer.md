The last attempt trims a plaintext add. In attempt 5 the −c5 inside m only existed to produce the −2·c5·t2 term. Here that term is subtracted directly as the ciphertext 2s, with s = c5·t2, and m = 2s + c3 loses its −c5.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext one("one");
  Plaintext c0("c0");
  Plaintext c1("c1");
  Plaintext c3("c3");
  Plaintext c5("c5");
  Ciphertext z = x * alpha;
  Ciphertext u = square(z);
  Ciphertext t2 = u + u - one;
  Ciphertext s = t2 * c5;
  Ciphertext s2 = s + s;
  Ciphertext m = s2 + c3;
  Ciphertext p = t2 * m;
  Ciphertext inner = p + p - s2 + c1 - c3 - c5;
  Ciphertext result = inner * z + c0;
  result.set_output("y");
}
```