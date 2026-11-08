The scale mismatch comes from terms at the same level with different multiplication counts. This version multiplies each coefficient in early, so the four terms land at distinct levels (2, 3, 4, 5), where CHEHAB aligns them for free. For example, c5·T5 = 2·(c5·t2)·t3 − c5·z.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext one("one");
  Plaintext c0("c0"), c1("c1"), c3("c3"), c5("c5"), c7("c7");
  Ciphertext z = x * alpha;
  Ciphertext s = square(z);
  Ciphertext t2 = s + s - one;
  Ciphertext zt2 = z * t2;
  Ciphertext t3 = zt2 + zt2 - z;
  Ciphertext t2s = square(t2);
  Ciphertext t4 = t2s + t2s - one;
  Ciphertext p7 = t3 * t4;
  Ciphertext t7 = p7 + p7 - z;
  Ciphertext term7 = t7 * c7;

  Ciphertext zc1 = z * c1;
  Ciphertext zc3 = z * c3;
  Ciphertext h3 = zc3 * t2;
  Ciphertext term3 = h3 + h3 - zc3;

  Ciphertext zc5 = z * c5;
  Ciphertext t2c5 = t2 * c5;
  Ciphertext h5 = t2c5 * t3;
  Ciphertext term5 = h5 + h5 - zc5;

  Ciphertext r = zc1 + term3;
  r = r + term5;
  r = r + term7;
  r = r + c0;
  r.set_output("y");
}
```