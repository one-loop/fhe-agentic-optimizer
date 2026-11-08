I still don't see a cheaper circuit. One ct-ct multiply with its relinearization and rescale is the minimum for x0 * x1, and any change I could make adds operations instead of removing them. I'm not going to submit a deliberately slower variant just to get a different circuit, so I'm leaving the kernel as the baseline. Resubmitting it will not be recorded as an improvement.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Ciphertext x1("x1");
  Ciphertext y = x0 * x1;
  y.set_output("y");
}
```