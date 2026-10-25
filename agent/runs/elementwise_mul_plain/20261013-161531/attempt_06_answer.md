I'm resubmitting the same kernel. Attempt 5 measured 13.52 ms against 13.01 ms for attempt 4, but the op graph is identical in every run (one ct×pt multiply, one rescale, nothing else), so the spread is timing noise. The minimum for this function is one ciphertext-plaintext multiply, and nothing I can legally write does less.

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