The idea is to factor the inner polynomial through t2 = 2u − 1, where u = z². Then 3T3/z-style terms become A = 2·t2 − 1 and B = t2·(4·t2 − 2) − 1, so the inner sum is t2·(c5·2A + 2c3) + (c1 − c3 − c5). The plaintext-only constant is applied as three separate multiplies of z, because plaintext-plaintext arithmetic is not allowed. This needs only one plaintext multiply inside the inner sum instead of two. It also lets z·t2 be computed in parallel with the inner factor m, so the multiplicative depth drops from 5 to 4.

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
  Ciphertext a = t2 + t2 - one;
  Ciphertext w = a + a;
  Ciphertext m = w * c5 + c3 + c3;
  Ciphertext zt2 = z * t2;
  Ciphertext main = zt2 * m;
  Ciphertext k = z * c1 - z * c3 - z * c5;
  Ciphertext result = main + k + c0;
  result.set_output("y");
}
```