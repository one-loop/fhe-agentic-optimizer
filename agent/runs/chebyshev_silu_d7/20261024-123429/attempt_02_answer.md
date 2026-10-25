Idea: with t = T2 = 2z²−1, the polynomial is y = c0 + c1·z + c2·t + c4(2t²−1) + c6(4t³−3t). Grouping gives y = (c0−c4) + c1·z + t·g, where g = c2 − 3c6 + 2c4·t + 4c6·t². Plaintext constants can be added to a ciphertext, so g is built from sums of ciphertexts and plaintexts without any plaintext-plaintext arithmetic. This keeps 3 ct-ct multiplications but needs only 3 ct-pt multiplications instead of 5, plus the one for z.

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
  Ciphertext t = u + u - one;
  Ciphertext v = t * t;
  Ciphertext a = t * c4;
  Ciphertext b = v * c6;
  Ciphertext b2 = b + b;
  Ciphertext b4 = b2 + b2;
  Ciphertext g = b4 + a + a + c2 - c6 - c6 - c6;
  Ciphertext r = t * g + z * c1;
  Ciphertext result = r + c0 - c4;
  result.set_output("y");
}
```