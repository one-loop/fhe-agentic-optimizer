I don't have a cheaper circuit to propose. `x0 * x1` is one ct-ct multiply with its relinearization and rescale. Every exact alternative adds a multiplication or needs a constant that CKKS forbids, and I covered those cases in my last two replies. Resubmitting it a fourth time gains nothing, so I'm stopping here. The baseline is the best valid kernel I can find.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Ciphertext x1("x1");
  Ciphertext y = x0 * x1;
  y.set_output("y");
}
```