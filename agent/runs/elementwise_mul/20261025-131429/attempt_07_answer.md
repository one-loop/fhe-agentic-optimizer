I have no cheaper circuit to propose. The only way to compute x0 * x1 here is one ct-ct multiply, one relinearization and one rescale, which is what the baseline already does. Any other structure adds operations or needs constants CKKS doesn't allow. I'm submitting the baseline again, and the result will be the same as in attempts 1 to 5.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Ciphertext x1("x1");
  Ciphertext y = x0 * x1;
  y.set_output("y");
}
```