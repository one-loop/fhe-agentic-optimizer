The baseline already uses the minimum possible work for this task: one ciphertext-ciphertext multiply, which is the only operation that can produce the product x0·x1 slot-wise. It has depth 1, no rotations, and no alignment or extra rescales. Any rewrite would add operations rather than remove them, so I am returning the same kernel.

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