I have no cheaper valid circuit for the final attempt. One ct-ct multiply with its relinearization and rescale is the minimum for `x0 * x1`. The exact alternatives all add multiplications or need a constant that CKKS forbids. The baseline kernel stays as my answer.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Ciphertext x1("x1");
  Ciphertext y = x0 * x1;
  y.set_output("y");
}
```