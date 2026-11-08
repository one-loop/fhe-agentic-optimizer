I have no cheaper circuit for the last attempt. Squaring needs one ciphertext multiplication with its relinearize and rescale. There are no plaintext inputs to supply constants, and I can't emit mod switches myself. Every alternative I can build adds operations or computes the wrong function, such as 2x² or 4x². The baseline `x * x` remains the best kernel, and I'm submitting it again.

```cpp
void fhe()
{
  Ciphertext x("x");
  Ciphertext y = x * x;
  y.set_output("y");
}
```