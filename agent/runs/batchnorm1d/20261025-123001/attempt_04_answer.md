This attempt uses a different circuit: negate x, multiply by A, subtract B, then negate again. It adds two cheap ciphertext negations but keeps a single plaintext multiply, rescale and plaintext add or subtract. It is unlikely to be faster, but it is a legal structural variant.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext a("A");
  Plaintext b("B");
  Ciphertext nx = -x;
  Ciphertext t = nx * a - b;
  Ciphertext y = -t;
  y.set_output("y");
}
```