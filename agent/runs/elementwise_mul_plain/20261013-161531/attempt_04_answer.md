The kernel is at its floor, so I'm resubmitting it unchanged. Attempt 3 measured 13.21 ms, within the same noise band as the others (13.15–14.66 ms), and the op graph is identical each time: one ciphertext-plaintext multiply and one rescale.

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