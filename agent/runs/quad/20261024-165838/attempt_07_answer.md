I have no new idea that lowers the cost, and I'm not going to submit another variant that is known to be slower or identical. The baseline's single multiply with relinearization and rescale is the minimum for x² given only one ciphertext input, so the baseline remains the best kernel.

```cpp
void fhe()
{
  Ciphertext x("x");
  Ciphertext y = x * x;
  y.set_output("y");
}
```