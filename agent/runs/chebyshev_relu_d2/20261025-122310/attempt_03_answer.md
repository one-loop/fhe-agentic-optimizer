The best kernel computes z = x·alpha, then (c1 + 2·c2·z)·z, so it needs two ct-pt multiplications and one ct-ct multiplication. I couldn't find a way to use fewer of them, because every constant has to be applied through a ciphertext multiplication.

This attempt only moves the doubling onto the input, using x+x (a cheap ciphertext add at level 1) before the c2 product instead of g+g afterwards. It computes the same function and the circuit may be slightly different.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext c0("c0");
  Plaintext c1("c1");
  Plaintext c2("c2");
  Ciphertext z = x * alpha;
  Ciphertext x2 = x + x;
  Ciphertext g2 = x2 * c2;
  Ciphertext w = g2 * alpha;
  Ciphertext m = w + c1;
  Ciphertext r = m * z;
  r = r - c2;
  r = r + c0;
  r.set_output("y");
}
```