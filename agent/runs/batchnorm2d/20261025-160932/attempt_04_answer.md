I can't find a cheaper circuit. The multiply, rescale and plaintext add are all required, and B/A is not allowed. I'll try one more structural variant: multiply the negated ciphertext by A, then subtract that product from B's contribution. It computes the same function but may change how CHEHAB aligns scales.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext a("A");
  Plaintext b("B");
  Ciphertext nx = -x;
  Ciphertext t = nx * a;
  Ciphertext y = b - t;
  y.set_output("y");
}
```