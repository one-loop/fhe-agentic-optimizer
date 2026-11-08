I don't have a further improvement that I expect to beat attempt 6. The one idea I considered was factoring z out of the whole sum: result = z·[(2·T2 − 1)·inner + c1 − c5 − c7], which would cut the plaintext multiplies from five to two. It needs 6 levels, and every operation gets more expensive with each extra level, so I expect it to be slower. Below is attempt 6 again, which is the best kernel so far at 94.14 ms against 215.54 ms for the baseline.

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