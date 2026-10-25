The baseline already uses the minimum possible work: one ciphertext-ciphertext addition, no multiplications, no rotations, and no level consumption. Any alternative formulation would add operations, so I'm returning the same kernel unchanged.

```cpp
// Elementwise add: y = x0 + x1. Already minimal (one ciphertext add, depth 0).

void fhe()
{
  Ciphertext x0("x0");
  Ciphertext x1("x1");
  Ciphertext y = x0 + x1;
  y.set_output("y");
}
```