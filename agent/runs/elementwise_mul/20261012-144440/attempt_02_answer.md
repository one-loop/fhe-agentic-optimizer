I can't get below one ciphertext-ciphertext multiply, so there's no faster kernel to propose. The result (31.56 ms against 31.49 ms) is within run-to-run noise. The same single multiply, relinearize and rescale is what remains, so I'm returning the baseline unchanged.

```cpp
// Elementwise mul: y = x0 * x1. A single ciphertext-ciphertext multiply is
// already the minimum work (one multiply, one relinearize, one rescale, depth 1).
void fhe()
{
  Ciphertext x0("x0");
  Ciphertext x1("x1");
  Ciphertext y = x0 * x1;
  y.set_output("y");
}
```