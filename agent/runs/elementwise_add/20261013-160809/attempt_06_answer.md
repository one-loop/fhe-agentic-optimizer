I'm submitting the same kernel once more. The measured times across attempts (0.73, 0.75, 0.84, 0.75, 0.83 ms) vary only by run-to-run noise, and the computation is identical each time. It is one ciphertext add with no multiplications, rotations, or level consumption, so no rewrite can cut work further. The baseline remains the best result.

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