I rewrite the inner sum so that T4 is not needed: inner = c3 − 2·c7 + 2·T2·(c5 + 2·c7·T2). That removes one plaintext multiply and the second "one" subtraction, but adds a few plaintext additions (c5, c3 and two of c7). I expect only a small net gain, about 2 ms, and I haven't compiled it.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext one("one");
  Plaintext c1("c1");
  Plaintext c3("c3");
  Plaintext c5("c5");
  Plaintext c7("c7");
  Ciphertext z = x * alpha;
  Ciphertext u = z * z;
  Ciphertext t2 = u + u - one;
  Ciphertext zt2 = z * t2;
  Ciphertext t3 = zt2 + zt2 - z;
  Ciphertext q = t2 * c7;
  Ciphertext q2 = q + q + c5;
  Ciphertext r = t2 * q2;
  Ciphertext inner = r + r + c3 - c7 - c7;
  Ciphertext w = z * c1 - z * c5 - z * c7;
  Ciphertext result = t3 * inner + w;
  result.set_output("y");
}
```