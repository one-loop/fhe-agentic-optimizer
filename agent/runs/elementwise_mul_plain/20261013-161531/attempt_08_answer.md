I'm submitting the same kernel as the final answer. Attempt 7 measured 13.26 ms, again within the 13.0–13.5 ms noise band, and the operation graph has not changed across any attempt: one ciphertext-plaintext multiply and one rescale. This is the minimum for the task, so there is nothing further to optimize.

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