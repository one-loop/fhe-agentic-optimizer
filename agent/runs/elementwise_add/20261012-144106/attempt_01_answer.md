The baseline already does the minimum possible work: one ciphertext-ciphertext addition, with no multiplications, rotations, or level changes. Any alternative would add operations, so I'm returning the same kernel unchanged.

```cpp
// Elementwise add: y = x0 + x1. This is already minimal (a single ciphertext add,
// no multiplications, rotations, or level consumption), so it is kept as is.

void fhe()
{
  Ciphertext x0("x0");
  Ciphertext x1("x1");
  Ciphertext y = x0 + x1;
  y.set_output("y");
}
```