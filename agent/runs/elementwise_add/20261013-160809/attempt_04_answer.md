The kernel is unchanged, and the timing spread (0.73 to 0.84 ms) is run-to-run noise on a single ciphertext add. The graph has one add and nothing else, so no rewrite can reduce the work. I'm resubmitting the same minimal kernel.

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