Idea: double the multiplier before the product instead of after. With m2 = 2·m, the product is u2·m2 = 2·u2·m directly. This removes the `pm+pm` addition at the deep level and does the doubling on the shallower m. The `low − a6 − m` term becomes `low − a6 − ½m2`; to avoid a half constant I express the correction through ciphertext terms already available (a6 and c4 only). Everything stays in plaintext-multiplication-free form. The circuit gets a different addition structure while keeping 3 ciphertext multiplications and 4 plaintext multiplications.

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
  Ciphertext t2 = u + u - one;
  Ciphertext u2 = t2 * t2;
  Ciphertext a6 = t2 * c6;
  Ciphertext a6d = a6 + a6;
  Ciphertext m = a6d + c4;
  Ciphertext m2 = m + m;
  Ciphertext pm = u2 * m2;
  Ciphertext low = t2 * c2 + z * c1;
  low = low - a6 - a6d;
  low = low - c4;
  Ciphertext result = pm + low;
  result = result + c0;
  result.set_output("y");
}
```