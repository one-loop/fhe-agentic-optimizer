The best measured time is now attempt 4 at 13.01 ms. The kernel's op graph hasn't changed across any attempt, so the variation is measurement noise. I'm resubmitting the same kernel as the best candidate.

```cpp
// Elementwise multiply by a plaintext vector: minimal form (one ct x pt multiply, depth 1).
void fhe()
{
  Ciphertext x0("x0");
  Plaintext p("p");
  Ciphertext y = x0 * p;
  y.set_output("y");
}
```