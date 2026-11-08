I have no new structural change that I expect to beat the current circuit. Attempts 4 and 6 are tied within noise, with the same multiply counts and depth. This resubmits attempt 6's form and has not been timed again.

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
  Ciphertext zz = z + z;
  Ciphertext s = square(zz);
  Ciphertext s2 = s - one;
  s2 = s2 - one;
  Ciphertext s3 = s2 - one;
  Ciphertext b = s2 * c5;
  b = b + c3;
  Ciphertext g = s3 * b;
  g = g + c1;
  g = g - c5;
  Ciphertext y = z * g;
  y = y + c0;
  y.set_output("y");
}
```