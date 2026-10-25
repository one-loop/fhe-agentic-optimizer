The kernel is already at the minimum: one plaintext encode and one ciphertext add. Any rewrite would still need both, and the available operators offer no cheaper way to add a plaintext vector to a ciphertext. The 0.01 ms difference from the baseline is measurement noise. I'm resubmitting the same kernel.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Plaintext p("p");
  Ciphertext y = x0 + p;
  y.set_output("y");
}
```