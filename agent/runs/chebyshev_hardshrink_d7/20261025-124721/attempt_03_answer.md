Since z·x = x²/4 = 4w, I can compute u = z·x directly and get v = u − 2. This drops the doubling adds. The multiply count stays the same, but the multiply now takes x at the input level rather than a second z.

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
  Ciphertext v = u - one - one;
  Ciphertext a3 = v - one;
  Ciphertext a5 = a3 * v - one;
  Ciphertext a7 = a5 * v - a3;
  Ciphertext p = a3 * c3 + a5 * c5 + a7 * c7 + c1;
  Ciphertext result = z * p;
  result.set_output("y");
}
```