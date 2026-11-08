I have no cheaper circuit for the last attempt either. The baseline `x0 * x1` is already the minimum: one ciphertext-ciphertext multiply, one relinearization and one rescale. Everything I can think of adds operations or needs a constant the task doesn't supply, so it would be slower or wrong.

The baseline stays the best result at 64.65 ms. This is the same kernel again.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Ciphertext x1("x1");
  Ciphertext y = x0 * x1;
  y.set_output("y");
}
```