This variant works from the raw square sq = t² instead of T4 in the c7 product. Since 2·c7·T4 = 4·c7·sq − 2·c7, the weight becomes w = 4·(c7·sq) + c3 − c7 − c7, with the doublings done as cheap additions. T4 is still built for e5, which keeps the level structure of attempt 3.

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
  Plaintext c7("c7");
  Ciphertext z = x * alpha;
  Ciphertext u = square(z);
  Ciphertext t = u + u - one;
  Ciphertext e3 = t + t - one;
  Ciphertext sq = square(t);
  Ciphertext t4 = sq + sq - one;
  Ciphertext e5 = t4 + t4 - e3;
  Ciphertext q = sq * c7;
  Ciphertext q2 = q + q;
  Ciphertext w = q2 + q2 + c3;
  w = w - c7;
  w = w - c7;
  Ciphertext s = e3 * w + e5 * c5;
  s = s - c7;
  s = s + c1;
  Ciphertext result = z * s + c0;
  result.set_output("y");
}
```