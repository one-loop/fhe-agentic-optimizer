I have no further idea that I expect to beat the current best by the required 3%, so I'm submitting the best confirmed kernel (attempt 2, about 56 ms vs 169 ms for the baseline) as the final answer. Every variant I tried since then (attempts 3, 6 and 7) measured only about 2% faster and was not confirmed.

The kernel evaluates the even part of the Chebyshev series from w = T2 = 2z² − 1 and folds c6 and c4 together as T4·(2·c6w + c4) − c6w. This brings depth down from 7 to 4 and ciphertext multiplications from 5 to 3.

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
  Ciphertext w2 = w * w;
  Ciphertext t4 = w2 + w2 - one;
  Ciphertext c6w = w * c6;
  Ciphertext q = c6w + c6w + c4;
  Ciphertext p = t4 * q;
  Ciphertext term2 = w * c2;
  Ciphertext term1 = z * c1;
  Ciphertext r = term2 - c6w;
  r = r + term1;
  Ciphertext result = p + r;
  result = result + c0;
  result.set_output("y");
}
```