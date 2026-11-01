The kernel is at its floor, so I'm keeping it as is. Attempt 4 measured 31.55 ms, in line with the baseline's 31.49 ms and the other runs (31.51–31.56 ms). The variation is measurement noise, not a difference between kernels.

The computation needs exactly one ciphertext-ciphertext multiply, and this kernel does exactly that, with its relinearize and rescale. There is no rotation, plaintext multiply, alignment or extra level to remove, and any restructuring would add operations.

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