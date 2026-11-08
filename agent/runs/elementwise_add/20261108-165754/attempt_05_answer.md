Nothing has changed: `y = x0 + x1` is one ciphertext add, and no circuit with fewer operations can compute it. Any alternative would add operations or be normalized back to the same circuit, so it cannot be faster. I'm resubmitting the same kernel, which will again match the baseline.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Ciphertext x1("x1");
  Ciphertext y = x0 + x1;
  y.set_output("y");
}
```