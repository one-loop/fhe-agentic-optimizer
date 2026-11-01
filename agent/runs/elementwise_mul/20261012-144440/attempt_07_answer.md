Attempt 6 measured 31.50 ms, the same as the baseline within noise. The kernel is already minimal: one ciphertext-ciphertext multiply with its relinearize and rescale, depth 1, no rotations and no alignment work. I'm returning it unchanged.

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