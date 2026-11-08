I don't have a structurally cheaper circuit left. The remaining options only add plaintext operations or levels, so I'm resubmitting the best kernel (attempt 3). It uses 3 ct-ct multiplications, 4 plaintext multiplications and 4 levels, using the identity c4·T4 + c6·T6 = T4·(c4 + 2·c6·T2) − c6·T2.

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
  Ciphertext t2 = u + u - one;
  Ciphertext s2 = square(t2);
  Ciphertext t4 = s2 + s2 - one;
  Ciphertext a = t2 * c6;
  Ciphertext w = a + a + c4;
  Ciphertext prod = t4 * w;
  Ciphertext low = t2 * c2 - a;
  low = low + z * c1;
  low = low + c0;
  Ciphertext result = prod + low;
  result.set_output("y");
}
```