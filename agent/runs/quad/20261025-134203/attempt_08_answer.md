I'm not going to spend the last attempt on another duplicate. I have no valid circuit that is cheaper than one ciphertext multiply, relinearization and rescale. Every alternative I could construct either simplifies back to the baseline or adds operations.

The best result remains the baseline at 64.31 ms. `square(x)` measured about 1% faster in attempt 1 (63.51 ms against 64.23 ms), but that is below the 3% needed to confirm a win.

This is my final kernel:

```cpp
void fhe()
{
  Ciphertext x("x");
  Ciphertext y = square(x);
  y.set_output("y");
}
```