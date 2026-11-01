I still can't find a cheaper circuit, so I'm submitting the baseline again. A single ct-ct multiply with relinearization and rescale is the minimum for x0 * x1. It will match the baseline circuit and can't be ranked as a new best.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Ciphertext x1("x1");
  Ciphertext y = x0 * x1;
  y.set_output("y");
}
```