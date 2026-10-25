The timing for this run was 0.79 ms, again within the noise range of the same single ciphertext add. The kernel has no multiplications, rotations, or level consumption, so no reformulation can reduce the work. I'm resubmitting the minimal kernel.

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