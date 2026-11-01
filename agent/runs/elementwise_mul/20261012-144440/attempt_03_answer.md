Attempt 2 measured 31.51 ms, the same as the baseline within noise. The kernel is already at the minimum of one ciphertext-ciphertext multiply, one relinearize and one rescale, so there is nothing left to remove. I'm returning the baseline unchanged as the final kernel.

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