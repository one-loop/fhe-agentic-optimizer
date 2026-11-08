Idea: keep the lower-depth doubling-identity scheme (t4 = 2·t2²−1, t5 = 2·t2·t3−z, t6 = 2·t3²−1, t7 = 2·t3·t4−z), but fix the scale mismatch by changing the summation order. The accumulator now starts from the deepest terms (c5·t5, c6·t6, c7·t7), and each lower-level term is added afterwards. Every later addition then pairs the accumulator at a deeper level with a term at a shallower level, so CHEHAB aligns them for free. The only same-level sum is the one among t5, t6 and t7, which are all products of rescaled ciphertexts and should share a scale history.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext one("one");
  Ciphertext z = x * alpha;
  Ciphertext z2 = square(z);
  Ciphertext t2 = z2 + z2 - one;
  Ciphertext a3 = z * t2;
  Ciphertext t3 = a3 + a3 - z;
  Ciphertext s4 = square(t2);
  Ciphertext t4 = s4 + s4 - one;
  Ciphertext a5 = t2 * t3;
  Ciphertext t5 = a5 + a5 - z;
  Ciphertext s6 = square(t3);
  Ciphertext t6 = s6 + s6 - one;
  Ciphertext a7 = t3 * t4;
  Ciphertext t7 = a7 + a7 - z;

  Plaintext c0("c0"), c1("c1"), c2("c2"), c3("c3"), c4("c4"), c5("c5"), c6("c6"), c7("c7");
  Ciphertext result = t5 * c5;
  result = result + t6 * c6;
  result = result + t7 * c7;
  result = result + t4 * c4;
  result = result + t3 * c3;
  result = result + t2 * c2;
  result = result + z * c1;
  result = result + c0;
  result.set_output("y");
}
```