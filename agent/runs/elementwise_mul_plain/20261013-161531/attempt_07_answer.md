I'm resubmitting the same kernel again. Attempt 6 measured 13.32 ms, in the same 13.0–13.5 ms band as the other runs, and the operation graph is unchanged each time: one ciphertext-plaintext multiply and one rescale. Since that is the minimum work the function needs, further attempts would only be measuring noise.

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