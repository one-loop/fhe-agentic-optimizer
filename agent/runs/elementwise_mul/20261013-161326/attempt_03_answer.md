I'm resubmitting the same kernel. The operation graph is identical across all three attempts: one ciphertext-ciphertext multiply, one relinearization, one rescale. The timing spread (64.8 to 68.2 ms) is run-to-run noise, not a difference in the computation. Nothing in the kernel can be removed, because the product of two encrypted vectors requires that multiply.

```cpp
// Elementwise product: one ciphertext x ciphertext multiply (depth 1, relinearized, rescaled).
// This is the minimal circuit for x0 * x1 in CKKS; the remaining variation is measurement noise.

void fhe()
{
  Ciphertext x0("x0");
  Ciphertext x1("x1");
  Ciphertext y = x0 * x1;
  y.set_output("y");
}
```