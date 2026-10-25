Same situation: the kernel is one ciphertext-ciphertext multiply, which already includes the relinearization and rescale. Results have settled at about 64.9 ms across three runs, so the remaining gap to the baseline is run-to-run variation, not a code change. I don't have a formulation that removes any of these operations, so I'm submitting the same kernel again.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Ciphertext x1("x1");
  Ciphertext y = x0 * x1;
  y.set_output("y");
}
```