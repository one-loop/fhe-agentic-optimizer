The previous attempt measured 0.75 ms against a baseline of 0.73 ms. That gap is timing noise, since the kernel is identical and contains one ciphertext add. No kernel can do less work than this: the output needs one addition of the two inputs, and any restructuring would only add operations. I'm keeping the same kernel and reporting the baseline as the best result.

```cpp
// Elementwise add: y = x0 + x1. Minimal form (one ciphertext add, depth 0).

void fhe()
{
  Ciphertext x0("x0");
  Ciphertext x1("x1");
  Ciphertext y = x0 + x1;
  y.set_output("y");
}
```