I'm keeping the same kernel. Attempt 5 measured 31.54 ms, which again falls within the 31.49–31.56 ms noise band of every run so far. Elementwise x0·x1 requires one ciphertext-ciphertext multiply, plus the relinearize and rescale it carries, and the kernel does exactly that. Nothing can be removed without changing the result.

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