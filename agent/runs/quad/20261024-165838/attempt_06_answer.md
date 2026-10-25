I'm out of ideas that could beat the baseline. Every correct evaluation of x² needs one ct-ct multiply with relinearization and rescale, which is exactly what the baseline does. The only other circuit I could build was the 2-multiply variant, and it was twice as slow. `square(x)` gave a different circuit but was not confirmed faster (63.61 ms against 64.23 ms). Further attempts would only repeat these, so the baseline stays the best kernel.

```cpp
void fhe()
{
  Ciphertext x("x");
  Ciphertext y = x * x;
  y.set_output("y");
}
```