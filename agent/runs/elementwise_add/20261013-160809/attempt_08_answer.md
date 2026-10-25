This run matched the baseline at 0.73 ms. The kernel is a single ciphertext add with no multiplications, rotations, or level use, which is the minimum for computing x0 + x1. The remaining variation is measurement noise, so I'm resubmitting it as the final kernel.

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