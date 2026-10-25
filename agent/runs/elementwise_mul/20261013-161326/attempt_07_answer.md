I'm returning the same kernel again. Attempt 6 used the identical circuit (one ciphertext-ciphertext multiply, one relinearization, one rescale, depth 1) and timed at 66.6 ms, in the same 64.8 to 68.2 ms noise band as the other six attempts. No rewrite can remove the multiplication, since x0 * x1 needs at least one ciphertext-ciphertext multiply. Attempt 1 remains the best measured version.

```cpp
// Elementwise product: one ciphertext x ciphertext multiply (depth 1, relinearized, rescaled).
// This is the minimal circuit for x0 * x1 in CKKS.

void fhe()
{
  Ciphertext x0("x0");
  Ciphertext x1("x1");
  Ciphertext y = x0 * x1;
  y.set_output("y");
}
```