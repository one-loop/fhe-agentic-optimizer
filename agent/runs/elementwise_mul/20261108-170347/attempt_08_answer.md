I have no cheaper circuit for the last attempt either. A single ciphertext-ciphertext multiply with its relinearization and rescale is the minimum for `x0 * x1`. The baseline at 6.87 ms remains the best result, so this is the same kernel again.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Ciphertext x1("x1");
  Ciphertext y = x0 * x1;
  y.set_output("y");
}
```