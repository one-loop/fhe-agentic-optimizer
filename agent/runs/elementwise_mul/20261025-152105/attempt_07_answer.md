I have nothing new to propose. `x0 * x1` as one ct-ct multiply with its relinearization and rescale is the minimum for this function. Every exact alternative adds multiplications or needs a constant that CKKS forbids. The baseline kernel is my final answer.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Ciphertext x1("x1");
  Ciphertext y = x0 * x1;
  y.set_output("y");
}
```