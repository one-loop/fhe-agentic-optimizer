The kernel is already optimal, and this run (0.75 ms) falls within the same noise band as the baseline (0.73 ms) and earlier retries (0.75 and 0.84 ms). It performs one ciphertext add with no multiplications, rotations, or level consumption, so there is no computation left to remove. I'm submitting the same kernel again.

```cpp
// Elementwise add: y = x0 + x1. Minimal form (one ciphertext add, depth 0).

void fhe()
{
  Ciphertext x0("x0");
  Ciphertext x1("x1");
  Ciphertext y = x0 + x1;
  y.set_output("y");
}
```