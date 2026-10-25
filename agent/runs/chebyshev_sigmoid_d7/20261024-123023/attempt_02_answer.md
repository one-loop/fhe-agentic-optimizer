Idea: the scale mismatch came from adding t5·c5 and t7·c7, which both land at level 5 with different multiplication histories. I fold each coefficient into a different multiplication stage so that every term in the final sum sits at a different level: c1 and c3 terms at levels 2 and 3, c5 at level 4, c7 at level 5. This keeps the depth at about 5 and avoids same-level sums of different history.

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
  Plaintext c0("c0");
  Ciphertext z = x * alpha;
  Ciphertext z2 = square(z);
  Ciphertext t2 = z2 + z2 - one;
  Ciphertext t2s = square(t2);
  Ciphertext t4 = t2s + t2s - one;
  Ciphertext zt2 = z * t2;
  Ciphertext t3 = zt2 + zt2 - z;

  Ciphertext zc1 = z * c1;
  Ciphertext zc3 = z * c3;
  Ciphertext zc5 = z * c5;
  Ciphertext zc7 = z * c7;

  Ciphertext a3 = zc3 * t2;
  Ciphertext term3 = a3 + a3 - zc3;

  Ciphertext t2c5 = t2 * c5;
  Ciphertext a5 = t2c5 * t3;
  Ciphertext term5 = a5 + a5 - zc5;

  Ciphertext t3c7 = t3 * c7;
  Ciphertext a7 = t3c7 * t4;
  Ciphertext term7 = a7 + a7 - zc7;

  Ciphertext result = zc1 + term3;
  result = result + term5;
  result = result + term7;
  result = result + c0;
  result.set_output("y");
}
```