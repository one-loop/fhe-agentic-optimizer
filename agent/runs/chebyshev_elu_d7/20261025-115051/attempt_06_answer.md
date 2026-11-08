The previous kernel was correct but only about 2% faster. This version folds the c3·t3 and c5·t3 terms into the final t3 multiplication by adding the plaintext constants `c3` and `c5` inside the factor. That removes two plaintext multiplications and costs two cheap plaintext adds. The output is unchanged.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext one("one");
  Plaintext c0("c0"), c1("c1"), c2("c2"), c3("c3"), c4("c4"), c5("c5"), c6("c6"), c7("c7");
  Ciphertext z = x * alpha;
  Ciphertext z2 = square(z);
  Ciphertext t2 = z2 + z2 - one;
  Ciphertext u = t2 + t2 - one;
  Ciphertext t3 = z * u;
  Ciphertext m4 = z * t3;
  Ciphertext t4 = m4 + m4 - t2;

  Ciphertext cz5 = z * c5;
  Ciphertext g = (cz5 + cz5) + c4;
  Ciphertext B = t4 * g;

  Ciphertext w = t3 * c6 + t4 * c7;
  Ciphertext v = w + w + c3 - c5;
  Ciphertext C = t3 * v - c6 - z * c7;

  Ciphertext result = z * c1 + t2 * c2;
  result = result + B;
  result = result + C;
  result = result + c0;
  result.set_output("y");
}
```