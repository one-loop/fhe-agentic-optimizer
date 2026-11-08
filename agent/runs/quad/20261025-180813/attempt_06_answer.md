I don't have a cheaper circuit to offer. The baseline's single multiply (with its relinearize and rescale) is already the minimum for x². CHEHAB also removes the redundant terms I added, so those variants collapse to the baseline's circuit. I'm submitting the baseline form again rather than a variant that would only add cost.

```cpp
void fhe()
{
  Ciphertext x("x");
  Ciphertext y = x * x;
  y.set_output("y");
}
```