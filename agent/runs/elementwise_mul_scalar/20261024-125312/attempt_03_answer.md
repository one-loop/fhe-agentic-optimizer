The kernel is already at the minimum: one ciphertext-plaintext multiply with its rescale, and no rotations, ciphertext multiplies or alignment ops. The differences between runs (8.80, 8.89, 9.46 ms) are measurement noise on identical programs. Nothing in the DSL can produce an arbitrary plaintext scalar product more cheaply, so I'm resubmitting the best kernel.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Plaintext s("s");
  Ciphertext y = x0 * s;
  y.set_output("y");
}
```