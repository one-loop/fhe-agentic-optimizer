Squaring was essentially the same cost as the plain multiply (about 1% faster), so the saving is too small to confirm. The remaining cost is one multiply, one relinearization and one rescale. Computing x² without a ciphertext multiplication isn't possible with only additions and rotations, since I have no plaintext inputs to work with.

One option is to drop the relinearization: a degree-2 ciphertext would normally be relinearized by CHEHAB automatically, so I can't control that. Another is the identity x² = ((x+x)² )/4, but dividing by 4 needs a constant, which is not allowed. A third is to use x*x via (x+x)*(x) and so on, but that still takes one multiply and adds more work.

Without constants or plaintext inputs the circuit can't be made cheaper than one multiply. I'll try a variant with a different structure, which may still shift the operation counts. The identity x² = ((x+x)·x)/2 needs a division by 2, so that isn't usable either. I'll resubmit the square form in a slightly different shape, which keeps the result correct.

```cpp
void fhe()
{
  Ciphertext x("x");
  Ciphertext y = square(x);
  y.set_output("y");
}
```