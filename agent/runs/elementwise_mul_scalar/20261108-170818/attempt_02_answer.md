I don't have a cheaper circuit to propose. The computation needs a product with a run-time value `s`, and the minimum for that is one ciphertext-plaintext multiplication plus one rescale, which is what the baseline already does. Every alternative I can build (for example splitting the product into sums, doubling, or routing through a ciphertext-ciphertext multiply) adds operations or levels. Resubmitting the same kernel can't win, so I'm keeping the baseline as the best.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Plaintext s("s");
  Ciphertext y = x0 * s;
  y.set_output("y");
}
```