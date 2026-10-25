Attempt 7 measured 0.80 ms with the same single-add program. Across all runs the times varied (0.72 to 1.71 ms) while the operation counts stayed identical, so the variation is timing noise. One ciphertext add is the minimum for x0 + x1, so I'm submitting the kernel unchanged.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Ciphertext x1("x1");
  Ciphertext y = x0 + x1;
  y.set_output("y");
}
```