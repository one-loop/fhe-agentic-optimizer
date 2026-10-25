The 64.83 ms result is within noise of the 65.53 ms baseline, and the operation count is unchanged: one ciphertext-ciphertext multiply, one relinearization, one rescale. I considered alternatives. Using squares, as in ((x0+x1)^2 - (x0-x1)^2)/4, needs two multiplications instead of one, so it would be slower. Any rewrite that keeps the output the same still needs at least one ct-ct multiply, so nothing in the kernel can cut the dominant cost. I'm keeping the same minimal kernel.

```cpp
// Elementwise product: one ciphertext x ciphertext multiply (depth 1, relinearized, rescaled).
// No cheaper formulation exists: any squaring-based identity needs two multiplications.

void fhe()
{
  Ciphertext x0("x0");
  Ciphertext x1("x1");
  Ciphertext y = x0 * x1;
  y.set_output("y");
}
```