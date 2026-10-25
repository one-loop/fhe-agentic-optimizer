The baseline form is already minimal: one ciphertext-ciphertext multiply, one relinearization and one rescale. The two measurements (64.64 ms and 65.06 ms) differ only by noise, and `square(x)` gave 67.50 ms. I haven't found a variant that removes any operation, since the DSL has no constants or extra inputs to build a cheaper route to x². Any alternative, such as splitting x into parts or using rotations, adds work.

I'm resubmitting the baseline form unchanged.

```cpp
void fhe()
{
  Ciphertext x("x");
  Ciphertext y = x * x;
  y.set_output("y");
}
```