The previous kernel used three plaintext products, one per eₖ term. Expanding gives S = e3·(c3 + 2·c7·T4) + c5·e5 − c7 + c1, which needs only two plaintext products (c7·T4 and c5·e5). c3, c7 and c1 are added to the ciphertext as plaintexts, which costs no level. Depth stays at 6, and there is one fewer ciphertext-plaintext multiplication and one fewer rescale.

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
  Ciphertext t4 = square(t);
  t4 = t4 + t4 - one;
  Ciphertext e5 = t4 + t4 - e3;
  Ciphertext q = t4 * c7;
  Ciphertext w = q + q + c3;
  Ciphertext s = e3 * w + e5 * c5;
  s = s - c7;
  s = s + c1;
  Ciphertext result = z * s + c0;
  result.set_output("y");
}
```