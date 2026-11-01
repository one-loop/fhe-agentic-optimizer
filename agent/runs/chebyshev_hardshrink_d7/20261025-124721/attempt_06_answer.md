The identities a5 = a3² + a3 − 1 and a7 = a5·a3 + a5 − a3 avoid `v` in the later products and use a true square, which is cheaper than a general multiply. Everything else follows attempt 3.

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
  Ciphertext u = z * x;
  Ciphertext a3 = u - one - one - one;
  Ciphertext a5 = square(a3) + a3 - one;
  Ciphertext a7 = a5 * a3 + a5 - a3;
  Ciphertext p = a3 * c3 + a5 * c5 + a7 * c7 + c1;
  Ciphertext result = z * p;
  result.set_output("y");
}
```