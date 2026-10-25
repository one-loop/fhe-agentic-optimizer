I have no cheaper circuit to propose. The baseline's single multiply with relinearization and rescale is the minimum for x², and every structural change I can construct adds operations. I'm resubmitting the same kernel, so it will be reported as identical to the baseline.

```cpp
void fhe()
{
  Ciphertext x("x");
  Ciphertext y = x * x;
  y.set_output("y");
}
```