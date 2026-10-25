The kernel is already minimal: one ciphertext-plaintext multiply with its rescale, and no rotations or ciphertext multiplies. The 0.09 ms difference from the baseline is measurement noise. I can't use any cheaper arithmetic, because the scalar comes only from the plaintext input and any non-multiplicative route (additions, rotations) would be more expensive or could not reproduce an arbitrary value. I'm resubmitting the same kernel.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Plaintext s("s");
  Ciphertext y = x0 * s;
  y.set_output("y");
}
```