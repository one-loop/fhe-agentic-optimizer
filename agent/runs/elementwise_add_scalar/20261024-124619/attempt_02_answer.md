The kernel is already a single add, which is the minimum operation count. The 0.01 ms difference between runs is measurement noise. Any other formulation (extra multiplies, rotations, or alignment) would only add cost, so I'm resubmitting the same kernel.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Plaintext s("s");
  Ciphertext y = x0 + s;
  y.set_output("y");
}
```