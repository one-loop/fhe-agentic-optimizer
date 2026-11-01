I'm stopping here rather than spending more attempts. Four runs of this kernel have measured 31.49–31.54 ms, which is noise around the same single operation, and no alternative can do less work. Elementwise x0·x1 needs one ciphertext-ciphertext multiply, which is what this kernel does, with its relinearize and rescale.

Final kernel (same as the baseline):

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