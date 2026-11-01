The kernel is already at the floor for this task: one scalar ciphertext-plaintext multiply, one rescale, no rotations or ciphertext-ciphertext multiplies, and depth 1. There is no cheaper way to compute x0 * s, and the rewrites available in this DSL would only add operations. I'm resubmitting the same kernel.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Plaintext s("s");
  Ciphertext y = x0 * s;
  y.set_output("y");
}
```