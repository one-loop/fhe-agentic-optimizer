The baseline is already a single ciphertext add, which is the minimum operation count for this function. Any other formulation (extra rotations, multiplications, or plaintext ops) would only add work. I can't remove the one add, so I'm resubmitting the same minimal kernel; it will not beat the baseline's circuit, but it is correct and optimal in operation count.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Ciphertext x1("x1");
  Ciphertext y = x0 + x1;
  y.set_output("y");
}
```