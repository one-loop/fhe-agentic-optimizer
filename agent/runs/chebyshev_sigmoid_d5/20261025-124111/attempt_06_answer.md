This version moves the plaintext multiplies onto z early, at level 2, so they run in parallel with t2:
- zc5 = z·c5, zc3 = z·c3 and zc1 = z·c1.
- q = zc5·t2.
- The main product is t2·(2q + e) with e = zc3 − zc5.
- The linear remainder is zc1 − zc3 − zc5, which equals z·(c1 − c3 − c5).

This cuts the multiplicative depth to 4, at the cost of two more plaintext multiplies than attempt 3.

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
  Ciphertext zc5 = z * c5;
  Ciphertext zc3 = z * c3;
  Ciphertext zc1 = z * c1;
  Ciphertext e = zc3 - zc5;
  Ciphertext q = zc5 * t2;
  Ciphertext g = q + q + e;
  Ciphertext h = t2 * g;
  Ciphertext result = h + h + zc1 - zc3 - zc5 + c0;
  result.set_output("y");
}
```