I'm stopping the search here and resubmitting the best kernel (attempt 1, 13.15 ms). The op graph is the same in every attempt: one ciphertext-plaintext multiply and one rescale. The spread between 13.15 ms and 14.66 ms is timing noise, not a difference in the kernel.

I don't see a legal way to do less work for this function. Any restructuring would add operations, such as extra multiplies or alignment steps, so the baseline form is already the floor.

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