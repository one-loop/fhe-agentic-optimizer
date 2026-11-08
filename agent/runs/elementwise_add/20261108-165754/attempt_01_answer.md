The baseline is already a single ciphertext add, which is the minimum for this function: there are no multiplications, rotations, rescales, or levels to remove. Any other formulation (for example, adding zero or splitting the add) would only add operations or leave the circuit unchanged, so it cannot win. I'm resubmitting the same minimal kernel.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Ciphertext x1("x1");
  Ciphertext y = x0 + x1;
  y.set_output("y");
}
```