I have no cheaper circuit to propose, and I've now resubmitted the baseline three times with the same result. A single x0 * x1 already uses the minimum: one ct-ct multiply, one relinearization and one rescale. Every alternative adds operations or needs constants that CKKS doesn't allow here. The baseline remains the best kernel I can offer.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Ciphertext x1("x1");
  Ciphertext y = x0 * x1;
  y.set_output("y");
}
```